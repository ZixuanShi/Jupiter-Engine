// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/Window/Null/WindowNull.h"

#include "Applications/GetApp.h"

import jpt.Logger;

namespace jpt
{
    bool WindowNull::PreInit([[maybe_unused]] std::int32_t argc, [[maybe_unused]] char* ppArgv[])
    {
        return true;
    }

    bool WindowNull::Init()
    {
        OnResize(kWidth, kHeight);
        return true;
    }

    void WindowNull::Run()
    {
        Debug::Info("Null window: no surface. Running {} frames, then exiting.", kFrames);

        for (std::uint32_t frame = 0; frame < kFrames; ++frame)
        {
            GetApp().OnFrame();
        }
    }

    void WindowNull::Terminate()
    {
    }

    void WindowNull::SetCursorCaptured([[maybe_unused]] bool captured)
    {
    }
}
