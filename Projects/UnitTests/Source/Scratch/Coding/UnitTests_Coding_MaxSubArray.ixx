// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Coding_MaxSubArray;

import jpt.Math;
import jpt.TypeDefs;
import jpt.DynamicArray;
import jpt.Utilities;

int32 MaxSubArray(const jpt::DynamicArray<int32>& nums)
{
	int32 maxSum = nums[0];
	int32 currentSum = nums[0];

	for (size_t i = 1; i < nums.Count(); ++i)
	{
		currentSum = jpt::Max(nums[i], currentSum + nums[i]);
		maxSum = jpt::Max(maxSum, currentSum);
	}

	return maxSum;
}

export bool UnitTests_Coding_MaxSubArray()
{
	JPT_ENSURE(MaxSubArray({ -2, 1, -3, 4, -1, 2, 1, -5, 4 }) == 6);
	JPT_ENSURE(MaxSubArray({ 1 }) == 1);
	JPT_ENSURE(MaxSubArray({ 5, 4, -1, 7, 8 }) == 23);

	return true;
}