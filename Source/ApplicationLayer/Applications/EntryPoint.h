// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#if IS_DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "ApplicationLayer/Applications/ApplicationBase.h"

/** Designed to be implemented in client game C++
	@return An Application's pointer to run the game */
extern jpt::ApplicationBase* CreateApplication();

#if IS_PLATFORM_WIN64

#include "ApplicationLayer/Applications/Platform_Win64/Win64Application.h"

#include <windows.h>

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
#if IS_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

	jpt::Win64Application* pApplication = dynamic_cast<jpt::Win64Application*>(CreateApplication());
	pApplication->SetHINSTANCE(hInstance);
	pApplication->SetnCmdShow(nCmdShow);

	if (pApplication->PreInit() && 
		pApplication->Init())
	{
		pApplication->RunGameLoop();
	}

	pApplication->Clean();
	JPT_SAFE_DELETE(pApplication);

	return 0;
}

#else
int main(int, char**)
{
#if IS_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

	jpt::ApplicationBase* pApplication = CreateApplication();

	if (pApplication->Init())
	{
		pApplication->Update();
	}

	pApplication->Clean();
	JPT_DELETE(pApplication);

	return 0;
}
#endif
