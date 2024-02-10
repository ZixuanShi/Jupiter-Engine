// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <cstddef>

export module UnitTests_Byte;

import jpt.Byte;

bool UnitTest_Byte()
{
    return true;
}

bool UnitTest_stdByte()
{
    std::byte b = std::byte(0);
    JPT_IGNORE(b);

    return true;
}

export bool RunUnitTests_Byte()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Byte(), "UnitTest_Byte Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_stdByte(), "UnitTest_stdByte Failed");

    return true;
}
