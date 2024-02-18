// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Sorting;

import jpt.Sorting;
import jpt.TypeDefs;
import jpt.DynamicArray;
import jpt.String;

bool UnitTest_Sorting_Basic()
{
    jpt::DynamicArray<int32> array{ 2,3,1,0,4 };

    JPT_LOG(array);

    //jpt::Sort(array);

    JPT_LOG(array);

    return true;
}

export bool RunUnitTests_Sorting()
{
    JPT_ENSURE(UnitTest_Sorting_Basic(), "UnitTest_Sorting_Basic Failed");

    return true;
}
