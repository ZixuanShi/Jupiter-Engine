#pragma once
#include "Application.h"

namespace jpt
{
	extern Application* CreateApplication();
}

int main(/*int32 argc, char** argv*/)
{
	jpt::Application* pApplication = jpt::CreateApplication();
	JPT_ASSERT(pApplication, "Application should not be nullptr in main()");
	if (pApplication->Init())
	{
		pApplication->Update();
	}
	pApplication->Clean();
	JPT_DELETE(pApplication);

	return 0;
}
