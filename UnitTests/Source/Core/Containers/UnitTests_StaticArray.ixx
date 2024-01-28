// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_StaticArray;

import jpt.StaticArray;

bool UnitTest_StaticArray()
{
    jpt::StaticArray<int32, 5> staticArray{ 0,1,2,3,4 };



    return true;
}

export bool RunUnitTests_StaticArray()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StaticArray(), "UnitTest_StaticArray Failed");

    return true;
}
