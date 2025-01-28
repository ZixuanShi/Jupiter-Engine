// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

module jpt.Application;

import jpt.CommandLine;

import jpt.ToString;

import jpt.Hardware.Manager;

import jpt.Platform;

import jpt.Framework;
import jpt.Framework.Create;

import jpt.Window;
import jpt.Window.Manager;

import jpt.Renderer;
import jpt.Renderer.Create;

import jpt.Input.Enums;
import jpt.Input.Manager;

import jpt.Scene.Manager;

import jpt.System.Paths;
import jpt.StopWatch;
import jpt.ProjectSettings;

import jpt.Event.Manager;
import jpt.Event.Window.Close;
import jpt.Event.Key;

namespace jpt
{
	bool jpt::Application::PreInit()
	{
		System::Paths::GetInstance().PreInit();
		Logger::GetInstance().PreInit();
		JPT_LOG("Application Launched with Args: " + CommandLine::GetInstance().ToString());

		HardwareManager::GetInstance().PreInit();

		ProjectSettings::GetInstance().Load();

		if (CommandLine::GetInstance().Has("no_window"))
		{
			m_shouldShutdown = true;
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
		m_pSceneManager  = new SceneManager();

		bool success = true;
		success &= m_pPlatform->PreInit();
		success &= m_pFramework->PreInit();
		success &= m_pWindowManager->PreInit();
		success &= m_pRenderer->PreInit();
		success &= InputManager::GetInstance().PreInit(m_frameworkAPI);
		success &= m_pSceneManager->PreInit();

		// Register for events
		jpt::EventManager::GetInstance().Register<jpt::Event_Key>([this](const jpt::Event_Key& eventKeyboardKeyPress)
			{
				if (eventKeyboardKeyPress.GetKey() == jpt::Input::Key::Escape)
				{
					m_shouldShutdown = true;
				}
				if (eventKeyboardKeyPress.GetKey() == jpt::Input::Key::N)
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
		success &= m_pSceneManager->Init();

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
		m_pSceneManager->Update(deltaSeconds);
	}

	void Application::Shutdown()
	{
		ProjectSettings::GetInstance().Save();

		JPT_SHUTDOWN(m_pSceneManager);
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

	static TimePrecision GetDeltaSeconds()
	{
		static StopWatch::Point previous = StopWatch::Now();
		const StopWatch::Point current = StopWatch::Now();
		const TimePrecision deltaSeconds = StopWatch::GetSecondsBetween(previous, current);
		previous = current;
		return deltaSeconds;
	}

	void Application::Run()
	{
		while (!m_shouldShutdown)
		{
			const TimePrecision deltaSeconds = GetDeltaSeconds();

			Update(deltaSeconds);
			if (m_shouldShutdown)
			{
				break;
			}

			m_pRenderer->DrawFrame();
		}
	}

	Window* Application::GetMainWindow() const
	{
		return m_pWindowManager->GetMainWindow();
	}

	void Application::SetShouldShutdown()
	{
		m_shouldShutdown = true;
	}
}