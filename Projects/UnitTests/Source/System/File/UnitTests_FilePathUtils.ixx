// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_FileUtils;

import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;

import jpt.File.Path;

bool UnitTest_FilePathUtils_Getters()
{
    //JPT_LOG(jpt::File::GetClientPath());
    //JPT_LOG(jpt::File::GetEnginePath());

    return true;
}

bool UnitTest_FilePathUtils_GetSeparator()
{
#if IS_PLATFORM_WIN64 || IS_PLATFORM_XBOX
    JPT_ENSURE(jpt::File::GetSeparator<jpt::File::Path>() == L"\\");
#else 
	JPT_ENSURE(jpt::File::GetSeparator<jpt::File::Path>() == L"/");
#endif

    return true;
}

bool UnitTest_FilePathUtils_FixSeparators()
{
#if IS_PLATFORM_WIN64 || IS_PLATFORM_XBOX
    jpt::File::Path path = "C:/Users/<user_name>/Documents/UnitTests";
    jpt::File::FixSeparators(path);
    JPT_ENSURE(path == "C:\\Users\\<user_name>\\Documents\\UnitTests");
#else 
    jpt::File::Path path = "C:\\Users\\<user_name>\\Documents\\UnitTests";
	jpt::File::FixSeparators(path);
	JPT_ENSURE(path == "C:/Users/<user_name>/Documents/UnitTests");
#endif

    return true;
}

export bool RunUnitTests_FilePathUtils()
{
    JPT_ENSURE(UnitTest_FilePathUtils_Getters());
    JPT_ENSURE(UnitTest_FilePathUtils_FixSeparators());
    JPT_ENSURE(UnitTest_FilePathUtils_GetSeparator());

    return true;
}
