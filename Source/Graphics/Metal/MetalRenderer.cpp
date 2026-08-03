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

#include "Graphics/Shader/ShaderTypes.h"
#include "MetalRenderer.h"

import jpt.Logger;
import jpt.Matrix44;
import jpt.Vertex;

// What lets the matrix cross to the GPU by memcpy: both are 16 column-major floats.
static_assert(sizeof(jpt::Mat44) == sizeof(simd_float4x4));

namespace jpt
{
    constexpr MTL::PixelFormat kDepthFormat = MTL::PixelFormatDepth32Float;

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
    }

    bool MetalRenderer::PreInit()
    {
        m_pDevice = MTL::CreateSystemDefaultDevice();
        if (!m_pDevice)
        {
            return false;
        }

        m_pQueue = m_pDevice->newCommandQueue();
        return m_pQueue;
    }

    bool MetalRenderer::Init(SurfaceHandle pMetalLayer)
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
        return CreatePipeline();
    }

    bool MetalRenderer::CreatePipeline()
    {
        MTL::Library* pLibrary = m_pDevice->newDefaultLibrary();
        if (!pLibrary)
        {
            Debug::Error("No default.metallib in the bundle.");
            return false;
        }

        MTL::Function* pVertexFn   = pLibrary->newFunction(NS::String::string("MeshVertex", NS::UTF8StringEncoding));
        MTL::Function* pFragmentFn = pLibrary->newFunction(NS::String::string("MeshFragment", NS::UTF8StringEncoding));
        pLibrary->release();

        if (!pVertexFn || !pFragmentFn)
        {
            Debug::Error("Shader entry point missing from default.metallib.");
            return false;
        }

        // Fixed-function vertex fetch, the direct analogue of Vulkan's
        // VkPipelineVertexInputStateCreateInfo -- so this survives into the RHI unchanged.
        MTL::VertexDescriptor* pVertexDesc = MTL::VertexDescriptor::vertexDescriptor();

        pVertexDesc->attributes()->object(0)->setFormat(MTL::VertexFormatFloat3);
        pVertexDesc->attributes()->object(0)->setOffset(offsetof(Vertex, position));
        pVertexDesc->attributes()->object(0)->setBufferIndex(0);

        pVertexDesc->attributes()->object(1)->setFormat(MTL::VertexFormatFloat3);
        pVertexDesc->attributes()->object(1)->setOffset(offsetof(Vertex, normal));
        pVertexDesc->attributes()->object(1)->setBufferIndex(0);

        pVertexDesc->layouts()->object(0)->setStride(sizeof(Vertex));

        MTL::RenderPipelineDescriptor* pDesc = MTL::RenderPipelineDescriptor::alloc()->init()->autorelease();
        pDesc->setVertexFunction(pVertexFn);
        pDesc->setFragmentFunction(pFragmentFn);
        pDesc->setVertexDescriptor(pVertexDesc);
        pDesc->colorAttachments()->object(0)->setPixelFormat(m_pLayer->pixelFormat());
        pDesc->setDepthAttachmentPixelFormat(kDepthFormat);

        NS::Error* pError = nullptr;
        m_pPipeline = m_pDevice->newRenderPipelineState(pDesc, &pError);

        pVertexFn->release();
        pFragmentFn->release();

        if (!m_pPipeline)
        {
            Debug::Error("Pipeline: {}", pError->localizedDescription()->utf8String());
            return false;
        }

        MTL::DepthStencilDescriptor* pDepthDesc = MTL::DepthStencilDescriptor::alloc()->init()->autorelease();
        pDepthDesc->setDepthCompareFunction(MTL::CompareFunctionLess);  // Perspective maps near to 0, far to 1. Metal's range, not OpenGL's -1..1.
        pDepthDesc->setDepthWriteEnabled(true);

        m_pDepthState = m_pDevice->newDepthStencilState(pDepthDesc);
        return m_pDepthState;
    }

    bool MetalRenderer::EnsureDepthTexture(uint32 pixelWidth, uint32 pixelHeight)
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

    bool MetalRenderer::SetMesh(const Mesh& mesh)
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

        return m_pVertices && m_pIndices;
    }

    void MetalRenderer::OnResize(uint32 pixelWidth, uint32 pixelHeight)
    {
        if (!m_pLayer || pixelWidth == 0 || pixelHeight == 0)
        {
            return;
        }

        m_pLayer->setDrawableSize(CGSizeMake(pixelWidth, pixelHeight));
    }

    void MetalRenderer::Draw()
    {
        if (!m_pLayer || !m_pQueue || !m_pPipeline || m_indexCount == 0)
        {
            return;
        }

        // Drawables are autoreleased. Without a pool draining each frame the finite pool
        // exhausts and the app stalls within seconds.
        NS::SharedPtr<NS::AutoreleasePool> pool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

        // nullptr when every drawable is still in flight. Not an error.
        CA::MetalDrawable* pDrawable = m_pLayer->nextDrawable();
        if (!pDrawable)
        {
            return;
        }

        // Sized from the drawable rather than the last OnResize, because Metal rejects a pass
        // whose attachments disagree -- and only one of the two is the texture being drawn to.
        MTL::Texture* pColorTexture = pDrawable->texture();
        if (!EnsureDepthTexture(static_cast<uint32>(pColorTexture->width()), static_cast<uint32>(pColorTexture->height())))
        {
            return;
        }

        MTL::RenderPassDescriptor* pPass = MTL::RenderPassDescriptor::renderPassDescriptor();

        MTL::RenderPassColorAttachmentDescriptor* pColor = pPass->colorAttachments()->object(0);
        pColor->setTexture(pColorTexture);
        pColor->setLoadAction(MTL::LoadActionClear);    // On a tile-based GPU, Clear skips reading the previous framebuffer from DRAM.
        pColor->setStoreAction(MTL::StoreActionStore);
        pColor->setClearColor(MTL::ClearColor::Make(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));

        MTL::RenderPassDepthAttachmentDescriptor* pDepth = pPass->depthAttachment();
        pDepth->setTexture(m_pDepthTexture);
        pDepth->setLoadAction(MTL::LoadActionClear);
        pDepth->setClearDepth(1.0);
        pDepth->setStoreAction(MTL::StoreActionDontCare);   // Mandatory for a memoryless texture: there is nowhere to store it to.

        MTL::CommandBuffer* pCommandBuffer = m_pQueue->commandBuffer();
        MTL::RenderCommandEncoder* pEncoder = pCommandBuffer->renderCommandEncoder(pPass);

        Uniforms uniforms;
        const Mat44 modelViewProjection = m_viewProjection * m_model;
        std::memcpy(&uniforms.modelViewProjection, &modelViewProjection, sizeof(modelViewProjection));
        std::memcpy(&uniforms.model, &m_model, sizeof(m_model));

        pEncoder->setRenderPipelineState(m_pPipeline);
        pEncoder->setDepthStencilState(m_pDepthState);
        pEncoder->setCullMode(MTL::CullModeBack);
        pEncoder->setFrontFacingWinding(MTL::WindingClockwise);   // Y flips to framebuffer space, so CCW-authored faces land clockwise.
        pEncoder->setVertexBuffer(m_pVertices, 0, 0);
        pEncoder->setVertexBytes(&uniforms, sizeof(uniforms), 1); // Fast path for small per-draw constants. Vulkan's push constants.
        pEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, m_indexCount, MTL::IndexTypeUInt32, m_pIndices, NS::UInteger(0));
        pEncoder->endEncoding();

        pCommandBuffer->presentDrawable(pDrawable);
        pCommandBuffer->commit();
    }

    void MetalRenderer::Terminate()
    {
        m_pLayer = nullptr;

        Release(m_pIndices);
        Release(m_pVertices);
        Release(m_pDepthTexture);
        Release(m_pDepthState);
        Release(m_pPipeline);
        Release(m_pQueue);
        Release(m_pDevice);
    }
}

#endif // IS_PLATFORM_MACOS || IS_PLATFORM_IOS
