#include "Core/Building/JupiterPCH.h"
#include "JupiterDemoGameApplication.h"

#include "ApplicationLayer/Windows/Window.h"

bool JupiterDemoGameApplication::PreInit()
{
    m_pWindow->SetTitle(L"Hello World Jupiter Engine");

    RETURN_FALSE_IF(!Super::PreInit());

    return true;
}

bool JupiterDemoGameApplication::Init()
{
    RETURN_FALSE_IF(!Super::Init());

    return true;
}
