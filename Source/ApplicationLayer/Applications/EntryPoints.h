// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#if IS_DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#if IS_PLATFORM_WIN64

#include "ApplicationBase.h"
#include <Windows.h>
//#include "ApplicationLayer/Applications/Platform_Win64/Win64Application.h"

namespace jpt
{
	int MainImpl(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
	{
#if IS_DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

		// jpt::Win64Application& win64App = static_cast<jpt::Win64Application&>(jpt::Win64Application::GetInstance());
		// win64App.SetHINSTANCE(hInstance);
		// win64App.SetnCmdShow(nCmdShow);

		// if (win64App.PreInit() &&
		// 	win64App.Init())
		// {
		// 	win64App.RunGameLoop();
		// }

		// win64App.Clean();

		jpt::ApplicationBase& app = jpt::ApplicationBase::GetInstance();
		if (app.Init())
		{
			app.Update();
		}

		app.Clean();

		return 0;
	}	
#endif
}