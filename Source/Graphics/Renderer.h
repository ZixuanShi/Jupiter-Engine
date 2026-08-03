// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
    #include "Metal/MetalRenderer.h"
#else
    #error "No Renderer backend for this platform"
#endif

import jpt.LinearColor;
import jpt.TypeDefs;
import std;

namespace jpt
{
    /** The contract every backend satisfies. Compile-time polymorphism */
    template<typename T>
    concept RendererType = requires(T renderer, typename T::SurfaceHandle surface,
                                    uint32 width, uint32 height,
                                    const LinearColor& color)
    {
        { renderer.PreInit() }               -> std::same_as<bool>;
        { renderer.Init(surface) }           -> std::same_as<bool>;
        { renderer.Terminate() }             -> std::same_as<void>;

        { renderer.OnResize(width, height) } -> std::same_as<void>;
        { renderer.OnFrame() }               -> std::same_as<void>;
        { renderer.SetClearColor(color) }    -> std::same_as<void>;
    };

    using Renderer = MetalRenderer;

    static_assert(RendererType<Renderer>);
    static_assert(!std::is_polymorphic_v<Renderer>, "Renderer must stay vtable-free");
}
