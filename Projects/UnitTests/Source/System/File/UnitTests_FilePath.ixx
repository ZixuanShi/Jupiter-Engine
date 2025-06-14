// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_FilePath;

import jpt.FilePath;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTests_FilePath()
{

    return true;
}

export bool RunUnitTests_FilePath()
{
    JPT_ENSURE(UnitTests_FilePath());

    return true;
}
