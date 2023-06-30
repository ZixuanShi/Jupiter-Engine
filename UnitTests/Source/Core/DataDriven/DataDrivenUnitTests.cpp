#include "JupiterPCH.h"
#include "DataDrivenUnitTests.h"

bool UnitTest_Json()
{
	jpt::optional<jpt::JsonFile> jsonFile = jpt::JsonFile::Load("Assets/Data/TestJson.json");

	return true;
}

void RunDataDrivenUnitTests()
{
	JPT_RETURN_IF_LOG(!UnitTest_Json(), "UnitTest_Json failed");
}
