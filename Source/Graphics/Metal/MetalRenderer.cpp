// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "MetalRenderer.h"

#include <cmath>

namespace jpt
{
    bool MetalRenderer::Init(void* pMetalLayer)
    {
        m_pLayer = reinterpret_cast<CA::MetalLayer*>(pMetalLayer);

        m_pDevice = MTL::CreateSystemDefaultDevice();
        if (m_pDevice == nullptr)
        {
            return false;
        }

        m_pQueue = m_pDevice->newCommandQueue();

        m_pLayer->setDevice(m_pDevice);
        m_pLayer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

        // Promises render-only access, letting Core Animation pick faster memory.
        m_pLayer->setFramebufferOnly(true);

        return m_pQueue != nullptr;
    }

    void MetalRenderer::OnResize(uint32 pixelWidth, uint32 pixelHeight)
    {
        if (m_pLayer == nullptr || pixelWidth == 0 || pixelHeight == 0)
        {
            return;
        }
        m_pLayer->setDrawableSize(CGSizeMake(pixelWidth, pixelHeight));
    }

    void MetalRenderer::OnFrameDraw(float64 elapsedSeconds)
    {
        if (m_pLayer == nullptr || m_pQueue == nullptr)
        {
            return;
        }

        // Drawables are autoreleased. Without a pool draining each frame the finite pool
        // exhausts and the app stalls within seconds.
        NS::SharedPtr<NS::AutoreleasePool> pool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

        // nullptr when every drawable is still in flight. Not an error.
        CA::MetalDrawable* pDrawable = m_pLayer->nextDrawable();
        if (pDrawable == nullptr)
        {
            return;
        }

        const float64 r = 0.5 + 0.5 * std::sin(elapsedSeconds);
        const float64 g = 0.5 + 0.5 * std::sin(elapsedSeconds + 2.0943951);
        const float64 b = 0.5 + 0.5 * std::sin(elapsedSeconds + 4.1887902);

        MTL::RenderPassDescriptor* pPass = MTL::RenderPassDescriptor::renderPassDescriptor();
        MTL::RenderPassColorAttachmentDescriptor* pColor = pPass->colorAttachments()->object(0);

        pColor->setTexture(pDrawable->texture());

        // On a tile-based GPU, Clear skips reading the previous framebuffer from DRAM.
        pColor->setLoadAction(MTL::LoadActionClear);
        pColor->setStoreAction(MTL::StoreActionStore);
        pColor->setClearColor(MTL::ClearColor::Make(r, g, b, 1.0));

        MTL::CommandBuffer* pCommandBuffer = m_pQueue->commandBuffer();

        // The clear is pass setup, not a command, so an empty encoder is enough.
        MTL::RenderCommandEncoder* pEncoder = pCommandBuffer->renderCommandEncoder(pPass);
        pEncoder->endEncoding();

        pCommandBuffer->presentDrawable(pDrawable);
        pCommandBuffer->commit();
    }

    void MetalRenderer::Terminate()
    {
        m_pLayer = nullptr;

        // Init() took ownership of both, and there is no SharedPtr to do it here.
        if (m_pQueue != nullptr)
        {
            m_pQueue->release();
            m_pQueue = nullptr;
        }
        if (m_pDevice != nullptr)
        {
            m_pDevice->release();
            m_pDevice = nullptr;
        }
    }
}

#endif // IS_PLATFORM_MACOS || IS_PLATFORM_IOS
