// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"
#include <time.h>

export module UnitTests_Sorting;

import jpt.Sorting;
import jpt.TypeDefs;
import jpt.DynamicArray;
import jpt.String;
import jpt.RandomNumberGenerator;

bool UnitTest_Sorting_Basic()
{
    jpt::DynamicArray<int32> array{ 2,3,1,0,4 };

    jpt::Sort(array);
    JPT_ENSURE((array == jpt::DynamicArray<int32>{ 0, 1, 2, 3, 4 }));

    return true;
}

bool UnitTest_Sorting_Random()
{
    static constexpr size_t kArraySize = 1'00;

    jpt::DynamicArray<int32> array;
    array.Resize(kArraySize);

    jpt::RNG local;
    local.SetSeed(79726);

    for (size_t i = 0; i < kArraySize; ++i)
	{
        array[i] = local.RandInMax(100);
	}

    jpt::Sort(array, [](int32 a, int32 b) { return a > b; });

    for (size_t i = 0; i < kArraySize; ++i)
    {
        if (i + 1 <= kArraySize - 1)
        {
            JPT_ENSURE(array[i] >= array[i + 1]);
        }
    }

    return true;
}

export bool RunUnitTests_Sorting()
{
    JPT_ENSURE(UnitTest_Sorting_Basic());
    JPT_ENSURE(UnitTest_Sorting_Random());

    return true;
}
