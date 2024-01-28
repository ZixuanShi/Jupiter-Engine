// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <vector>

export module UnitTests_DynamicArray;

import jpt.DynamicArray;

bool UnitTest_DynamicArray()
{
    std::vector<int32> v;


    return true;
}

export bool RunUnitTests_DynamicArray()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_DynamicArray(), "UnitTest_DynamicArray Failed");

    return true;
}
