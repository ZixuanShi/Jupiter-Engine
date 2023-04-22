#include "Core/Building/JupiterPCH.h"

#include "DemoGameApplication.h"

bool DemoGameApplication::Init()
{
    if (jpt::Application::Init())
    {
        std::cout << "Hello World Jupiter Demo Game" << std::endl;
    }
    return true;
}
