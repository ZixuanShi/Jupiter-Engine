// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"
#include <time.h>

export module UnitTests_Sorting;

import jpt.Function;
import jpt.Sorting;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.DynamicArray;
import jpt.String;
import jpt.RandomNumberGenerator;

bool UnitTest_Sorting_Basic()
{
    jpt::DynamicArray<int32> arr{ 2,3,1,0,4 };

    jpt::Sort(arr);
    JPT_ENSURE((arr == jpt::DynamicArray<int32>{ 0, 1, 2, 3, 4 }));

    return true;
}

bool UnitTest_Sorting_Random()
{
    static constexpr size_t kArraySize = 10;

    jpt::DynamicArray<int32> arr;
    arr.Resize(kArraySize);

    jpt::RNG local;
    local.SetSeed(79726);

    for (size_t i = 0; i < kArraySize; ++i)
	{
        arr[i] = local.RandInMax(100);
	}

    jpt::Sort(arr, [](int32 a, int32 b) { return a > b; });

    for (size_t i = 0; i < kArraySize; ++i)
    {
        if (i + 1 <= kArraySize - 1)
        {
            JPT_ENSURE(arr[i] >= arr[i + 1]);
        }
    }

    jpt::Sort(arr, [](int32 a, int32 b) { return a < b; });

    for (size_t i = 0; i < kArraySize; ++i)
    {
        if (i + 1 <= kArraySize - 1)
        {
            JPT_ENSURE(arr[i] <= arr[i + 1]);
        }
    }

    return true;
}

bool UnitTest_Sorting_StackArray()
{
    int32 arr[] = { 5,2,4,6,1,3 };

    jpt::Sort(arr, 6);
    JPT_ENSURE((arr[0] == 1 && 
                arr[1] == 2 && 
                arr[2] == 3 && 
                arr[3] == 4 && 
                arr[4] == 5 && 
                arr[5] == 6));

    jpt::Sort(arr, 6, [](int32 a, int32 b) { return a > b; });
    JPT_ENSURE((arr[0] == 6 &&
				arr[1] == 5 &&
				arr[2] == 4 &&
				arr[3] == 3 &&
				arr[4] == 2 &&
				arr[5] == 1));

    jpt::Function<bool(int32, int32)> compare = [](int32 a, int32 b) { return a < b; };
    jpt::Sort(arr, 6, compare);
    JPT_ENSURE((arr[0] == 1 &&
                arr[1] == 2 &&
                arr[2] == 3 &&
                arr[3] == 4 &&
                arr[4] == 5 &&
                arr[5] == 6));

    compare = [](int32 a, int32 b) { return a > b; };
    jpt::Sort(arr, 6, compare);
    JPT_ENSURE((arr[0] == 6 &&
		        arr[1] == 5 &&
		        arr[2] == 4 &&
		        arr[3] == 3 &&
		        arr[4] == 2 &&
		        arr[5] == 1));
    
    return true;
}

struct NonTrivial
{
    int32 m_int;
    jpt::String m_string;

    NonTrivial() = default;
    NonTrivial(int32 value, const jpt::String& string) : m_int(value), m_string(string) {}
    bool operator<(const NonTrivial& other) const { return m_int < other.m_int; }
    bool operator>(const NonTrivial& other) const { return m_int > other.m_int; }
};

bool UnitTest_Sorting_NonTrivial()
{
	jpt::DynamicArray<NonTrivial> arr;
	arr.EmplaceBack(2, "two");
	arr.EmplaceBack(3, "three");
	arr.EmplaceBack(1, "one");
	arr.EmplaceBack(0, "zero");
	arr.EmplaceBack(4, "four");

	jpt::Sort(arr);
	JPT_ENSURE((arr[0].m_int == 0 && arr[0].m_string == "zero" &&
        		arr[1].m_int == 1 && arr[1].m_string == "one" &&
        		arr[2].m_int == 2 && arr[2].m_string == "two" &&
        		arr[3].m_int == 3 && arr[3].m_string == "three" &&
        		arr[4].m_int == 4 && arr[4].m_string == "four"));

    jpt::Sort(arr, [](const NonTrivial& a, const NonTrivial& b) { return a > b; });
    JPT_ENSURE((arr[0].m_int == 4 && arr[0].m_string == "four" &&
        		arr[1].m_int == 3 && arr[1].m_string == "three" &&
        		arr[2].m_int == 2 && arr[2].m_string == "two" &&
        		arr[3].m_int == 1 && arr[3].m_string == "one" &&
        		arr[4].m_int == 0 && arr[4].m_string == "zero"));

    jpt::Function<bool(const NonTrivial&, const NonTrivial&)> compare = [](const NonTrivial& a, const NonTrivial& b) { return a < b; };
    jpt::Sort(arr, compare);
    JPT_ENSURE((arr[0].m_int == 0 && arr[0].m_string == "zero" &&
        		arr[1].m_int == 1 && arr[1].m_string == "one" &&
        		arr[2].m_int == 2 && arr[2].m_string == "two" &&
        		arr[3].m_int == 3 && arr[3].m_string == "three" &&
        		arr[4].m_int == 4 && arr[4].m_string == "four"));

    compare = [](const NonTrivial& a, const NonTrivial& b) { return a > b; };
    jpt::Sort(arr, compare);
    JPT_ENSURE((arr[0].m_int == 4 && arr[0].m_string == "four" &&
        		arr[1].m_int == 3 && arr[1].m_string == "three" &&
        		arr[2].m_int == 2 && arr[2].m_string == "two" &&
        		arr[3].m_int == 1 && arr[3].m_string == "one" &&
        		arr[4].m_int == 0 && arr[4].m_string == "zero"));

	return true;
}

export bool RunUnitTests_Sorting()
{
    JPT_ENSURE(UnitTest_Sorting_Basic());
    JPT_ENSURE(UnitTest_Sorting_Random());
    JPT_ENSURE(UnitTest_Sorting_StackArray());
    JPT_ENSURE(UnitTest_Sorting_NonTrivial());

    return true;
}
