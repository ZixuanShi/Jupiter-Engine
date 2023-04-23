#include "Core/Building/JupiterPCH.h"

#include "DemoGameApplication.h"

DemoGameApplication::DemoGameApplication()
{
    JPT_LOG("Constructed demo game application");
}

bool DemoGameApplication::Init()
{
    if (jpt::Application::Init())
    {
        JPT_LOG("Hello World Jupiter Demo Game");
    }

    return true;
}
