#pragma once
#include "Applications/Application.h"
#include "MemoryManagement/Macros.h"

extern jpt::Application* jpt::CreateApplication();

int main(int argc, char** argv)
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