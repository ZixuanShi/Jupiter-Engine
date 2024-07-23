// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application_Base.h"
#include "Debugging/Logger.h"

#if IS_PLATFORM_WIN64
	#include "Applications/App/Application_Win64.h"
	#include <Windows.h>
#endif

export module jpt.EntryPoints;

import jpt.CommandLine;

#if IS_DEBUG
	import jpt.MemoryLeakDetector;
#endif

namespace jpt
{
	int MainImpl_Final(Application_Base* pApp)
	{
		JPT_LOG("Application Launched with Args: " + CommandLine::GetInstance().ToString());

#if IS_DEBUG
		MemoryLeakDetector::Init();
#endif

		if (pApp->PreInit() && pApp->Init())
		{
			pApp->Run();
		}

		pApp->Terminate();

		return 0;
	}

#if IS_PLATFORM_WIN64
	export int MainImpl_Win64(Application_Win64* pApp, HINSTANCE hInstance, LPSTR launchArgs, int nCmdShow)
	{
		CommandLine::GetInstance().Parse(launchArgs);

		pApp->SetHINSTANCE(hInstance);
		pApp->SetnCmdShow(nCmdShow);

		return MainImpl_Final(pApp);
	}
#else
	export int MainImpl(Application_Base* pApp)
	{
		CommandLine::GetInstance().Parse(argc, argv);

		return MainImpl_Final(pApp);
	}
#endif
}
