#include "Core/Building/JupiterPCH.h"
#include "Applications/EntryPoint.h"

#include "DemoGameApplication.h"

jpt::Application* jpt::CreateApplication()
{
	int32* pInt = jpt::Test();
	JPT_WARNING("%i", *pInt);
	JPT_DELETE(pInt);
	return JPT_NEW(DemoGameApplication);
}