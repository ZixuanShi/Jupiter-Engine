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

namespace
{
    NS::SharedPtr<MTL::Device>       g_device;
    NS::SharedPtr<MTL::CommandQueue> g_queue;
    CA::MetalLayer*                  g_layer = nullptr;
}

namespace jpt
{
    bool InitRenderer(void* metalLayer)
    {
        g_layer = reinterpret_cast<CA::MetalLayer*>(metalLayer);

        g_device = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
        if (!g_device)
        {
            return false;
        }

        g_queue = NS::TransferPtr(g_device->newCommandQueue());

        g_layer->setDevice(g_device.get());
        g_layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

        // Promises render-only access, letting Core Animation pick faster memory.
        g_layer->setFramebufferOnly(true);

        return static_cast<bool>(g_queue);
    }

    void ResizeRenderer(std::uint32_t pixelWidth, std::uint32_t pixelHeight)
    {
        if (g_layer == nullptr || pixelWidth == 0 || pixelHeight == 0)
        {
            return;
        }
        g_layer->setDrawableSize(CGSizeMake(pixelWidth, pixelHeight));
    }

    void DrawFrame(double deltaSeconds)
    {
        if (g_layer == nullptr || !g_queue)
        {
            return;
        }

        // Drawables are autoreleased. Without a pool draining each frame the finite pool
        // exhausts and the app stalls within seconds.
        NS::SharedPtr<NS::AutoreleasePool> pool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

        // nullptr when every drawable is still in flight. Not an error.
        CA::MetalDrawable* drawable = g_layer->nextDrawable();
        if (drawable == nullptr)
        {
            return;
        }

        const double r = 0.5 + 0.5 * std::sin(deltaSeconds);
        const double g = 0.5 + 0.5 * std::sin(deltaSeconds + 2.0943951);
        const double b = 0.5 + 0.5 * std::sin(deltaSeconds + 4.1887902);

        MTL::RenderPassDescriptor* pass = MTL::RenderPassDescriptor::renderPassDescriptor();
        MTL::RenderPassColorAttachmentDescriptor* color = pass->colorAttachments()->object(0);

        color->setTexture(drawable->texture());

        // On a tile-based GPU, Clear skips reading the previous framebuffer from DRAM.
        color->setLoadAction(MTL::LoadActionClear);
        color->setStoreAction(MTL::StoreActionStore);
        color->setClearColor(MTL::ClearColor::Make(r, g, b, 1.0));

        MTL::CommandBuffer* commandBuffer = g_queue->commandBuffer();

        // The clear is pass setup, not a command, so an empty encoder is enough.
        MTL::RenderCommandEncoder* encoder = commandBuffer->renderCommandEncoder(pass);
        encoder->endEncoding();

        commandBuffer->presentDrawable(drawable);
        commandBuffer->commit();
    }

    void ShutdownRenderer()
    {
        g_layer = nullptr;
        g_queue.reset();
        g_device.reset();
    }
}

#endif // IS_PLATFORM_MACOS || IS_PLATFORM_IOS
