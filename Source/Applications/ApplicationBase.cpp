// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/GetApp.h"
#include "Graphics/ImGui/ImGuiLayer.h"
#include "Graphics/Renderer.h"

module jpt.ApplicationBase;

import jpt.Environment;
import jpt.Logger;
import jpt.Window;

#if IS_EDITOR
    import jpt.EditorUI;
#endif

#if !IS_CONFIG_RELEASE
    import jpt.InputTests;
    import jpt.MathTests;
#endif

namespace jpt
{
    bool ApplicationBase::PreInit(int argc, char* argv[])
    {
        Debug::PreInit();
        Debug::Info("Jupiter Engine from {}-{}-{}", GetPlatformName(), GetConfigName(), GetAppName());
        m_launchArgs.PreInit(argc, argv);

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

        if (!m_window.PreInit())
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

        // After the window: the renderer overwrites what SDL set on the layer, and the pixel
        // format it picks here is baked into every pipeline.
        if (!OnSurfaceReady(m_window.GetSurface()))
        {
            return false;
        }

        // After the surface: OnResize is a no-op while the layer is null.
        OnResize(m_window.GetWidth(), m_window.GetHeight());

        // After OnSurfaceReady, where ImGuiInit creates the context this attaches to.
        ImGuiInitPlatform(m_window.GetNativeHandle());

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
        // SDL_AppQuit runs even when SDL_AppInit failed. Measured: a second call does not crash,
        // since every subsystem nulls its own handles -- this holds the single-banner contract.
        if (m_status == Status::Succeeded)
        {
            return;
        }
        m_status = Status::Succeeded;

        m_renderer.Terminate();
        m_window.Terminate();

        Debug::Info("Application Terminated.");
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
#if IS_EDITOR
            m_editorUI.Draw();
#endif
            m_renderer.EndFrame();
        }

        m_frameTimer.EndFrame();
    }

    bool ApplicationBase::OnSurfaceReady(Renderer::SurfaceHandle surface)
    {
        // The first point at which a mesh can be uploaded, which is why a client hides this.
        if (!m_renderer.Init(surface))
        {
            Debug::Error("Failed to initialize the renderer.");
            return false;
        }

        return true;
    }

    void ApplicationBase::SetPaused(bool paused) noexcept
    {
        if (m_status == Status::Running || m_status == Status::Paused)
        {
            m_status = paused ? Status::Paused : Status::Running;
        }
    }

    void ApplicationBase::OnResize(uint32 pixelWidth, uint32 pixelHeight)
    {
        m_window.OnResize(pixelWidth, pixelHeight);
        m_renderer.OnResize(pixelWidth, pixelHeight);
    }
}
