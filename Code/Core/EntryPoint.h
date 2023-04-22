#pragma once
#include "Applications/Application.h"

extern jpt::Application* jpt::CreateApplication();

int main(int32 argc, char** argv)
{
	JPT_UNUSED(argc);
	JPT_UNUSED(argv);

	jpt::Application* pApplication = jpt::CreateApplication();
	if (pApplication->Init())
	{
		pApplication->Update();
	}
	pApplication->Clean();
	JPT_SAFE_DELETE(pApplication);

	return 0;
}