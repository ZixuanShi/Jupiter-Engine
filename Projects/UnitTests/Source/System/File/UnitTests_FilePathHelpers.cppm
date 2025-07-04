// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_File.Path.Helpers;

import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;

import jpt.FilePath;
import jpt.FilePathHelpers;

bool UnitTests_FilePathHelpers_Getters()
{
    //JPT_LOG(jpt::File::GetClientPath());
    //JPT_LOG(jpt::File::GetEnginePath());

    return true;
}

bool UnitTests_FilePathHelpers_GetSeparator()
{
#if IS_PLATFORM_WINDOWS || IS_PLATFORM_XBOX
    JPT_ENSURE(jpt::File::GetSeparator<jpt::File::Path>() == L"\\");
#else 
    JPT_ENSURE(jpt::File::GetSeparator<jpt::File::Path>() == L"/");
#endif

    return true;
}

bool UnitTests_FilePathHelpers_FixSeparators()
{
#if IS_PLATFORM_WINDOWS || IS_PLATFORM_XBOX
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
    JPT_ENSURE(UnitTests_FilePathHelpers_Getters());
    JPT_ENSURE(UnitTests_FilePathHelpers_FixSeparators());
    JPT_ENSURE(UnitTests_FilePathHelpers_GetSeparator());

    return true;
}
