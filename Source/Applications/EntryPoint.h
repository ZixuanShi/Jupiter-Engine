#pragma once
#include "Application.h"

// Designed to be implemented in client game C++
// Returns: An Application's pointer to run the game
extern jpt::Application* CreateApplication();

int main()
{
	jpt::Application* pApplication = CreateApplication();
	if (pApplication->Init())
	{
		pApplication->Update();
	}
	pApplication->Clean();
	JPT_DELETE(pApplication);

	_CrtDumpMemoryLeaks();

	return 0;
}
