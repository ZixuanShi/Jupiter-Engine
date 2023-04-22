#include "Core/Building/JupiterPCH.h"
#include "DemoGameApplication.h"

#include <iostream>

bool DemoGameApplication::Init()
{
    if (jpt::Application::Init())
    {
        std::cout << "Hello World Jupiter Demo Game" << std::endl;
    }
    return true;
}
