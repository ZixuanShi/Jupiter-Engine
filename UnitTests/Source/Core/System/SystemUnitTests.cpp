// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "SystemUnitTests.h"

bool UnitTest_FilePathUtils()
{
	jpt::string dir = jpt::GetEngineDir();
	JPT_RETURN_FALSE_IF_LOG(dir != "C:/Program Files/Jupiter Technologies/Jupiter Engine/", "");
	dir = jpt::GetEngineAssetPath("Dummy.txt");
	JPT_RETURN_FALSE_IF_LOG(dir != "C:/Program Files/Jupiter Technologies/Jupiter Engine/Assets/Dummy.txt", "");

	jpt::wstring dirW = jpt::FixedSlashes(jpt::GetEngineDirW());
	JPT_RETURN_FALSE_IF_LOG(dirW != L"C:/Program Files/Jupiter Technologies/Jupiter Engine/", "");

	return true;
}

bool UnitTest_File()
{
	//jpt::File file("C:/Program Files/Jupiter Technologies/Jupiter Engine/UnitTests/Scripts/GenerateProjects.bat");
	//JPT_RETURN_FALSE_IF_LOG(file.GetFolderPath() != "C:/Program Files/Jupiter Technologies/Jupiter Engine/UnitTests/Scripts/", "");
	//JPT_RETURN_FALSE_IF_LOG(file.GetName() != "GenerateProjects", "");
	//JPT_RETURN_FALSE_IF_LOG(file.GetExtension() != "bat", "");


	return true;
}

void RunSystemUnitTests()
{
	JPT_RETURN_IF_LOG(!UnitTest_FilePathUtils(), "UnitTest_FilePathUtils failed");
	JPT_RETURN_IF_LOG(!UnitTest_File(), "UnitTest_File failed");
}
