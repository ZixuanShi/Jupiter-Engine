#include "Core/Building/JupiterPCH.h"

#include "DemoGameApplication.h"

DemoGameApplication::DemoGameApplication()
{
}

bool DemoGameApplication::Init()
{
    if (!jpt::Application::Init())
    {
        return false;
    }

    return true;
}
