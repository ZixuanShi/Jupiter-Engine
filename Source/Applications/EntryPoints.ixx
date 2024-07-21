// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application_Base.h"
#include "Debugging/Logger.h"

#if IS_PLATFORM_WIN64
	#include <Windows.h>
#endif

export module jpt.EntryPoints;

import jpt.Application_Win64;
import jpt.CommandLine;

#if IS_DEBUG
	import jpt.MemoryLeakDetector;
#endif

namespace jpt
{
	static int MainImpl()
	{
		JPT_LOG("Application Launched with Args: " + CommandLine::GetInstance().ToString());

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
}

#if IS_PLATFORM_WIN64

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR launchArgs, int nCmdShow)
{
	using namespace jpt;

	CommandLine::GetInstance().Parse(launchArgs);

	Application_Win64* app = static_cast<Application_Win64*>(Application_Base::GetInstance());
	app->SetHINSTANCE(hInstance);
	app->SetnCmdShow(nCmdShow);

	return MainImpl();
}
#else

	int main(int argc, char* argv[])
	{
		jpt::CommandLine::GetInstance().Parse(argc, argv);

		return jpt::MainImpl();
	}
#endif