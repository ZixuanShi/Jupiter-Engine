// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/Window/Window.h"
#include "Graphics/Renderer.h"

module jpt.Application;

import jpt.Constants;
import jpt.LaunchArgs;
import jpt.Logger;
import jpt.Utils;
import std;

#if !IS_CONFIG_RELEASE
    import jpt.MathTests;
#endif

namespace jpt
{
    bool Application::PreInit()
    {
        Debug::Info("Jupiter Engine from {}-{}", jpt::GetPlatformName(), jpt::GetConfigName());

#if !IS_CONFIG_RELEASE
        RunMathTests();
#endif

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

    void Application::Update()
    {
        const float32 elapsed = static_cast<float32>(m_frameTimer.GetElapsedSeconds());
        constexpr float32 kPhase = kTwoPi<float32> / 3.0f;

        m_renderer.SetClearColor({ 0.5f + 0.5f * std::sin(elapsed),
                                   0.5f + 0.5f * std::sin(elapsed + kPhase),
                                   0.5f + 0.5f * std::sin(elapsed + kPhase * 2.0f) });
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
        m_window.OnResize(pixelWidth, pixelHeight);
        m_renderer.OnResize(pixelWidth, pixelHeight);
    }

    void Application::OnFrame()
    {
        if (m_status != Status::Running)
        {
            return;
        }

        m_frameTimer.Update();

        // TODO: Read input 
        
        Update();
        PostUpdate();

        m_renderer.Draw();
    }
}
