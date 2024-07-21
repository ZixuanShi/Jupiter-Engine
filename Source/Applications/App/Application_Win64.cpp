// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/App/Application_Win64.h"

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

import jpt.CommandLine;
import jpt.Utilities;
import jpt.Framework_GLFW;
import jpt.Window_GLFW;

namespace jpt
{
	bool Application_Win64::PreInit()
	{
		JPT_ENSURE(Super::PreInit());
		JPT_ENSURE(m_hInstance != nullptr);

		m_pFramework = new Framework_GLFW;
		m_pFramework->Init();

		m_pWindow = new Window_GLFW;
		m_pWindow->Init();

		return true;
	}

	bool Application_Win64::Init()
	{
		JPT_ENSURE(Super::Init());

		if (CommandLine::GetInstance().Has("no_window"))
		{
			m_shouldTerminate = true;
			return true;
		}

		return true;
	}

	void Application_Win64::Update()
	{
		Super::Update();

		m_pFramework->Update();
		m_pWindow->Update();
	}

	void Application_Win64::Terminate()
	{
		Super::Terminate();

		m_pFramework->Terminate();
		JPT_SAFE_DELETE(m_pFramework);

		m_pWindow->Terminate();
		JPT_SAFE_DELETE(m_pWindow);
	}
}