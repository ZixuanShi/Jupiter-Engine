#include "Core/Building/JupiterPCH.h"
#include "Applications/EntryPoint.h"

#include "DemoGameApplication.h"

jpt::Application jpt::CreateApplication()
{
	JPT_START_TIMING_PROFILER("Create Application");

	jpt::vector<jpt::string> strings{ "Jupiter Engine", "Zixuan Shi", "Jupiter Technologies" };
	for (jpt::string& str : strings)
	{
		JPT_LOG("%s, %u, %u", str.c_str(), str.size(), str.capacity());
		str = jpt::string("Test");
	}
	for (jpt::string& str : strings)
	{
		JPT_LOG("%s, %u, %u", str.c_str(), str.size(), str.capacity())
	}
	return DemoGameApplication();
}