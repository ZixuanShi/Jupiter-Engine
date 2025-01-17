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

// @return: All possible combination of numbers inside nums that add up to target.
static jpt::DynamicArray<jpt::DynamicArray<int32>> CombinationSum(const jpt::DynamicArray<int32>& candidates, int32 target)
{
}

export bool UnitTests_Coding_TwoSum()
{
	jpt::DynamicArray<int32> nums;
	int32 target = 0;

	nums = { 1,2,3,4,5 };
	target = 6;
	JPT_LOG(TwoSum(nums, target));

	//-------------------------------------------------

	return true;
}