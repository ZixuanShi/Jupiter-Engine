// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/App/Application_Base.h"

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

import jpt.Application_Factories;
import jpt.CommandLine;
import jpt.ProjectSettings;
import jpt.Framework_Base;
import jpt.Window_Base;
import jpt.Renderer_Base;
import jpt.Input.Manager;
import jpt.Input.KeyCode;

import jpt.System.Paths;
import jpt.StopWatch;

import jpt.ToString;

namespace jpt
{
	bool jpt::Application_Base::PreInit()
	{
		System::Paths::GetInstance().PreInit(File::GetClientDirW(), File::GetOutputDirW());
		JPT_LOG("Application Launched with Args: " + CommandLine::GetInstance().ToString());

		ProjectSettings::GetInstance().PreInit();

		m_pFramework = Framework_Create();
		m_pWindow    = Window_Create();
		m_pRenderer  = Renderer_Create();
		m_pInputManager = new Input::Manager();

		m_pFramework->PreInit();
		m_pWindow->PreInit();
		m_pRenderer->PreInit();
		m_pInputManager->PreInit();

		JPT_LOG("Application PreInit CommandLines: " + CommandLine::GetInstance().ToString());

		return true;
	}

	bool Application_Base::Init()
	{
		if (CommandLine::GetInstance().Has("no_window"))
		{
			m_shouldTerminate = true;
			return true;
		}

		m_pFramework->Init();
		m_pWindow->Init(this);
		m_pRenderer->Init();
		m_pInputManager->Init();

		return true;
	}

	void Application_Base::Update(TimePrecision deltaSeconds)
	{
		m_pFramework->Update(deltaSeconds);
		m_pWindow->Update(deltaSeconds);
		m_pRenderer->Update();
		m_pInputManager->Update();
	}

	void Application_Base::Terminate()
	{
		JPT_TERMINATE(m_pFramework);
		JPT_TERMINATE(m_pWindow);
		JPT_TERMINATE(m_pRenderer);
		JPT_TERMINATE(m_pInputManager);
	}

	void Application_Base::Run()
	{
		StopWatch::Point previous = StopWatch::Now();
		TimePrecision accumulator = 0.0;
		uint32 frameCount = 0;

		while (!m_shouldTerminate)
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
				JPT_LOG("FPS: " + ToString(frameCount));
				frameCount = 0;
				accumulator = 0.0;
			}
		}
	}

	void Application_Base::ProcessInput()
	{
		if (m_pInputManager->IsKeyPressed(Input::KeyCode::Keyboard_Escape))
		{
			m_shouldTerminate = true;
		}
	}

	void Application_Base::Render()
	{
	}
}