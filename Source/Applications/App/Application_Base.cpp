// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/App/Application_Base.h"

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

import jpt.Application_Factories;
import jpt.CommandLine;

import jpt.ToString;

import jpt.Framework;
import jpt.Window;
import jpt.Renderer_Base;

import jpt.Input.Manager;
import jpt.Input.KeyCode;

import jpt.System.Paths;
import jpt.StopWatch;
import jpt.ProjectSettings;

import jpt.Event.Manager;
import jpt.Event.Window.Close;

namespace jpt
{
	bool jpt::Application_Base::PreInit()
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
		m_pWindow = Window_Create();
		//m_pRenderer = Renderer_Create();

		bool success = true;
		success &= m_pFramework->PreInit();
		success &= m_pWindow->PreInit();
		//success &= m_pRenderer->PreInit();
		success &= Input::Manager::GetInstance().PreInit();

		return success;
	}

	bool Application_Base::Init()
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
		success &= m_pWindow->Init();
		//success &= m_pRenderer->Init();
		success &= Input::Manager::GetInstance().Init();

		return success;
	}

	void Application_Base::Update(TimePrecision deltaSeconds)
	{
		EventManager::GetInstance().Update(deltaSeconds);

		if (!CommandLine::GetInstance().Has("no_window"))
		{
			m_pFramework->Update(deltaSeconds);
			m_pWindow->Update(deltaSeconds);
			//m_pRenderer->Update(deltaSeconds);
		}

		Input::Manager::GetInstance().Update(deltaSeconds);
	}

	void Application_Base::Shutdown()
	{
		if (!CommandLine::GetInstance().Has("no_window"))
		{
			JPT_SHUTDOWN(m_pWindow);
			JPT_SHUTDOWN(m_pFramework);
			//JPT_SHUTDOWN(m_pRenderer);
		}

		Input::Manager::GetInstance().Shutdown();
		ProjectSettings::GetInstance().Save();
		EventManager::GetInstance().Shutdown();
	}

	void Application_Base::Run()
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
			Render();

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

	void Application_Base::ProcessInput()
	{
		if (Input::Manager::GetInstance().IsPressed(Input::KeyCode::Keyboard_Escape))
		{
			EventManager::GetInstance().Send(Event_Window_Close(nullptr));
		}
	}

	void Application_Base::Render()
	{
		//m_pRenderer->Render();
	}
}