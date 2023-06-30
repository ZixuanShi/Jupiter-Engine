#include "JupiterPCH.h"
#include "JupiterUnitTestsApplication.h"

#include "ApplicationLayer/Windows/Window.h"
#include "Core/Algorithms/AlgorithmsUnitTests.h"
#include "Core/DataStructures/DataStructuresUnitTests.h"
#include "Core/Misc/MiscUnitTests.h"
#include "Core/System/SystemUnitTests.h"
#include "Core/DataDriven/DataDrivenUnitTests.h"

bool JupiterUnitTestsApplication::PreInit()
{
    JPT_RETURN_FALSE_IF_LOG(!Super::PreInit(), "Failed Super::PreInit()");

    m_pWindow->SetTitle(L"木星引擎 Jupiter Engine");

    return true;
}

bool JupiterUnitTestsApplication::Init()
{
    JPT_RETURN_FALSE_IF_LOG(!Super::Init(), "Failed Super::Init()");

    RunAlgorithmsUnitTests();
    RunDataStructuresUnitTests();
    RunMiscUnitTests();
    RunSystemUnitTests();
    RunDataDrivenUnitTests();

    return true;
}
