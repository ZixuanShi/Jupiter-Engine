#include "Core/Building/JupiterPCH.h"

#include "DemoGameApplication.h"

bool DemoGameApplication::Init()
{
    if (jpt::Application::Init())
    {
        JPT_LOG("Hello World Jupiter Demo Game");
    }

    return true;
}
