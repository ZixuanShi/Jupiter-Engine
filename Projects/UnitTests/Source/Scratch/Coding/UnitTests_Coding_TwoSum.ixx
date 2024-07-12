// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Coding_TwoSum;

import jpt.CoreModules;

static jpt::DynamicArray<size_t> TwoSum(const jpt::DynamicArray<int32>& nums, int32 target)
{
	jpt::HashMap<size_t, size_t> numToIndex;

	for (size_t i = 0; i < nums.Size(); ++i)
	{
		const int32 complement = target - nums[i];

		if (numToIndex.Has(complement))
		{
			return { numToIndex[complement], i };
		}

		numToIndex[nums[i]] = i;
	}

	return {};
}

export bool UnitTest_Coding_TwoSum()
{
	jpt::DynamicArray<int32> nums = { 2, 7, 11, 15 };
	int32 target = 9;
	jpt::DynamicArray<size_t> result = TwoSum(nums, target);
	JPT_ENSURE(result == jpt::DynamicArray<size_t>({ 0, 1 }));

	nums = { 3, 2, 4 };
	target = 6;
	result = TwoSum(nums, target);
	JPT_ENSURE(result == jpt::DynamicArray<size_t>({ 1, 2 }));

	nums = { 3, 3 };
	target = 6;
	result = TwoSum(nums, target);
	JPT_ENSURE(result == jpt::DynamicArray<size_t>({ 0, 1 }));

	return true;
}