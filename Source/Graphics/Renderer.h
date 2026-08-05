// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
    #include "Metal/Metal4Renderer.h"
#else
    #error "No Renderer backend for this platform"
#endif

import jpt.LinearColor;
import jpt.Mesh;
import jpt.Texture;
import jpt.TypeDefs;
import std;

namespace jpt
{
    /** The contract every backend satisfies. Compile-time polymorphism */
    template<typename T>
    concept RendererType = requires(T renderer, typename T::SurfaceHandle surface,
                                    uint32 width, uint32 height,
                                    const LinearColor& color, const Mesh& mesh,
                                    std::span<const Texture> textures)
    {
        { renderer.PreInit() }                  -> std::same_as<bool>;
        { renderer.Init(surface) }              -> std::same_as<bool>;
        { renderer.Terminate() }                -> std::same_as<void>;

        { renderer.OnResize(width, height) }    -> std::same_as<void>;
        { renderer.RequestCapture() }           -> std::same_as<void>;

        // Two halves rather than one Draw(), so UI can be built while the pass is open. False
        // from BeginFrame means the frame was dropped and EndFrame must not follow.
        { renderer.BeginFrame() }               -> std::same_as<bool>;
        { renderer.EndFrame() }                 -> std::same_as<void>;

        // Pulls camera and clock off the application itself, so the contract is one call rather
        // than one setter per input.
        { renderer.Update() }                   -> std::same_as<void>;
        { renderer.SetMesh(mesh) }              -> std::same_as<bool>;
        { renderer.SetTextures(textures) }      -> std::same_as<bool>;
    };

    using Renderer = Metal4Renderer;

    static_assert(RendererType<Renderer>);
    static_assert(!std::is_polymorphic_v<Renderer>, "Renderer must stay vtable-free");
}
