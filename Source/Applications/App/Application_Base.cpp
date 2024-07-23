// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/App/Application_Base.h"
#include "Core/Minimal/CoreMacros.h"

import jpt.Application_Factories;
import jpt.CommandLine;
import jpt.Framework_Base;
import jpt.Window_Base;
import jpt.Renderer_Base;

namespace jpt
{
	bool jpt::Application_Base::PreInit()
	{
		if (CommandLine::GetInstance().Has("no_window"))
		{
			m_shouldTerminate = true;
			return true;
		}

		m_pFramework = Framework_Create();
		m_pFramework->Init();

		m_pWindow = Window_Create();
		m_pWindow->Init();

		return true;
	}

	void Application_Base::Update()
	{
		m_pFramework->Update();
		m_pWindow->Update();
	}

	void Application_Base::Terminate()
	{
		if (m_pFramework)
		{
			m_pFramework->Terminate();
			JPT_DELETE(m_pFramework);
		}

		if (m_pWindow)
		{
			m_pWindow->Terminate();
			JPT_DELETE(m_pWindow);
		}
	}

	void Application_Base::Run()
	{
		while (!m_shouldTerminate)
		{
			PollInput();
			Update();
			Render();
		}
	}

	void Application_Base::TerminateApp()
	{
		m_shouldTerminate = true;
	}
}