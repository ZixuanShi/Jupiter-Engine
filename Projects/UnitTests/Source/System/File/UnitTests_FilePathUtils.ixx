// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_FileUtils;

import jpt.FilePath;
import jpt.FilePathUtils;
import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_FilePathUtils_GetSeparator()
{
#if IS_PLATFORM_WIN64 || IS_PLATFORM_XBOX
    JPT_ENSURE(jpt::GetSeparator<jpt::FilePath>() == L"\\");
#else 
	JPT_ENSURE(jpt::GetSeparator<jpt::FilePath>() == L"/");
#endif

    return true;
}


bool UnitTest_FilePathUtils_FixSeparators()
{
#if IS_PLATFORM_WIN64 || IS_PLATFORM_XBOX
    jpt::FilePath path = "C:/Users/<user_name>/Documents/UnitTests";
    jpt::FixSeparators(path);
    JPT_ENSURE(path == "C:\\Users\\<user_name>\\Documents\\UnitTests");
#else 
    jpt::FilePath path = "C:\\Users\\<user_name>\\Documents\\UnitTests";
	jpt::FixSeparators(path);
	JPT_ENSURE(path == "C:/Users/<user_name>/Documents/UnitTests");
#endif

    return true;
}

export bool RunUnitTests_FilePathUtils()
{
    JPT_ENSURE(UnitTest_FilePathUtils_FixSeparators());
    JPT_ENSURE(UnitTest_FilePathUtils_GetSeparator());

    return true;
}
