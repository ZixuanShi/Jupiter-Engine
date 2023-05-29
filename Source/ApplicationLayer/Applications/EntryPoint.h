#pragma once
#include "ApplicationLayer/Applications/ApplicationBase.h"

// Designed to be implemented in client game C++
// Returns: An Application's pointer to run the game
extern jpt::ApplicationBase* CreateApplication();

#if IS_PLATFORM_WIN64
#include "ApplicationLayer/Applications/Platform_Win64/Win64Application.h"

#include <windows.h>

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	jpt::Win64Application* pApplication = static_cast<jpt::Win64Application*>(CreateApplication());
	pApplication->SetHINSTANCE(hInstance);
	pApplication->SetnCmdShow(nCmdShow);

	if (pApplication->Init())
	{
		pApplication->RunGameLoop();
	}

	pApplication->Clean();
	JPT_SAFE_DELETE(pApplication);

	_CrtDumpMemoryLeaks();

	return 0;
}

#else
int main(int, char**)
{
	jpt::ApplicationBase* pApplication = CreateApplication();

	if (pApplication->Init())
	{
		pApplication->Update();
	}

	pApplication->Clean();
	JPT_DELETE(pApplication);

	_CrtDumpMemoryLeaks();

	return 0;
}
#endif
