#include "JupiterPCH.h"
#include "ApplicationLayer/Applications/EntryPoint.h"

#include "JupiterUnitTestsApplication.h"

jpt::ApplicationBase* CreateApplication()
{
	JPT_START_TIMING_PROFILER("Create Application");
	return new JupiterUnitTestsApplication();
}