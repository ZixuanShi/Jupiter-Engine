// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_StaticArray;

import jpt.CoreModules;

bool UnitTest_StaticArray()
{

    return true;
}

export bool RunUnitTests_StaticArray()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StaticArray(), "UnitTest_StaticArray Failed");

    return true;
}