// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Coding;

import jpt.CoreModules;

int SubArraySum(const jpt::DynamicArray<int32>& nums, int k)
{
	jpt::HashMap<int32, int32> prefixSum;
	prefixSum[0] = 1;

	int sum = 0;
	int count = 0;

	for (int32 i : nums)
	{
		sum += i;

		if (prefixSum.Contains(sum - k))
		{
			count += prefixSum[sum - k];
		}

		++prefixSum[sum];
	}

	return count;
}

export bool RunUnitTests_Coding()
{

	return true;
}