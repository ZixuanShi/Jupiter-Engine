#include "Application.h"

#include <iostream>

namespace jpt
{
Application* Application::s_instance = nullptr;

bool Application::Init()
{
	std::cout << "Hello World Jupiter Engine" << std::endl;
	return true;
}

void Application::Update()
{

}

void Application::Clean()
{
}
}
