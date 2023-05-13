#include "Core/Building/JupiterPCH.h"
#include "JupiterUnitTestsApplication.h"

#include "Core/Algorithms/AlgorithmsUnitTests.h"
#include "Core/DataStructures/DataStructuresUnitTests.h"
#include "Core/Types/TypesUnitTests.h"

bool JupiterUnitTestsApplication::Init()
{
    if (!jpt::Application::Init())
    {
        return false;
    }
    
    RunAlgorithmsUnitTests();
    RunDataStructuresUnitTests();
    RunTypesUnitTests();

    return true;
}
