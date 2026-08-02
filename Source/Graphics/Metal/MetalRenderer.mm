// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#include "MetalRenderer.h"

#include <cmath>

namespace
{
    id<MTLDevice>       g_device = nil;
    id<MTLCommandQueue> g_queue  = nil;
    CAMetalLayer*       g_layer  = nil;
}

namespace jpt
{
    bool InitRenderer(void* metalLayer)
    {
        // __bridge: reinterpret without transferring ownership. The view owns the layer.
        g_layer = (__bridge CAMetalLayer*)metalLayer;

        g_device = MTLCreateSystemDefaultDevice();
        if (g_device == nil)
        {
            return false;
        }

        g_queue = [g_device newCommandQueue];

        g_layer.device = g_device;
        g_layer.pixelFormat = MTLPixelFormatBGRA8Unorm;

        // Promises render-only access, letting Core Animation pick faster memory.
        g_layer.framebufferOnly = YES;

        return g_queue != nil;
    }

    void ResizeRenderer(std::uint32_t pixelWidth, std::uint32_t pixelHeight)
    {
        if (g_layer == nil || pixelWidth == 0 || pixelHeight == 0)
        {
            return;
        }
        g_layer.drawableSize = CGSizeMake(pixelWidth, pixelHeight);
    }

    void DrawFrame(double elapsedSeconds)
    {
        if (g_layer == nil || g_queue == nil)
        {
            return;
        }

        // Drawables are autoreleased. Without a pool draining each frame the finite pool
        // exhausts and the app stalls within seconds.
        @autoreleasepool
        {
            // nil when every drawable is still in flight. Not an error.
            id<CAMetalDrawable> drawable = [g_layer nextDrawable];
            if (drawable == nil)
            {
                return;
            }

            const double r = 0.5 + 0.5 * std::sin(elapsedSeconds);
            const double g = 0.5 + 0.5 * std::sin(elapsedSeconds + 2.0943951);
            const double b = 0.5 + 0.5 * std::sin(elapsedSeconds + 4.1887902);

            MTLRenderPassDescriptor* pass = [MTLRenderPassDescriptor renderPassDescriptor];
            pass.colorAttachments[0].texture = drawable.texture;

            // On a tile-based GPU, Clear skips reading the previous framebuffer from DRAM.
            pass.colorAttachments[0].loadAction  = MTLLoadActionClear;
            pass.colorAttachments[0].storeAction = MTLStoreActionStore;
            pass.colorAttachments[0].clearColor  = MTLClearColorMake(r, g, b, 1.0);

            id<MTLCommandBuffer> commandBuffer = [g_queue commandBuffer];

            // The clear is pass setup, not a command, so an empty encoder is enough.
            id<MTLRenderCommandEncoder> encoder =
                [commandBuffer renderCommandEncoderWithDescriptor:pass];
            [encoder endEncoding];

            [commandBuffer presentDrawable:drawable];
            [commandBuffer commit];
        }
    }

    void ShutdownRenderer()
    {
        g_layer  = nil;
        g_queue  = nil;
        g_device = nil;
    }
}

#endif // IS_PLATFORM_MACOS || IS_PLATFORM_IOS
