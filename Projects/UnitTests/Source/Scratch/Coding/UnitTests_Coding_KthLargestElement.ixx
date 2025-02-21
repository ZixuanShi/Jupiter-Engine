// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module UnitTests_Coding_KthLargestElement;

import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Utilities;

int32 QuickSelect(const jpt::DynamicArray<int32>& input, size_t k)
{
	JPT_ASSERT(input.Count() >= k);

	const Index pivotIdx = input.Count() / 2;
	const int32 pivot = input[pivotIdx];

	jpt::DynamicArray<int32> left;
	jpt::DynamicArray<int32> mid;
	jpt::DynamicArray<int32> right;

	for (int val : input)
	{
		if (val > pivot)
		{
			left.EmplaceBack(val);
		}
		else if (val < pivot)
		{
			right.EmplaceBack(val);
		}
		else
		{
			mid.EmplaceBack(val);
		}
	}

	if (k <= left.Count())
	{
		return QuickSelect(left, k);
	}

	if (left.Count() + mid.Count() < k)
	{
		return QuickSelect(right, k - left.Count() - mid.Count());
	}

	return pivot;
}

int32 FindKthLargestElement(const jpt::DynamicArray<int32>& input, size_t k)
{
	return QuickSelect(input, k);
}

export bool UnitTests_Coding_KthLargestElement()
{
	// https://www.geeksforgeeks.org/kth-largest-element-in-an-array/

	const jpt::DynamicArray<int32> input = { 5,6,4,1,2,3 };

	JPT_ENSURE(FindKthLargestElement(input, 3) == 4);
	JPT_ENSURE(FindKthLargestElement(input, 1) == 6);
	JPT_ENSURE(FindKthLargestElement(input, 6) == 1);

	return true;
}