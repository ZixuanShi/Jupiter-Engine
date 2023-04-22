#include "JupiterPCH.h"
#include "Application.h"

namespace jpt
{
Application* Application::s_instance = nullptr;

bool Application::Init()
{
	JPT_LOG("Hello World Jupiter Engine");
	return true;
}

void Application::Update()
{

}

void Application::Clean()
{
}
}
