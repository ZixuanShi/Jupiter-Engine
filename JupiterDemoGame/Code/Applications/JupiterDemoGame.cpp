#include "Core/Building/JupiterPCH.h"
#include "Applications/EntryPoint.h"

#include "DemoGameApplication.h"

jpt::Application jpt::CreateApplication()
{
	JPT_START_TIMING_PROFILER("Create Application");

	jpt::vector<jpt::string> strings{ "Jupiter Engine", "Zixuan Shi", "Jupiter Technologies" };
	for (const jpt::string& str : strings)
	{
		JPT_LOG("%s", str.c_str());
	}

	return DemoGameApplication();
}