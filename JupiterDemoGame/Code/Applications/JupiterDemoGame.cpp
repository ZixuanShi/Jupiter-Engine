#include "Core/Building/JupiterPCH.h"
#include "Applications/EntryPoint.h"

#include "DemoGameApplication.h"

#include <iostream>

jpt::Application* jpt::CreateApplication()
{
	JPT_START_TIMING_PROFILER("Create Application");

	int32* pInt = JPT_NEW(int32);
	JPT_WARNING("%i", *pInt);
	JPT_DELETE(pInt);

	float* pAnother = JPT_NEW_WITH_VALUE(float, 42.056f);
	JPT_ERROR("%.3f", *pAnother);
	JPT_DELETE(pAnother);

	jpt::string str = "Jupiter Engine";
	JPT_LOG("%s", str.c_str());
	std::cout << kInvalidValue<jpt::string>.c_str() << '\n';

	return JPT_NEW(DemoGameApplication);
}