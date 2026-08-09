// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <cstdint>

#include "Applications/Window/WindowBase.h"

namespace jpt
{
    /** A window that never opens, so a platform with no backend yet still links and runs.

        Run() drives the frame loop a fixed number of times and returns, which is the desktop
        shape -- the loop belongs to the backend -- with the event source removed. Nothing calls
        OnSurfaceReady, because there is no surface: the renderer stays uninitialised and
        BeginFrame() declines every frame.

        Replaced by WindowSDL3, not extended. */
    class WindowNull : public WindowBase
    {
    public:
        static constexpr std::uint32_t kFrames = 3;

        // What a desktop window opens at. Only so GetAspectRatio() answers something a camera
        // can use before any real surface exists.
        static constexpr std::uint32_t kWidth  = 1280;
        static constexpr std::uint32_t kHeight = 720;

    public:
        bool PreInit(std::int32_t argc, char* ppArgv[]);
        bool Init();
        void Run();
        void Terminate();

        /** Hides WindowBase::SetCursorCaptured, whose assert fires on a backend that has no
            pointer to capture. There is no pointer here either, but there is also no bug. */
        void SetCursorCaptured(bool captured);
    };
}
