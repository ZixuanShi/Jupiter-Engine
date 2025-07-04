// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Coding_TwoSum;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.DynamicArray;
import jpt.Pair;
import jpt.HashMap;

// @return: All the pairs of numbers inside nums that add up to target.
// i.e. nums = {1,2,3,4,5}, target = 6
// return = {{1,5}, {2,4}}
static jpt::DynamicArray<jpt::Pair<int32, int32>> TwoSum(const jpt::DynamicArray<int32>& nums, int32 target)
{
    jpt::DynamicArray<jpt::Pair<int32, int32>> result;
    jpt::HashMap<int32, int32> map;
    jpt::HashMap<int32, uint32> occurences;

    for (int32 i : nums)
    {
        map.Add(i, target - i);
        ++occurences[i];
    }

    for (const auto&[key, value] : map)
    {
        if (!map.Has(value))
        {
            continue;
        }

        if (value == key && occurences[key] == 1)
        {
            continue;
        }

        result.Add({ key, value });
    }

    return result;
}

void Sum(const jpt::DynamicArray<int32>& candidates, int32 target, size_t i, jpt::DynamicArray<jpt::DynamicArray<int32>>& result, jpt::DynamicArray<int32>& combination)
{
    if (target == 0)
    {
        result.Add(combination);
        return;
    }
    for (size_t j = i; j < candidates.Count(); ++j)
    {
        if (target < candidates[j])
        {
            continue;
        }

        combination.Add(candidates[j]);
        Sum(candidates, target - candidates[j], j, result, combination);
        combination.Pop();
    }
}

// @return: All possible combination of numbers inside nums that add up to target.
static jpt::DynamicArray<jpt::DynamicArray<int32>> CombinationSum(const jpt::DynamicArray<int32>& candidates, int32 target)
{
    jpt::DynamicArray<jpt::DynamicArray<int32>> result;
    jpt::DynamicArray<int32> combination;
    Sum(candidates, target, 0, result, combination);
    return result;
}

export bool UnitTests_Coding_TwoSum()
{
    jpt::DynamicArray<int32> nums;
    int32 target = 0;
    jpt::DynamicArray<jpt::Pair<int32, int32>> result;

    nums = { 1,2,3,4,5 };
    target = 6;
    result = { { 5, 1 }, { 4, 2 }, { 1, 5 }, { 2, 4 } };
    JPT_ENSURE(TwoSum(nums, target) == result);

    //-------------------------------------------------
    jpt::DynamicArray<jpt::DynamicArray<int32>> result2;

    nums = { 2,3,5 };
    target = 8;
    result2 = { { 2, 2, 2, 2 }, { 2, 3, 3 }, { 3, 5 } };
    JPT_ENSURE(CombinationSum(nums, target) == result2);

    return true;
}