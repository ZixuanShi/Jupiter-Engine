// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_FilePath;

import jpt.File.Path;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_FilePath()
{

    return true;
}

export bool RunUnitTests_FilePath()
{
    JPT_ENSURE(UnitTest_FilePath());

    return true;
}
