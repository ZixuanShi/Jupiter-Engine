#include "Core/Building/JupiterPCH.h"
#include "Applications/EntryPoint.h"

#include "DemoGameApplication.h"

jpt::Application* jpt::CreateApplication()
{
	return JPT_NEW(DemoGameApplication);
}