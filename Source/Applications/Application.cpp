// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
    #include "Window/Apple/AppleWindow.h"
#endif

module jpt.Application;

import jpt.Logger;
import jpt.Utils;

namespace jpt
{
    bool Application::PreInit()
    {
        jpt::Debug("Jupiter Engine from {}-{}", jpt::GetPlatformName(), jpt::GetConfigName());
        return true;
    }

    bool Application::Init()
    {
        if (!CreateAppWindow(1280, 720, "Jupiter Engine"))
        {
            Debug("Failed to create the platform window.");
            return false;
        }

        m_status = Status::Running;
        return true;
    }

    void Application::Update(float64 deltaSeconds)
    {
        static float64 s_totalTime = 0.0;
        s_totalTime += deltaSeconds;
        if (s_totalTime > 5.0)
        {
            m_status = Status::Succeeded;
            Debug("5 seconds elapsed, exiting.");
        }
    }

    void Application::Terminate()
    {
        Debug("Application Terminated.");
    }

    void Application::Run(int argc, char* argv[])
    {
        RunAppLoop(argc, argv);
    }
}