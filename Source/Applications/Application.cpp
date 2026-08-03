// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/Window/Window.h"
#include "Graphics/Renderer.h"

module jpt.Application;

import jpt.LaunchArgs;
import jpt.Logger;
import jpt.Utils;

namespace jpt
{
    bool Application::PreInit()
    {
        Debug::Info("Jupiter Engine from {}-{}", jpt::GetPlatformName(), jpt::GetConfigName());

        const LaunchArgs& launchArgs = LaunchArgs::GetInstance();
        if (!m_window.PreInit(launchArgs.GetCount(), launchArgs.GetValues()))
        {
            Debug::Error("Failed to pre-initialise the window.");
            return false;
        }

        if (!m_renderer.PreInit())
        {
            Debug::Error("Failed to pre-initialise the renderer.");
            return false;
        }

        return true;
    }

    bool Application::Init()
    {
        if (!m_window.Init())
        {
            Debug::Error("Failed to initialise the window.");
            return false;
        }

        m_status = Status::Running;
        return true;
    }

    void Application::Terminate()
    {
        m_renderer.Terminate();
        m_window.Terminate();
        m_status = Status::Succeeded;
        Debug::Info("Application Terminated.");
    }

    void Application::Run()
    {
        m_window.Run();
    }

    bool Application::OnSurfaceReady(Renderer::SurfaceHandle surface)
    {
        if (!m_renderer.Init(surface))
        {
            Debug::Error("Failed to initialise the renderer.");
            return false;
        }
        return true;
    }

    void Application::OnResize(uint32 pixelWidth, uint32 pixelHeight)
    {
        m_renderer.OnResize(pixelWidth, pixelHeight);
    }

    void Application::OnFrame()
    {
        if (m_status != Status::Running)
        {
            return;
        }

        m_frameTimer.BeginFrame();
        m_renderer.OnFrameDraw(m_frameTimer.GetElapsedSeconds());
        m_frameTimer.EndFrame();
    }
}
