#include "Core/Building/JupiterPCH.h"
#include "ApplicationLayer/Applications/EntryPoint.h"

#include "JupiterUnitTestsApplication.h"

jpt::ApplicationBase* CreateApplication()
{
	JPT_START_TIMING_PROFILER("Create Application");

	JPT_LOG("%d", sizeof(jpt::wstring));
	JPT_LOG("%d", sizeof(jpt::string));
	return new JupiterUnitTestsApplication();
}