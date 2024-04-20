// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_FileUtils;

import jpt.FileUtils;
import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_FileUtils_FixSlashes()
{
    jpt::String path = "C:\\Users\\<user_name>\\Documents\\UnitTests";
    jpt::FixSlashes(path);
    JPT_ENSURE(path == "C:/Users/<user_name>/Documents/UnitTests");

    jpt::WString wpath = L"C:\\Users\\<user_name>\\Documents\\UnitTests";
    jpt::FixSlashes(wpath);
    JPT_ENSURE(wpath == L"C:/Users/<user_name>/Documents/UnitTests");

    return true;
}

export bool RunUnitTests_FileUtils()
{
    JPT_ENSURE(UnitTest_FileUtils_FixSlashes());

    return true;
}
