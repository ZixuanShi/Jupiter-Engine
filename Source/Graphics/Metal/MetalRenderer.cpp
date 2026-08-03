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

#include "Graphics/ShaderTypes.h"
#include "MetalRenderer.h"

import jpt.Logger;
import jpt.Matrix44;
import jpt.Vector3;
import jpt.Vertex;

// What lets the matrix cross to the GPU by memcpy: both are 16 column-major floats.
static_assert(sizeof(jpt::Mat44) == sizeof(simd_float4x4));

namespace jpt
{
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
        return CreatePipeline() && CreateGeometry();
    }

    bool MetalRenderer::CreatePipeline()
    {
        MTL::Library* pLibrary = m_pDevice->newDefaultLibrary();
        if (!pLibrary)
        {
            Debug::Error("No default.metallib in the bundle.");
            return false;
        }

        MTL::Function* pVertexFn   = pLibrary->newFunction(NS::String::string("TriangleVertex", NS::UTF8StringEncoding));
        MTL::Function* pFragmentFn = pLibrary->newFunction(NS::String::string("TriangleFragment", NS::UTF8StringEncoding));
        pLibrary->release();

        if (!pVertexFn || !pFragmentFn)
        {
            Debug::Error("Shader entry point missing from default.metallib.");
            return false;
        }

        // Fixed-function vertex fetch, the direct analogue of Vulkan's
        // VkPipelineVertexInputStateCreateInfo -- so this survives into the RHI unchanged.
        MTL::VertexDescriptor* pVertexDesc = MTL::VertexDescriptor::vertexDescriptor();

        pVertexDesc->attributes()->object(0)->setFormat(MTL::VertexFormatFloat2);
        pVertexDesc->attributes()->object(0)->setOffset(offsetof(Vertex, position));
        pVertexDesc->attributes()->object(0)->setBufferIndex(0);

        pVertexDesc->attributes()->object(1)->setFormat(MTL::VertexFormatFloat4);
        pVertexDesc->attributes()->object(1)->setOffset(offsetof(Vertex, color));
        pVertexDesc->attributes()->object(1)->setBufferIndex(0);

        pVertexDesc->layouts()->object(0)->setStride(sizeof(Vertex));

        MTL::RenderPipelineDescriptor* pDesc = MTL::RenderPipelineDescriptor::alloc()->init()->autorelease();
        pDesc->setVertexFunction(pVertexFn);
        pDesc->setFragmentFunction(pFragmentFn);
        pDesc->setVertexDescriptor(pVertexDesc);
        pDesc->colorAttachments()->object(0)->setPixelFormat(m_pLayer->pixelFormat());

        NS::Error* pError = nullptr;
        m_pPipeline = m_pDevice->newRenderPipelineState(pDesc, &pError);

        pVertexFn->release();
        pFragmentFn->release();

        if (!m_pPipeline)
        {
            Debug::Error("Pipeline: {}", pError->localizedDescription()->utf8String());
            return false;
        }

        return true;
    }

    bool MetalRenderer::CreateGeometry()
    {
        const Vertex vertices[] =
        {
            { {  0.0f,  0.5f }, { 1.0f, 0.0f, 0.0f } },
            { { -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
            { {  0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f } },
        };

        m_pVertices = m_pDevice->newBuffer(vertices, sizeof(vertices), MTL::ResourceStorageModeShared);
        return m_pVertices;
    }

    void MetalRenderer::OnResize(uint32 pixelWidth, uint32 pixelHeight)
    {
        if (!m_pLayer || pixelWidth == 0 || pixelHeight == 0)
        {
            return;
        }

        m_pLayer->setDrawableSize(CGSizeMake(pixelWidth, pixelHeight));
    }

    void MetalRenderer::OnFrame()
    {
        if (!m_pLayer || !m_pQueue || !m_pPipeline)
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

        MTL::RenderPassDescriptor* pPass = MTL::RenderPassDescriptor::renderPassDescriptor();
        MTL::RenderPassColorAttachmentDescriptor* pColor = pPass->colorAttachments()->object(0);

        pColor->setTexture(pDrawable->texture());
        pColor->setLoadAction(MTL::LoadActionClear);    // On a tile-based GPU, Clear skips reading the previous framebuffer from DRAM.
        pColor->setStoreAction(MTL::StoreActionStore);
        pColor->setClearColor(MTL::ClearColor::Make(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));

        MTL::CommandBuffer* pCommandBuffer = m_pQueue->commandBuffer();
        MTL::RenderCommandEncoder* pEncoder = pCommandBuffer->renderCommandEncoder(pPass);

        const CGSize drawable = m_pLayer->drawableSize();
        const float32 aspect  = (drawable.height > 0.0) ? static_cast<float32>(drawable.width) / static_cast<float32>(drawable.height) : 1.0f;

        Uniforms uniforms;
        const Mat44 aspectCorrection = Mat44::Scale(Vec3(1.0f / aspect, 1.0f, 1.0f));
        std::memcpy(&uniforms.modelViewProjection, &aspectCorrection, sizeof(aspectCorrection));

        pEncoder->setRenderPipelineState(m_pPipeline);
        pEncoder->setVertexBuffer(m_pVertices, 0, 0);
        pEncoder->setVertexBytes(&uniforms, sizeof(uniforms), 1);

        pEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));
        pEncoder->endEncoding();

        pCommandBuffer->presentDrawable(pDrawable);
        pCommandBuffer->commit();
    }

    void MetalRenderer::Terminate()
    {
        m_pLayer = nullptr;

        if (m_pVertices)
        {
            m_pVertices->release();
            m_pVertices = nullptr;
        }
        if (m_pPipeline)
        {
            m_pPipeline->release();
            m_pPipeline = nullptr;
        }
        if (m_pQueue)
        {
            m_pQueue->release();
            m_pQueue = nullptr;
        }
        if (m_pDevice)
        {
            m_pDevice->release();
            m_pDevice = nullptr;
        }
    }
}

#endif // IS_PLATFORM_MACOS || IS_PLATFORM_IOS
