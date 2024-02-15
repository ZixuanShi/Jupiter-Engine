// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Byte;

import jpt.Byte;

bool UnitTest_Byte()
{
    return true;
}

export bool RunUnitTests_Byte()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Byte(), "UnitTest_Byte Failed");

    return true;
}
