// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_BinarySearch;

import jpt.BinarySearch;
import jpt.DynamicArray;
import jpt.StaticArray;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_BinarySearch_Fixed()
{
	//jpt::StaticArray<int32, 10> data = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    //jpt::BinarySearch

    return true;
}

export bool RunUnitTests_BinarySearch()
{
    JPT_ENSURE(UnitTest_BinarySearch_Fixed());

    return true;
}
