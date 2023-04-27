#include "Core/Building/JupiterPCH.h"
#include "Applications/EntryPoint.h"

#include "DemoGameApplication.h"

jpt::Application* jpt::CreateApplication()
{
	int32* pInt = JPT_NEW(int32);
	JPT_WARNING("%i", *pInt);
	JPT_DELETE(pInt);

	float* pAnother = JPT_NEW_WITH_VALUE(float, 42.056f);
	JPT_WARNING("%.3f", *pAnother);
	JPT_DELETE(pAnother);

	return JPT_NEW(DemoGameApplication);
}
