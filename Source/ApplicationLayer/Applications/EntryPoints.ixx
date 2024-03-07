// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if !IS_RELEASE
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#if IS_PLATFORM_WIN64
	#include <Windows.h>
#endif

export module jpt.EntryPoints;

// TODO: When Win64Application is implemented, do conditional import for application
export import jpt.ApplicationBase;

#if IS_PLATFORM_WIN64

export namespace jpt
{
	int MainImplWin64(HINSTANCE /*hInstance*/, HINSTANCE, LPSTR, int /*nCmdShow*/)
	{
#if !IS_RELEASE
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

		jpt::ApplicationBase& app = jpt::ApplicationBase::GetInstance();
		if (app.Init())
		{
			app.Run();
		}

		app.Terminate();

		return 0;
	}	
#endif
}