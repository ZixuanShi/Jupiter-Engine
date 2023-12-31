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
	int MainImplWin64(HINSTANCE /*hInstance*/, HINSTANCE, LPSTR, int /*nCmdShow*/)
	{
#if IS_DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

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