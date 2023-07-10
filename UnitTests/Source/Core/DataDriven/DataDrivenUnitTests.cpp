#include "JupiterPCH.h"
#include "DataDrivenUnitTests.h"

#include "Core/DataDriven/JsonParser.h"

bool UnitTest_Json()
{
	jpt::optional<jpt::JsonFile> jsonFile = jpt::JsonFile::Load(jpt::EAssetDirectoryType::Engine, "Assets/Common/Data/TestJson.json");
	
	return true;
}

void RunDataDrivenUnitTests()
{
	JPT_RETURN_IF_LOG(!UnitTest_Json(), "UnitTest_Json failed");
}
