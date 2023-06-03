#include "Core/Building/JupiterPCH.h"
#include "JupiterUnitTestsApplication.h"

#include "ApplicationLayer/Windows/Window.h"
#include "Core/Algorithms/AlgorithmsUnitTests.h"
#include "Core/DataStructures/DataStructuresUnitTests.h"
#include "Core/Types/TypesUnitTests.h"

bool JupiterUnitTestsApplication::PreInit()
{
    m_pWindow->SetTitle(L"恭喜木星引擎支持中文");

    JPT_RETURN_FALSE_IF_LOG(!Super::PreInit(), "Failed Super::PreInit()");

    return true;
}

bool JupiterUnitTestsApplication::Init()
{
    JPT_RETURN_FALSE_IF_LOG(!Super::Init(), "Failed Super::Init()");

    RunAlgorithmsUnitTests();
    RunDataStructuresUnitTests();
    RunTypesUnitTests();

    return true;
}
