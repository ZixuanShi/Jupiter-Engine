#include "Core/Building/JupiterPCH.h"

#include "DemoGameApplication.h"

bool DemoGameApplication::Init()
{
    if (!jpt::Application::Init())
    {
        return false;
    }

    return true;
}
