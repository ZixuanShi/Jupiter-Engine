#include "Core/Building/JupiterPCH.h"
#include "Core/EntryPoint.h"

#include "DemoGameApplication.h"

jpt::Application* jpt::CreateApplication()
{
	return JPT_NEW(DemoGameApplication());
}