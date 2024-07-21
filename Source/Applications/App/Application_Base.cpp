// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/App/Application_Base.h"
#include "Core/Minimal/CoreMacros.h"

import jpt.Application_Factories;

namespace jpt
{
	bool jpt::Application_Base::PreInit()
	{
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
		m_pFramework->Terminate();
		JPT_SAFE_DELETE(m_pFramework);

		m_pWindow->Terminate();
		JPT_SAFE_DELETE(m_pWindow);
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