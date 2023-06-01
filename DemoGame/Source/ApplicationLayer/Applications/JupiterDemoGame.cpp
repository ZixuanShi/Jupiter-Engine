#include "Core/Building/JupiterPCH.h"
#include "ApplicationLayer/Applications/EntryPoint.h"

#include "JupiterDemoGameApplication.h"

jpt::ApplicationBase* CreateApplication()
{
	JPT_START_TIMING_PROFILER("Create Application");
	return new JupiterDemoGameApplication();
}