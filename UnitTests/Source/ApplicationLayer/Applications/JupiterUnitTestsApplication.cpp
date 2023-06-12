#include "JupiterPCH.h"
#include "JupiterUnitTestsApplication.h"

#include "ApplicationLayer/Windows/Window.h"
#include "Core/Algorithms/AlgorithmsUnitTests.h"
#include "Core/DataStructures/DataStructuresUnitTests.h"
#include "Core/Types/TypesUnitTests.h"
#include "Core/System/SystemUnitTests.h"

bool JupiterUnitTestsApplication::PreInit()
{
    m_pWindow->SetTitle(L"木星引擎 Jupiter Engine");

    JPT_RETURN_FALSE_IF_LOG(!Super::PreInit(), "Failed Super::PreInit()");

    return true;
}

bool JupiterUnitTestsApplication::Init()
{
    JPT_RETURN_FALSE_IF_LOG(!Super::Init(), "Failed Super::Init()");

    RunAlgorithmsUnitTests();
    RunDataStructuresUnitTests();
    RunTypesUnitTests();
    RunSystemUnitTests();

    return true;
}
