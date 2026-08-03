// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
    #include "Metal/MetalRenderer.h"
#else
    #error "No Renderer backend for this platform"
#endif

import jpt.LinearColor;
import jpt.Matrix44;
import jpt.Mesh;
import jpt.TypeDefs;
import std;

namespace jpt
{
    /** The contract every backend satisfies. Compile-time polymorphism */
    template<typename T>
    concept RendererType = requires(T renderer, typename T::SurfaceHandle surface,
                                    uint32 width, uint32 height,
                                    const LinearColor& color, const Mesh& mesh, const Mat44& matrix)
    {
        { renderer.PreInit() }                  -> std::same_as<bool>;
        { renderer.Init(surface) }              -> std::same_as<bool>;
        { renderer.Terminate() }                -> std::same_as<void>;

        { renderer.OnResize(width, height) }    -> std::same_as<void>;

        // Two halves rather than one Draw(), so UI can be built while the pass is open. False
        // from BeginFrame means the frame was dropped and EndFrame must not follow.
        { renderer.BeginFrame() }               -> std::same_as<bool>;
        { renderer.EndFrame() }                 -> std::same_as<void>;

        { renderer.SetMesh(mesh) }              -> std::same_as<bool>;
        { renderer.SetClearColor(color) }       -> std::same_as<void>;
        { renderer.SetModel(matrix) }           -> std::same_as<void>;
        { renderer.SetViewProjection(matrix) }  -> std::same_as<void>;
    };

    using Renderer = MetalRenderer;

    static_assert(RendererType<Renderer>);
    static_assert(!std::is_polymorphic_v<Renderer>, "Renderer must stay vtable-free");
}
