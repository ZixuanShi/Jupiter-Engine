#pragma once
#include "Application.h"

extern jpt::Application* jpt::CreateApplication();

int main(int32 argc, char** argv)
{
	JPT_UNUSED(argc);
	JPT_UNUSED(argv);

#if JPT_ENABLE_MEMORY_TRACKING
	JPT_INIT_MEMORY_TRACKER();
#endif

	jpt::Application* pApplication = jpt::CreateApplication();
	if (pApplication->Init())
	{
		pApplication->Update();
	}
	pApplication->Clean();
	JPT_SAFE_DELETE(pApplication);

#if JPT_ENABLE_MEMORY_TRACKING
	JPT_SHUTDOWN_MEMORY_TRACKER();
#endif

	return 0;
}
