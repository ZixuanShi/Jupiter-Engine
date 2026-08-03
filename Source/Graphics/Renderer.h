// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

// Backend headers first: they include libc++ textually, and a translation unit must see the
// textual copies before the module imports below.
#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
    #include "Metal/MetalRenderer.h"
#else
    #error "No Renderer backend for this platform"
#endif

import jpt.TypeDefs;
import std;

namespace jpt
{
    /** The contract every backend satisfies. Compile-time polymorphism: no base class and no
        vtable, just a shape the platform's Renderer alias is checked against. */
    template<typename T>
    concept RendererType = requires(T renderer, typename T::SurfaceHandle surface,
                                    uint32 width, uint32 height, float64 seconds)
    {
        { renderer.PreInit() }               -> std::same_as<bool>;
        { renderer.Init(surface) }           -> std::same_as<bool>;
        { renderer.Terminate() }             -> std::same_as<void>;

        { renderer.OnResize(width, height) } -> std::same_as<void>;
        { renderer.OnFrameDraw(seconds) }    -> std::same_as<void>;
    };

    using Renderer = MetalRenderer;

    static_assert(RendererType<Renderer>);
}
