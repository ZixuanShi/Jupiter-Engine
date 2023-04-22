#include "Core/EntryPoint.h"

#include "DemoGameApplication.h"

jpt::Application* jpt::CreateApplication()
{
	return new DemoGameApplication();
}