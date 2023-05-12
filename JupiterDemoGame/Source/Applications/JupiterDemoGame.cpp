#include "Core/Building/JupiterPCH.h"
#include "Applications/EntryPoint.h"

#include "DemoGameApplication.h"

jpt::Application CreateApplication()
{
	JPT_START_TIMING_PROFILER("Create Application");

	// String View
	std::string stdStr = "std string";
	std::string_view stdStrView(stdStr.c_str() + 2, 3);
	std::string_view stdStrView2("std stringview raw");
	std::string_view stdStrView3(stdStr);
	std::cout << stdStrView << '\n';
	std::cout << stdStrView2 << '\n';
	std::cout << stdStrView3 << '\n';

	jpt::string jptStr = "jpt string";
	jpt::string_view jptStrView(jptStr.c_str() + 2, 3);
	jpt::string_view jptStrView2("jpt stringview raw");
	jpt::string_view jptStrView3(jptStr);
	std::cout << jptStrView << '\n';
	std::cout << jptStrView2 << '\n';
	std::cout << jptStrView3 << '\n';

	return DemoGameApplication();
}