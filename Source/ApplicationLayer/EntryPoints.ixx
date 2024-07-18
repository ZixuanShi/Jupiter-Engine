// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_WIN64
	#include <Windows.h>
#endif

export module jpt.EntryPoints;

import jpt.Application_Base;
import jpt.LaunchArgs;

#if IS_DEBUG
	import jpt.MemoryLeakDetector;
#endif

#if IS_PLATFORM_WIN64
	import jpt.Application_Win64;
#endif

namespace jpt
{
	static int MainFinal()
	{
		#if IS_DEBUG
			MemoryLeakDetector::Init();
		#endif

		Application_Base* app = Application_Base::GetInstance();
		if (app->PreInit() && app->Init())
		{
			app->Run();
		}

		app->Terminate();

		return 0;
	}

#if IS_PLATFORM_WIN64
	export int MainImpl_Win64(HINSTANCE hInstance, HINSTANCE, LPSTR args, int nCmdShow)
	{
		LaunchArgs::GetInstance().Parse(args);

		Application_Win64* app = static_cast<Application_Win64*>(Application_Win64::GetInstance());
		app->SetHINSTANCE(hInstance);
		app->SetnCmdShow(nCmdShow);

		return MainFinal();
	}	
}
#else
	export int MainImpl(int argc, char* argv[])
	{
		LaunchArgs::GetInstance().Parse(argc, argv);

		return MainFinal();
	}
#endif