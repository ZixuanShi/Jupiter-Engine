#pragma once
#include "Application.h"
#include "Core/MemoryManagement/MemoryTracking.h"

extern jpt::Application* jpt::CreateApplication();

int main(int32 argc, char** argv)
{
	JPT_UNUSED(argc);
	JPT_UNUSED(argv);

	JPT_USE_TIMING_PROFILER("Running");

	jpt::Application* pApplication = jpt::CreateApplication();
	JPT_ASSERT(pApplication, "Application should not be nullptr in main()");
	if (pApplication->Init())
	{
		pApplication->Update();
	}
	pApplication->Clean();
	JPT_DELETE(pApplication);

#if JPT_ENABLE_MEMORY_TRACKING
	//jpt::MemoryTracker::GetInstance()->ShutdownMemoryLeakDetector();
#endif

	return 0;
}
