#include "Core/Building/JupiterPCH.h"
#include "Applications/EntryPoint.h"

#include "DemoGameApplication.h"

jpt::Application jpt::CreateApplication()
{
	JPT_START_TIMING_PROFILER("Create Application");

	//jpt::vector<jpt::string> strings{ "Jupiter Engine", "Zixuan Shi", "Jupiter Technologies" };
	//for (jpt::string& str : strings)
	//{
	//	JPT_LOG("Compare result: %s", str == "Zixuan Shi" ? "true" : "false");
	//}

	jpt::string str1("Test");
	jpt::string substr(str1.substr(2,2));

	str1 += substr + "45";
	str1.find_last_of("Z");

	str1 += jpt::to_string(123);
	str1 = std::move(substr);

	JPT_LOG("%s", str1.c_str());
	JPT_LOG("%s", substr.c_str());

	return DemoGameApplication();
}