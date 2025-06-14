// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

module jpt.Application;

import jpt.Framework;
import jpt.Framework_Create;
import jpt.Window;
import jpt.WindowManager;
import jpt.CommandLine;
import jpt.FrameTimer;

import jpt.AssetManager;
import jpt.SceneManager;

import jpt.Math;
import jpt.ToString;

import jpt.Debugger;

import jpt.EventManager;
import jpt.Event_Window_Close;
import jpt.Event_Key;

import jpt.Renderer;
import jpt.Renderer_Create;

import jpt.Input_Enums;
import jpt.InputManager;

import jpt.Platform;
import jpt.ProjectSettings;
import jpt.SystemPaths;
import jpt.HardwareManager;

namespace jpt
{
    bool jpt::Application::PreInit()
    {
        System::Paths::GetInstance().PreInit();
        Debugger::GetInstance().PreInit();
        JPT_LOG("Application Launched with Args: " + ToString(CommandLine::GetInstance().GetArgs()));

        HardwareManager::GetInstance().PreInit();

        ProjectSettings::GetInstance().Load();

        if (CommandLine::GetInstance().Has("no_window"))
        {
            m_status = Status::Success;
            return true;
        }

        // Pick APIs
        m_frameworkAPI = FindFrameworkAPI();
        m_graphicsAPI  = FindGraphicsAPI();

        // Initialize core systems
        JPT_ASSERT(m_pPlatform, "Platform is not set");
        m_pFramework     = Framework_Create(m_frameworkAPI);
        m_pWindowManager = new WindowManager();
        m_pRenderer      = Renderer_Create(m_graphicsAPI);

        bool success = true;
        success &= m_pPlatform->PreInit();
        success &= m_pFramework->PreInit();
        success &= m_pWindowManager->PreInit();
        success &= m_pRenderer->PreInit();
        success &= InputManager::GetInstance().PreInit(m_frameworkAPI);
        success &= SceneManager::GetInstance().PreInit();
        success &= AssetManager::GetInstance().PreInit();

        // Register for events
        jpt::EventManager::GetInstance().Register<jpt::Event_Key>([this](const jpt::Event_Key& eventKeyboardKeyPress)
            {
                if (eventKeyboardKeyPress.GetKey() == jpt::Input::Key::Escape)
                {
                    m_status = Status::Success;
                }
                if (eventKeyboardKeyPress.GetKey() == jpt::Input::Key::N && eventKeyboardKeyPress.GetState() == jpt::Input::KeyState::Pressed)
                {
                    jpt::Window* pWindow = m_pWindowManager->Create();
                    GetRenderer()->RegisterWindow(pWindow);
                }
            });

        return success;
    }

    bool Application::Init()
    {
        if (CommandLine::GetInstance().Has("no_window"))
        {
            return true;
        }

        // Initialize systems
        bool success = true;
        success &= m_pPlatform->Init();
        success &= m_pFramework->Init();
        success &= m_pWindowManager->Init(GetName());
        success &= m_pRenderer->Init();
        success &= InputManager::GetInstance().Init();
        success &= SceneManager::GetInstance().Init();
        success &= AssetManager::GetInstance().Init();

        if (success)
        {
            m_status = Status::Running;
        }

        return success;
    }

    void Application::Update(TimePrecision deltaSeconds)
    {
        EventManager::GetInstance().Update(deltaSeconds);
        m_pPlatform->Update(deltaSeconds);
        m_pFramework->Update(deltaSeconds);
        m_pWindowManager->Update(deltaSeconds);
        m_pRenderer->Update(deltaSeconds);
        InputManager::GetInstance().Update(deltaSeconds);
        SceneManager::GetInstance().Update(deltaSeconds);
    }

    void Application::Shutdown()
    {
        ProjectSettings::GetInstance().Save();

        AssetManager::GetInstance().Shutdown();
        SceneManager::GetInstance().Shutdown();
        InputManager::GetInstance().Shutdown();
        EventManager::GetInstance().Shutdown();

        if (!CommandLine::GetInstance().Has("no_window"))
        {
            JPT_SHUTDOWN(m_pRenderer);
            JPT_SHUTDOWN(m_pWindowManager);
            JPT_SHUTDOWN(m_pFramework);
        }

        JPT_SHUTDOWN(m_pPlatform);
    }

    void Application::Run()
    {
        FrameTimer frameTimer;

        while (m_status == Status::Running)
        {
            frameTimer.BeginFrame();

            Update(frameTimer.GetDeltaSeconds());
            m_pRenderer->DrawFrame();

            frameTimer.EndFrame();
        }
    }

    Window* Application::GetMainWindow() const
    {
        return m_pWindowManager->GetMainWindow();
    }
}