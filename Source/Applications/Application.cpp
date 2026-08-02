// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Graphics/Renderer.h"

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
        Debug::Log("Jupiter Engine from {}-{}", jpt::GetPlatformName(), jpt::GetConfigName());
        return true;
    }

    bool Application::Init()
    {
        if (!CreateAppWindow(1280, 720, "Jupiter Engine"))
        {
            Debug::Log("Failed to create the platform window.");
            return false;
        }

        m_status = Status::Running;
        return true;
    }

    void Application::Update([[maybe_unused]] float64 deltaSeconds)
    {
    }

    void Application::Terminate()
    {
        m_renderer.Terminate();
        m_status = Status::Succeeded;
        Debug::Log("Application Terminated.");
    }

    void Application::Run(int argc, char* argv[])
    {
        RunAppLoop(argc, argv);
    }

    bool Application::OnSurfaceReady(void* pMetalLayer)
    {
        if (!m_renderer.Init(pMetalLayer))
        {
            Debug::Log("Failed to initialise the renderer.");
            return false;
        }
        return true;
    }

    void Application::OnResize(uint32 pixelWidth, uint32 pixelHeight)
    {
        m_renderer.OnResize(pixelWidth, pixelHeight);
    }

    void Application::OnFrameDraw()
    {
        if (m_status != Status::Running)
        {
            return;
        }

        m_frameTimer.BeginFrame();

        // Drawing sits outside Update so an override cannot forget it.
        Update(m_frameTimer.GetDeltaSeconds());
        m_renderer.OnFrameDraw(m_frameTimer.GetElapsedSeconds());

        m_frameTimer.EndFrame();
    }
}
