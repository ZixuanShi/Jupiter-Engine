// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/Window/Window.h"
#include "Graphics/Renderer.h"

module jpt.Application;

import jpt.Environment;
import jpt.LaunchArgs;
import jpt.Logger;
import jpt.ObjLoader;
import jpt.Texture;
import jpt.TextureLoader;
import jpt.Vector3;

import std;

#if !IS_CONFIG_RELEASE
    import jpt.EditorUI;
    import jpt.InputTests;
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

    bool Application::Init()
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

    void Application::Update()
    {
        m_input.Update();       // Recognizes gestures, whose handlers write the scene.
        m_scene.Update();       // Polls held keys, which a one-shot gesture event cannot express.
        m_renderer.Update();
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
            Debug::Error("Failed to initialize the renderer.");
            return false;
        }

        if (!m_renderer.SetMesh(LoadObj("Assets/Meshes/Mug.obj")))
        {
            Debug::Error("Failed to upload the mesh.");
            return false;
        }

        // Indexed by TextureSlot, so the order here is the [[texture(n)]] order in the shader.
        static constexpr const char* kMaps[kTextureSlotCount] =
        {
            "Assets/Textures/Mug_BC.jpg",
            "Assets/Textures/Mug_Normal.png",
            "Assets/Textures/Mug_Roughness.jpg",
            "Assets/Textures/Mug_Metallic.jpg",
            "Assets/Textures/Mug_AO.jpg",
        };

        std::array<Texture, kTextureSlotCount> textures;
        for (usize slot = 0; slot < kTextureSlotCount; ++slot)
        {
            textures[slot] = LoadTexture(kMaps[slot]);
        }

        if (!m_renderer.SetTextures(textures))
        {
            Debug::Error("Failed to upload the material textures.");
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

        if (m_renderer.BeginFrame())
        {
            DrawUI();
            m_renderer.EndFrame();
        }
    }

    void Application::DrawUI()
    {
#if !IS_CONFIG_RELEASE
        DrawEditorUI();
#endif
    }
}
