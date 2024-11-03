// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/App/Application.h"

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

import jpt.CommandLine;

import jpt.ToString;

import jpt.Framework;
import jpt.Framework.Create;

import jpt.Window;
import jpt.Window.Create;

import jpt.Input.Manager;
import jpt.Input.KeyCode;

import jpt.System.Paths;
import jpt.StopWatch;
import jpt.ProjectSettings;

import jpt.Event.Manager;
import jpt.Event.Window.Close;

namespace jpt
{
	bool jpt::Application::PreInit()
	{
		System::Paths::GetInstance().PreInit(File::GetClientDirW(), File::GetOutputDirW());
		JPT_LOG("Application Launched with Args: " + CommandLine::GetInstance().ToString());

		ProjectSettings::GetInstance().Load();

		if (CommandLine::GetInstance().Has("no_window"))
		{
			m_shouldShutdown = true;
			return true;
		}

		// Initialize core systems
		m_pFramework = Framework_Create();
		m_pMainWindow = Window_Create();

		bool success = true;
		success &= m_pFramework->PreInit();
		success &= m_pMainWindow->PreInit();
		success &= Input::Manager::GetInstance().PreInit();

		return success;
	}

	bool Application::Init()
	{
		EventManager::GetInstance().Register<Event_Window_Close>([this](const Event_Window_Close&)
			{
				m_shouldShutdown = true;
			});

		if (CommandLine::GetInstance().Has("no_window"))
		{
			return true;
		}

		// Initialize systems
		bool success = true;
		success &= m_pFramework->Init();
		success &= m_pMainWindow->Init();
		success &= Input::Manager::GetInstance().Init();

		return success;
	}

	void Application::Update(TimePrecision deltaSeconds)
	{
		EventManager::GetInstance().Update(deltaSeconds);

		if (!CommandLine::GetInstance().Has("no_window"))
		{
			m_pFramework->Update(deltaSeconds);
			m_pMainWindow->Update(deltaSeconds);
		}

		Input::Manager::GetInstance().Update(deltaSeconds);
	}

	void Application::Shutdown()
	{
		if (!CommandLine::GetInstance().Has("no_window"))
		{
			JPT_SHUTDOWN(m_pMainWindow);
			JPT_SHUTDOWN(m_pFramework);
		}

		Input::Manager::GetInstance().Shutdown();
		ProjectSettings::GetInstance().Save();
		EventManager::GetInstance().Shutdown();
	}

	void Application::Run()
	{
		StopWatch::Point previous = StopWatch::Now();
		TimePrecision accumulator = 0.0;
		uint32 frameCount = 0;

		while (!m_shouldShutdown)
		{
			const StopWatch::Point current = StopWatch::Now();
			const TimePrecision deltaSeconds = StopWatch::GetSecondsBetween(previous, current);

			ProcessInput();
			Update(deltaSeconds);

			previous = current;

			++frameCount;
			accumulator += deltaSeconds;
			if (accumulator >= 1.0)
			{
				const bool showFPS = ProjectSettings::GetInstance().Get("show_fps", false);
				if (showFPS)
				{
					JPT_LOG("FPS: " + ToString(frameCount));
				}

				frameCount = 0;
				accumulator = 0.0;
			}
		}
	}

	void Application::ProcessInput()
	{
		if (Input::Manager::GetInstance().IsPressed(Input::KeyCode::Keyboard_Escape))
		{
			EventManager::GetInstance().Send(Event_Window_Close(nullptr));
		}
	}
}