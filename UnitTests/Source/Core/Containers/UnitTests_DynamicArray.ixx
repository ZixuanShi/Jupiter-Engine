// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <vector>

export module UnitTests_DynamicArray;

import jpt.TypeDefs;
import jpt.DynamicArray;

bool UnitTest_DynamicArray_Trivial()
{
    jpt::DynamicArray<int32> dynamicArray{ 0,1,2,3,4 };



    return true;
}

void v()
{
    std::vector<int32> v;
}

export bool RunUnitTests_DynamicArray()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_DynamicArray_Trivial(), "UnitTest_DynamicArray_Trivial Failed");

    return true;
}
