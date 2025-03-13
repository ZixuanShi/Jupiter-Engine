// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Coding_SubArraySum;

import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.HashMap;
import jpt.Utilities;

/** Given an array of integers nums and an integer k, return the total number of subarrays whose sum equals to k.
    A subarray is a contiguous non-empty sequence of elements within an array. */
int SubArraySum(const jpt::DynamicArray<int32>& nums, int k)
{
    jpt::HashMap<int32, int32> prefixSum;
    prefixSum[0] = 1;

    int sum = 0;
    int count = 0;

    for (int32 i : nums)
    {
        sum += i;

        if (prefixSum.Has(sum - k))
        {
            count += prefixSum[sum - k];
        }

        ++prefixSum[sum];
    }

    return count;
}

export bool UnitTests_Coding_SubArraySum()
{
    JPT_ENSURE(SubArraySum({ 1,1,1 }, 2) == 2);
    JPT_ENSURE(SubArraySum({ 1,2,3 }, 3) == 2);

    return true;
}