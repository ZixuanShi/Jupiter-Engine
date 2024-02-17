// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Sorting;

import jpt.Sorting;
import jpt.TypeDefs;
import jpt.DynamicArray;
import jpt.String;

template<typename T>
bool OverriddenComparator(const T& a, const T& b)
{
    JPT_LOG("Using Overridden Comparator");
    return a < b;
}

bool UnitTest_Sorting_Basic()
{
    jpt::DynamicArray<int32> array{ 2,3,1,0,4 };

    JPT_LOG(array);

    jpt::Sort(array);
    jpt::Sort(array, [](const int32& a, const int32& b) 
        {
            JPT_LOG("Using custom placeholder lambda");
			return a > b;
        });
    jpt::Sort(array, OverriddenComparator<int32>);

    auto lambda = [](const int32& a, const int32& b)
        {
            JPT_LOG("Using custom lambda");
            return a > b;
        };
    jpt::Sort(array, lambda);

    JPT_LOG(array);

    return true;
}

export bool RunUnitTests_Sorting()
{
    JPT_ENSURE(UnitTest_Sorting_Basic(), "UnitTest_Sorting_Basic Failed");

    return true;
}
