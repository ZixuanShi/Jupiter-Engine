// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/Window/Window.h"
#include "Graphics/Renderer.h"

module jpt.ApplicationBase;

import jpt.Environment;
import jpt.LaunchArgs;
import jpt.Logger;
import jpt.Vector3;

import std;

#if !IS_CONFIG_RELEASE
    import jpt.EditorUI;
    import jpt.InputTests;
    import jpt.MathTests;
#endif

namespace jpt
{
    bool ApplicationBase::PreInit()
    {
        Debug::Info("Jupiter Engine from {}-{}", jpt::GetPlatformName(), jpt::GetConfigName());

#if !IS_CONFIG_RELEASE
        RunMathTests();
#endif

        // Before the window, which starts delivering events.
        if (!m_input.PreInit())
        {
            Debug::Error("Failed to pre-initialize input.");
            return false;
        }

#if !IS_CONFIG_RELEASE
        RunInputTests();
#endif

        const LaunchArgs& launchArgs = LaunchArgs::GetInstance();
        if (!m_window.PreInit(launchArgs.GetCount(), launchArgs.GetValues()))
        {
            Debug::Error("Failed to pre-initialize the window.");
            return false;
        }

        if (!m_renderer.PreInit())
        {
            Debug::Error("Failed to pre-initialize the renderer.");
            return false;
        }

        return true;
    }

    bool ApplicationBase::Init()
    {
        // Before the window: Window::Init() hands control to AppKit and events start arriving.
        if (!m_scene.Init())
        {
            Debug::Error("Failed to initialize the scene.");
            return false;
        }

        if (!m_window.Init())
        {
            Debug::Error("Failed to initialize the window.");
            return false;
        }

        m_status = Status::Running;
        return true;
    }

    void ApplicationBase::Update()
    {
        m_input.Update();       // Recognizes gestures, whose handlers write the scene.
        m_scene.Update();       // Polls held keys, which a one-shot gesture event cannot express.
        m_renderer.Update();
    }

    void ApplicationBase::PostUpdate()
    {
    }

    void ApplicationBase::Terminate()
    {
        m_renderer.Terminate();
        m_window.Terminate();

        m_status = Status::Succeeded;
        Debug::Info("Application Terminated.");
    }

    void ApplicationBase::Run()
    {
        m_window.Run();
    }

    void ApplicationBase::OnFrame()
    {
        if (m_status != Status::Running)
        {
            return;
        }

        m_frameTimer.Update();

        Update();
        PostUpdate();

        if (m_renderer.BeginFrame())
        {
#if !IS_CONFIG_RELEASE
            m_editorUI.Draw();
#endif
            m_renderer.EndFrame();
        }

        m_frameTimer.EndFrame();
    }

    bool ApplicationBase::OnSurfaceReady(Renderer::SurfaceHandle surface)
    {
        // Content is the client's: this is the first point at which a mesh can be uploaded, and on
        // iOS it arrives well after Init(), so a client hides this and loads there.
        if (!m_renderer.Init(surface))
        {
            Debug::Error("Failed to initialize the renderer.");
            return false;
        }

        return true;
    }

    void ApplicationBase::OnResize(uint32 pixelWidth, uint32 pixelHeight)
    {
        m_window.OnResize(pixelWidth, pixelHeight);
        m_renderer.OnResize(pixelWidth, pixelHeight);
    }
}
