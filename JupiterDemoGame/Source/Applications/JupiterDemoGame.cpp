#include "Core/Building/JupiterPCH.h"
#include "Applications/EntryPoint.h"

#include "DemoGameApplication.h"

jpt::Application* CreateApplication()
{
	JPT_START_TIMING_PROFILER("Create Application");
	return new DemoGameApplication();
}