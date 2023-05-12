#include "Core/Building/JupiterPCH.h"
#include "Applications/EntryPoint.h"

#include "JupiterUnitTestsApplication.h"

jpt::Application* CreateApplication()
{
	JPT_START_TIMING_PROFILER("Create Application");
	return new JupiterUnitTestsApplication();
}