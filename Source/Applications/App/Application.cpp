// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/App/Application.h"

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

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

import jpt.Input.KeyCode;
import jpt.Input.Manager;

import jpt.System.Paths;
import jpt.StopWatch;
import jpt.ProjectSettings;

import jpt.Event.Manager;

namespace jpt
{
	bool jpt::Application::PreInit()
	{
		Logger::GetInstance().PreInit();
		System::Paths::GetInstance().PreInit();
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

		bool success = true;
		success &= m_pPlatform->PreInit();
		success &= m_pFramework->PreInit();
		success &= m_pWindowManager->PreInit();
		success &= m_pRenderer->PreInit();
		success &= InputManager::GetInstance().PreInit(m_frameworkAPI);

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
	}

	void Application::Shutdown()
	{
		ProjectSettings::GetInstance().Save();

		if (!CommandLine::GetInstance().Has("no_window"))
		{
			JPT_SHUTDOWN(m_pWindowManager);
			JPT_SHUTDOWN(m_pFramework);
			JPT_SHUTDOWN(m_pRenderer);
		}

		JPT_SHUTDOWN(m_pPlatform);
		InputManager::GetInstance().Shutdown();
		EventManager::GetInstance().Shutdown();
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