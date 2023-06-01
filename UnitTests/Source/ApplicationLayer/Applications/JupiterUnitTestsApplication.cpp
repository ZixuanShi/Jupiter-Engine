#include "Core/Building/JupiterPCH.h"
#include "JupiterUnitTestsApplication.h"

#include "ApplicationLayer/Windows/Window.h"
#include "Core/Algorithms/AlgorithmsUnitTests.h"
#include "Core/DataStructures/DataStructuresUnitTests.h"
#include "Core/Types/TypesUnitTests.h"

bool JupiterUnitTestsApplication::PreInit()
{
    m_pWindow->SetTitle(L"Jupiter Unit Tests");

    RETURN_FALSE_IF_LOG(!Super::PreInit(), "Failed Super::PreInit()");

    return true;
}

bool JupiterUnitTestsApplication::Init()
{
    RETURN_FALSE_IF_LOG(!Super::Init(), "Failed Super::Init()");

    RunAlgorithmsUnitTests();
    RunDataStructuresUnitTests();
    RunTypesUnitTests();

    return true;
}
