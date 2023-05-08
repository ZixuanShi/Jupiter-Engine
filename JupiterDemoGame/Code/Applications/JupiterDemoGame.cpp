#include "Core/Building/JupiterPCH.h"
#include "Applications/EntryPoint.h"

#include "DemoGameApplication.h"

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
	JPT_LOG("%s", str.GetBuffer());

	jpt::vector<int32> vec{1,2,3,4,5};
	for (size_t i = 0; i < vec.size(); ++i)
	{
		JPT_LOG("%d", vec[i]);
	}

	return JPT_NEW(DemoGameApplication);
}