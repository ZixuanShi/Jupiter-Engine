#include "JupiterPCH.h"
#include "DataDrivenUnitTests.h"

bool UnitTest_Json()
{
	jpt::JsonFile jsonFile = jpt::LoadJsonFile("Assets/Engine/Data/TestJson.json");
	return true;
}

void RunDataDrivenUnitTests()
{
	JPT_RETURN_IF_LOG(!UnitTest_Json(), "UnitTest_Json failed");
}
