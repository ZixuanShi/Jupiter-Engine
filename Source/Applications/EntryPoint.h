#pragma once
#include "Application.h"

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
