// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

// Backend headers first: they include libc++ textually, and a translation unit must see the
// textual copies before the module imports below.
#if IS_PLATFORM_MACOS
    #include "Apple/WindowMac.h"
#elif IS_PLATFORM_IOS
    #include "Apple/WindowIOS.h"
#else
    // Windows and Linux, until WindowSDL3 lands. A backend is what a platform is missing, so the
    // seam answers with one rather than with an #error the whole engine cannot get past.
    #include "Null/WindowNull.h"
#endif

import jpt.TypeDefs;
import std;

namespace jpt
{
    /** The contract every window backend satisfies. Run() hands control to the platform:
        it blocks forever on Apple, where AppKit and UIKit own the loop, and is the poll loop
        itself on desktop backends such as GLFW. */
    template<typename T>
    concept WindowType = requires(T window, int32 argc, char** ppArgv,
                                  uint32 width, uint32 height)
    {
        { window.PreInit(argc, ppArgv) } -> std::same_as<bool>;
        { window.Init() }                -> std::same_as<bool>;
        { window.Run() }                 -> std::same_as<void>;
        { window.Terminate() }           -> std::same_as<void>;

        { window.OnResize(width, height) } -> std::same_as<void>;
        { window.SetCursorCaptured(true) } -> std::same_as<void>;
    };

#if IS_PLATFORM_MACOS
    using Window = WindowMac;
#elif IS_PLATFORM_IOS
    using Window = WindowIOS;
#else
    using Window = WindowNull;
#endif

    static_assert(WindowType<Window>);

    // A comment cannot stop someone adding `virtual` to WindowBase. This can.
    static_assert(!std::is_polymorphic_v<Window>, "Window must stay vtable-free");
}
