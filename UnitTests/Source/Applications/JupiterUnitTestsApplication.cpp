#include "Core/Building/JupiterPCH.h"
#include "JupiterUnitTestsApplication.h"

#include "ApplicationLayer/Windows/Window.h"
#include "Core/Algorithms/AlgorithmsUnitTests.h"
#include "Core/DataStructures/DataStructuresUnitTests.h"
#include "Core/Types/TypesUnitTests.h"

bool JupiterUnitTestsApplication::PreInit()
{
    m_pWindow->SetTitle(L"Jupiter Unit Tests");

    if (!Super::PreInit())
    {
        return false;
    }

    return true;
}

bool JupiterUnitTestsApplication::Init()
{
    if (!Super::Init())
    {
        return false;
    }
    
    RunAlgorithmsUnitTests();
    RunDataStructuresUnitTests();
    RunTypesUnitTests();

    return true;
}
