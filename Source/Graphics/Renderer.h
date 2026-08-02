// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.TypeDefs;
import std;

namespace jpt
{
    /** The contract every backend satisfies. Compile-time polymorphism: no base class and no
        vtable, just a shape the platform's Renderer alias is checked against. */
    template<typename T>
    concept RendererType = requires(T renderer, void* pSurface,
                                    uint32 width, uint32 height, float64 seconds)
    {
        { renderer.Init(pSurface) }          -> std::same_as<bool>;
        { renderer.OnResize(width, height) } -> std::same_as<void>;
        { renderer.OnFrameDraw(seconds) }    -> std::same_as<void>;
        { renderer.Terminate() }             -> std::same_as<void>;
    };
}

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
    #include "Metal/MetalRenderer.h"

    namespace jpt
    {
        using Renderer = MetalRenderer;
    }
#endif

static_assert(jpt::RendererType<jpt::Renderer>);
