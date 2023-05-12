#include "Core/Building/JupiterPCH.h"
#include "Applications/EntryPoint.h"

#include "DemoGameApplication.h"

jpt::Application CreateApplication()
{
	JPT_START_TIMING_PROFILER("Create Application");

	// std
	std::string stdstr(std::to_string(123));
	JPT_LOG(stdstr.c_str());

	// jpt
	jpt::string jptstr(jpt::to_string(123));
	jpt::string subt = jptstr.substr(1,2);
	JPT_SYSTEM_INFO(subt.c_str());

	return DemoGameApplication();
}