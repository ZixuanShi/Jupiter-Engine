// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Byte;

import jpt.Byte;
import jpt.Utilities;

bool UnitTests_Byte()
{
    return true;
}

export bool RunUnitTests_Byte()
{
    JPT_ENSURE(UnitTests_Byte());

    return true;
}
