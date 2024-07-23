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
	export int MainImpl(Application_Base* pApp)
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
}
