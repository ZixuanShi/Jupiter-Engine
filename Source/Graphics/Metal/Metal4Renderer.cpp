// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <cstddef>
#include <cstring>

#include "Graphics/ImGui/ImGuiLayer.h"
#include "Graphics/Shader/ShaderTypes.h"
#include "Metal4Renderer.h"

import jpt.Logger;
import jpt.Matrix44;
import jpt.PlatformPaths;
import jpt.Vertex;
import std;

// What lets the matrix cross to the GPU by memcpy: both are 16 column-major floats.
static_assert(sizeof(jpt::Mat44) == sizeof(simd_float4x4));
static_assert(std::is_standard_layout_v<jpt::Vertex>, "offsetof requires it");
static_assert(sizeof(jpt::Vertex) == 48, "Vertex layout is the vertex descriptor's contract");

namespace jpt
{
    constexpr MTL::PixelFormat kDepthFormat = MTL::PixelFormatDepth32Float;

    // Metal 4 binds constants by raw GPU address, whose alignment requirement is coarser than
    // sizeof(Uniforms), so the slots are strided rather than packed.
    constexpr uint32 kUniformStride = 256;
    static_assert(sizeof(Uniforms) <= kUniformStride, "A slot must hold one whole Uniforms");

    namespace
    {
        template<typename T>
        void Release(T*& pObject)
        {
            if (pObject)
            {
                pObject->release();
                pObject = nullptr;
            }
        }

        // Not a member: Metal4Renderer.h is included by Application.cppm's global module fragment,
        // and a semaphore member would drag `import std` in with it.
        std::counting_semaphore<kFramesInFlight> g_frameSemaphore{ kFramesInFlight };
    }

    bool Metal4Renderer::PreInit()
    {
        m_pDevice = MTL::CreateSystemDefaultDevice();
        if (!m_pDevice)
        {
            return false;
        }

        m_pQueue = m_pDevice->newMTL4CommandQueue();
        if (!m_pQueue)
        {
            Debug::Error("Metal 4 is unavailable on this device.");
            return false;
        }

        for (uint32 i = 0; i < kFramesInFlight; ++i)
        {
            m_pAllocators[i] = m_pDevice->newCommandAllocator();
            if (!m_pAllocators[i])
            {
                return false;
            }
        }

        NS::Error* pError = nullptr;
        MTL::ResidencySetDescriptor* pResidencyDesc = MTL::ResidencySetDescriptor::alloc()->init();
        m_pResidencySet = m_pDevice->newResidencySet(pResidencyDesc, &pError);
        pResidencyDesc->release();

        if (!m_pResidencySet)
        {
            Debug::Error("Residency set: {}", pError->localizedDescription()->utf8String());
            return false;
        }

        // Queue-wide rather than per command buffer: every frame uses the same allocations.
        m_pQueue->addResidencySet(m_pResidencySet);
        return true;
    }

    bool Metal4Renderer::Init(SurfaceHandle pMetalLayer)
    {
        if (!pMetalLayer)
        {
            return false;
        }

        m_pLayer = pMetalLayer;

        m_pLayer->setDevice(m_pDevice);
        m_pLayer->setPixelFormat(MTL::PixelFormatBGRA8Unorm_sRGB);
        m_pLayer->setFramebufferOnly(true); // Promises render-only access, letting Core Animation pick faster memory.

        NS::SharedPtr<NS::AutoreleasePool> pool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

        // One buffer, kFramesInFlight slots. Shared storage because the CPU writes it every frame
        // and Apple silicon has no separate device memory to stage it into.
        m_pUniforms = m_pDevice->newBuffer(kFramesInFlight * kUniformStride, MTL::ResourceStorageModeShared);
        if (!m_pUniforms)
        {
            return false;
        }

        if (!CreatePipeline())
        {
            return false;
        }

        UpdateResidency();

        // Layout persistence lives with the rest of the editor's preferences. In Release this
        // is a no-op stub and ImGui is not linked at all.
        const std::filesystem::path iniPath = GetSavedDir() / "Preferences" / "imgui.ini";

        std::error_code error;
        std::filesystem::create_directories(iniPath.parent_path(), error);

        // The backend sizes its own per-frame buffers off kFramesInFlight, so it must agree with
        // the ring above or it will overwrite geometry the GPU is still reading.
        return ImGuiInit(m_pDevice, m_pQueue, static_cast<int>(kFramesInFlight), iniPath.string().c_str());
    }

    bool Metal4Renderer::BeginFrame()
    {
        if (!m_pLayer || !m_pQueue || !m_pPipeline)
        {
            return false;
        }

        // Blocks until the frame that last held this slot has completed. Released by the commit
        // feedback handler in EndFrame, or explicitly below when the frame never gets that far.
        g_frameSemaphore.acquire();

        // Drawables are autoreleased. Without a pool draining each frame the finite pool
        // exhausts and the app stalls within seconds. Raw rather than NS::SharedPtr, because
        // the header only forward-declares metal-cpp and SharedPtr needs the complete type.
        m_pFramePool = NS::AutoreleasePool::alloc()->init();

        m_frameIndex = (m_frameIndex + 1) % kFramesInFlight;

        // Frees every command previously allocated from it, which the semaphore above is what
        // makes safe -- this slot's last frame is known to be off the GPU.
        m_pAllocators[m_frameIndex]->reset();

        // nullptr when every drawable is still in flight. Not an error -- but the pool still has
        // to drain and the slot has to go back, or a dropped frame leaks and the ring deadlocks.
        m_pDrawable = m_pLayer->nextDrawable();
        if (!m_pDrawable)
        {
            g_frameSemaphore.release();
            EndFramePool();
            return false;
        }

        // Sized from the drawable rather than the last OnResize, because Metal rejects a pass
        // whose attachments disagree -- and only one of the two is the texture being drawn to.
        MTL::Texture* pColorTexture = m_pDrawable->texture();
        if (!EnsureDepthTexture(static_cast<uint32>(pColorTexture->width()), static_cast<uint32>(pColorTexture->height())))
        {
            g_frameSemaphore.release();
            EndFramePool();
            return false;
        }

        m_pPass = MTL4::RenderPassDescriptor::alloc()->init()->autorelease();

        MTL::RenderPassColorAttachmentDescriptor* pColor = m_pPass->colorAttachments()->object(0);
        pColor->setTexture(pColorTexture);
        pColor->setLoadAction(MTL::LoadActionClear);    // On a tile-based GPU, Clear skips reading the previous framebuffer from DRAM.
        pColor->setStoreAction(MTL::StoreActionStore);
        pColor->setClearColor(MTL::ClearColor::Make(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));

        MTL::RenderPassDepthAttachmentDescriptor* pDepth = m_pPass->depthAttachment();
        pDepth->setTexture(m_pDepthTexture);
        pDepth->setLoadAction(MTL::LoadActionClear);
        pDepth->setClearDepth(1.0);
        pDepth->setStoreAction(MTL::StoreActionDontCare);   // Mandatory for a memoryless texture: there is nowhere to store it to.

        m_pCommandBuffer = m_pDevice->newCommandBuffer()->autorelease();
        m_pCommandBuffer->beginCommandBuffer(m_pAllocators[m_frameIndex]);

        // Must see the live descriptor: the backend takes its pipeline's attachment formats off
        // the attached textures, which is why UI cannot be built before the pass exists.
        ImGuiBeginFrame(m_pPass, static_cast<int>(m_frameIndex));
        return true;
    }

    void Metal4Renderer::EndFrame()
    {
        MTL4::RenderCommandEncoder* pEncoder = m_pCommandBuffer->renderCommandEncoder(m_pPass);

        if (m_indexCount > 0)
        {
            const uint32 uniformOffset = m_frameIndex * kUniformStride;

            Uniforms uniforms{};   // Value-initialised: the tail padding is memcpy'd to the GPU too.
            const Mat44 modelViewProjection = m_viewProjection * m_model;
            std::memcpy(&uniforms.modelViewProjection, &modelViewProjection, sizeof(modelViewProjection));
            std::memcpy(&uniforms.model, &m_model, sizeof(m_model));
            uniforms.time = m_time;
            std::memcpy(static_cast<std::byte*>(m_pUniforms->contents()) + uniformOffset, &uniforms, sizeof(uniforms));

            // Metal 4 has no setVertexBytes: everything the shader reads is a GPU address in the
            // argument table. The strided form is what feeds [[stage_in]] through the descriptor.
            m_pArgumentTable->setAddress(m_pVertices->gpuAddress(), sizeof(Vertex), 0);
            m_pArgumentTable->setAddress(m_pUniforms->gpuAddress() + uniformOffset, 1);

            // Both stages: the table is per-stage state, and the fragment shader reads the same
            // slot 1 for uniforms.time. A stage with no table bound reads an undefined address.
            pEncoder->setArgumentTable(m_pArgumentTable, MTL::RenderStageVertex | MTL::RenderStageFragment);
            pEncoder->setRenderPipelineState(m_pPipeline);
            pEncoder->setDepthStencilState(m_pDepthState);
            pEncoder->setCullMode(MTL::CullModeBack);
            pEncoder->setFrontFacingWinding(MTL::WindingClockwise);   // Y flips to framebuffer space, so CCW-authored faces land clockwise.
            pEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, m_indexCount, MTL::IndexTypeUInt32,
                                            m_pIndices->gpuAddress(), m_indexCount * sizeof(uint32));
        }

        // ImGui binds its own pipeline but never a depth state, so it would inherit the scene's
        // and write depth for 2D geometry. Sits on top of the scene by construction, not by luck.
        pEncoder->setDepthStencilState(m_pUIDepthState);
        pEncoder->setCullMode(MTL::CullModeNone);
        ImGuiEndFrame(m_pCommandBuffer, pEncoder);

        pEncoder->endEncoding();
        m_pCommandBuffer->endCommandBuffer();

        // Presentation is explicit in Metal 4: wait for the drawable to be writable, submit, then
        // signal that the GPU is finished with it. Classic Metal folded all three into
        // presentDrawable, which is the swapchain-semaphore pair Vulkan makes you write out.
        m_pQueue->wait(m_pDrawable);

        MTL4::CommitOptions* pOptions = MTL4::CommitOptions::alloc()->init()->autorelease();
        pOptions->addFeedbackHandler([](MTL4::CommitFeedback*)
        {
            g_frameSemaphore.release();
        });

        m_pQueue->commit(&m_pCommandBuffer, 1, pOptions);
        m_pQueue->signalDrawable(m_pDrawable);
        m_pDrawable->present();

        EndFramePool();
    }

    void Metal4Renderer::EndFramePool()
    {
        m_pPass          = nullptr;     // Autoreleased; the pool below owns them.
        m_pDrawable      = nullptr;
        m_pCommandBuffer = nullptr;

        Release(m_pFramePool);          // Draining the pool is what releases everything above.
    }

    void Metal4Renderer::Terminate()
    {
        ImGuiTerminate();

        m_pLayer = nullptr;

        for (uint32 i = 0; i < kFramesInFlight; ++i)
        {
            Release(m_pAllocators[i]);
        }

        Release(m_pResidencySet);
        Release(m_pArgumentTable);
        Release(m_pUniforms);
        Release(m_pIndices);
        Release(m_pVertices);
        Release(m_pDepthTexture);
        Release(m_pUIDepthState);
        Release(m_pDepthState);
        Release(m_pPipeline);
        Release(m_pCompiler);
        Release(m_pQueue);
        Release(m_pDevice);
    }

    void Metal4Renderer::OnResize(uint32 pixelWidth, uint32 pixelHeight)
    {
        if (!m_pLayer || pixelWidth == 0 || pixelHeight == 0)
        {
            return;
        }

        m_pLayer->setDrawableSize(CGSizeMake(pixelWidth, pixelHeight));
    }

    bool Metal4Renderer::SetMesh(const Mesh& mesh)
    {
        if (mesh.vertices.empty() || mesh.indices.empty())
        {
            return false;
        }

        Release(m_pVertices);
        Release(m_pIndices);

        m_pVertices  = m_pDevice->newBuffer(mesh.vertices.data(), mesh.vertices.size() * sizeof(Vertex), MTL::ResourceStorageModeShared);
        m_pIndices   = m_pDevice->newBuffer(mesh.indices.data(),  mesh.indices.size()  * sizeof(uint32), MTL::ResourceStorageModeShared);
        m_indexCount = static_cast<uint32>(mesh.indices.size());

        UpdateResidency();
        return m_pVertices && m_pIndices;
    }

    void Metal4Renderer::UpdateResidency()
    {
        if (!m_pResidencySet)
        {
            return;
        }

        // Rebuilt wholesale rather than diffed: the set is three buffers, and tracking which one
        // changed costs more code than re-adding all of them.
        m_pResidencySet->removeAllAllocations();

        if (m_pVertices) { m_pResidencySet->addAllocation(m_pVertices); }
        if (m_pIndices)  { m_pResidencySet->addAllocation(m_pIndices); }
        if (m_pUniforms) { m_pResidencySet->addAllocation(m_pUniforms); }

        // The depth texture is deliberately absent. Residency covers what the GPU reaches by raw
        // address; an attachment is bound by the pass descriptor instead -- and a memoryless one
        // has no memory to make resident at all, which the validation layer asserts on.
        m_pResidencySet->commit();
        m_pResidencySet->requestResidency();
    }

    bool Metal4Renderer::CreatePipeline()
    {
        NS::Error* pError = nullptr;

        MTL4::CompilerDescriptor* pCompilerDesc = MTL4::CompilerDescriptor::alloc()->init();
        m_pCompiler = m_pDevice->newCompiler(pCompilerDesc, &pError);
        pCompilerDesc->release();

        if (!m_pCompiler)
        {
            Debug::Error("Metal 4 compiler: {}", pError->localizedDescription()->utf8String());
            return false;
        }

        MTL::Library* pLibrary = m_pDevice->newDefaultLibrary();
        if (!pLibrary)
        {
            Debug::Error("No default.metallib in the bundle.");
            return false;
        }

        // Functions reach the pipeline by descriptor now, rather than as MTL::Function objects.
        MTL4::LibraryFunctionDescriptor* pVertexFn = MTL4::LibraryFunctionDescriptor::alloc()->init()->autorelease();
        pVertexFn->setLibrary(pLibrary);
        pVertexFn->setName(NS::String::string("MeshVertex", NS::UTF8StringEncoding));

        MTL4::LibraryFunctionDescriptor* pFragmentFn = MTL4::LibraryFunctionDescriptor::alloc()->init()->autorelease();
        pFragmentFn->setLibrary(pLibrary);
        pFragmentFn->setName(NS::String::string("MeshFragment", NS::UTF8StringEncoding));

        // Fixed-function vertex fetch survives Metal 4 unchanged, and is still the direct analogue
        // of Vulkan's VkPipelineVertexInputStateCreateInfo.
        MTL::VertexDescriptor* pVertexDesc = MTL::VertexDescriptor::vertexDescriptor();

        pVertexDesc->attributes()->object(0)->setFormat(MTL::VertexFormatFloat3);
        pVertexDesc->attributes()->object(0)->setOffset(offsetof(Vertex, position));
        pVertexDesc->attributes()->object(0)->setBufferIndex(0);

        pVertexDesc->attributes()->object(1)->setFormat(MTL::VertexFormatFloat3);
        pVertexDesc->attributes()->object(1)->setOffset(offsetof(Vertex, normal));
        pVertexDesc->attributes()->object(1)->setBufferIndex(0);

        pVertexDesc->attributes()->object(2)->setFormat(MTL::VertexFormatFloat2);
        pVertexDesc->attributes()->object(2)->setOffset(offsetof(Vertex, uv));
        pVertexDesc->attributes()->object(2)->setBufferIndex(0);

        // Float4 rather than a packed UChar4Normalized: LinearColor is already four floats, so
        // this costs no conversion on either side, and 16 bytes a vertex is not yet worth code.
        pVertexDesc->attributes()->object(3)->setFormat(MTL::VertexFormatFloat4);
        pVertexDesc->attributes()->object(3)->setOffset(offsetof(Vertex, color));
        pVertexDesc->attributes()->object(3)->setBufferIndex(0);

        pVertexDesc->layouts()->object(0)->setStride(sizeof(Vertex));

        MTL4::RenderPipelineDescriptor* pDesc = MTL4::RenderPipelineDescriptor::alloc()->init()->autorelease();
        pDesc->setVertexFunctionDescriptor(pVertexFn);
        pDesc->setFragmentFunctionDescriptor(pFragmentFn);
        pDesc->setVertexDescriptor(pVertexDesc);
        pDesc->colorAttachments()->object(0)->setPixelFormat(m_pLayer->pixelFormat());

        // No depth attachment format here, unlike classic Metal: Metal 4 decouples it from the
        // pipeline and takes it from the render pass at encode time.
        m_pPipeline = m_pCompiler->newRenderPipelineState(pDesc, nullptr, &pError);
        pLibrary->release();

        if (!m_pPipeline)
        {
            Debug::Error("Pipeline: {}", pError->localizedDescription()->utf8String());
            return false;
        }

        MTL4::ArgumentTableDescriptor* pTableDesc = MTL4::ArgumentTableDescriptor::alloc()->init();
        pTableDesc->setMaxBufferBindCount(2);           // Vertices at 0, uniforms at 1.
        pTableDesc->setSupportAttributeStrides(true);   // Required for the strided binding [[stage_in]] reads.

        m_pArgumentTable = m_pDevice->newArgumentTable(pTableDesc, &pError);
        pTableDesc->release();

        if (!m_pArgumentTable)
        {
            Debug::Error("Argument table: {}", pError->localizedDescription()->utf8String());
            return false;
        }

        MTL::DepthStencilDescriptor* pDepthDesc = MTL::DepthStencilDescriptor::alloc()->init()->autorelease();
        pDepthDesc->setDepthCompareFunction(MTL::CompareFunctionLess);  // Perspective maps near to 0, far to 1. Metal's range, not OpenGL's -1..1.
        pDepthDesc->setDepthWriteEnabled(true);

        m_pDepthState = m_pDevice->newDepthStencilState(pDepthDesc);

        // Always + no write: UI is composited over the finished scene and has no depth of its own.
        MTL::DepthStencilDescriptor* pUIDepthDesc = MTL::DepthStencilDescriptor::alloc()->init()->autorelease();
        pUIDepthDesc->setDepthCompareFunction(MTL::CompareFunctionAlways);
        pUIDepthDesc->setDepthWriteEnabled(false);

        m_pUIDepthState = m_pDevice->newDepthStencilState(pUIDepthDesc);
        return m_pDepthState && m_pUIDepthState;
    }

    bool Metal4Renderer::EnsureDepthTexture(uint32 pixelWidth, uint32 pixelHeight)
    {
        if (m_pDepthTexture && m_pDepthTexture->width() == pixelWidth && m_pDepthTexture->height() == pixelHeight)
        {
            return true;
        }

        Release(m_pDepthTexture);

        MTL::TextureDescriptor* pDesc = MTL::TextureDescriptor::texture2DDescriptor(kDepthFormat, pixelWidth, pixelHeight, false);
        pDesc->setUsage(MTL::TextureUsageRenderTarget);

        // Memoryless keeps the buffer in tile memory and never backs it with DRAM, which is
        // free here because depth is produced and consumed inside the one pass. Apple-family
        // GPUs only -- an Intel Mac has no tile memory to put it in.
        pDesc->setStorageMode(m_pDevice->supportsFamily(MTL::GPUFamilyApple1) ? MTL::StorageModeMemoryless : MTL::StorageModePrivate);

        m_pDepthTexture = m_pDevice->newTexture(pDesc);
        return m_pDepthTexture;
    }
}

#endif // IS_PLATFORM_MACOS || IS_PLATFORM_IOS
