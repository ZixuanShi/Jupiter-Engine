// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"
#include <algorithm>

export module UnitTests_Coding_FindMedianSortedArrays;

import jpt.Sorting;
import jpt.TypeDefs;
import jpt.Limits;
import jpt.DynamicArray;
import jpt.SortedMap;
import jpt.Utilities;
import jpt.Math;

double FindMedianSortedArrays_BruteForce(const jpt::DynamicArray<int32>& nums1, const jpt::DynamicArray<int32>& nums2)
{
	// Merge and return the median
	jpt::DynamicArray<int32> merged;
	merged.Reserve(nums1.Count() + nums2.Count());

	for (int32 i : nums1)
	{
		merged.AddBack(i);
	}
	for (int32 i : nums2)
	{
		merged.AddBack(i);
	}

	jpt::Sort(merged);

	if (merged.Count() % 2 == 0)
	{
		return (merged[merged.Count() / 2 - 1] + merged[merged.Count() / 2]) / 2.0;
	}
	else
	{
		return merged[merged.Count() / 2];
	}
}

// Given two sorted arrays nums1 and nums2 of size m and n respectively, return the median of the two sorted arrays.
// The overall run time complexity should be O(log(m+n)).
double FindMedianSortedArrays_BinarySearch(const jpt::DynamicArray<int32>& nums1, const jpt::DynamicArray<int32>& nums2)
{
	// Ensure nums1 is the smaller array
	if (nums1.Count() > nums2.Count())
	{
		return FindMedianSortedArrays_BinarySearch(nums2, nums1);
	}

	const size_t nums1Count = nums1.Count();
	const size_t nums2Count = nums2.Count();
	const size_t totalCount = nums1Count + nums2Count;
	const size_t partition = (totalCount + 1) / 2;

	// Edge case: If nums1 is empty, return the median of nums2
	if (nums1Count == 0)
	{
		if (totalCount % 2 == 0)
		{
			return (nums2[partition - 1] + nums2[partition]) / 2.0;
		}
		else
		{
			return nums2[partition - 1];
		}
	}
	if (nums2Count == 0)
	{
		if (totalCount % 2 == 0)
		{
			return (nums1[partition - 1] + nums1[partition]) / 2.0;
		}
		else
		{
			return nums1[partition - 1];
		}
	}

	size_t left = 0;
	size_t right = nums1Count;

	while (left <= right)
	{
		const size_t partition1 = (left + right) / 2;
		const size_t partition2 = partition - partition1;

		const int32 maxLeft1 = partition1 == 0 ? jpt::Limits<int32>::kMin : nums1[partition1 - 1];
		const int32 minRight1 = partition1 == nums1Count ? jpt::Limits<int32>::kMax : nums1[partition1];
		const int32 maxLeft2 = partition2 == 0 ? jpt::Limits<int32>::kMin : nums2[partition2 - 1];
		const int32 minRight2 = partition2 == nums2Count ? jpt::Limits<int32>::kMax : nums2[partition2];

		if (maxLeft1 <= minRight2 && maxLeft2 <= minRight1)
		{
			if (totalCount % 2 == 0)
			{
				return (jpt::Max(maxLeft1, maxLeft2) + jpt::Min(minRight1, minRight2)) / 2.0;
			}
			else
			{
				return jpt::Max(maxLeft1, maxLeft2);
			}
		}
		else if (maxLeft1 > minRight2)
		{
			right = partition1 - 1;
		}
		else
		{
			left = partition1 + 1;
		}
	}

	return 0.0f;
}

export bool UnitTest_Coding_FindMedianSortedArrays()
{
	JPT_ENSURE(FindMedianSortedArrays_BruteForce({ 1, 3 }, { 2 }) == 2.0);
	JPT_ENSURE(FindMedianSortedArrays_BruteForce({ 1, 2 }, { 3, 4 }) == 2.5);
	JPT_ENSURE(FindMedianSortedArrays_BruteForce({ 1, 3 }, { 2, 7 }) == 2.5);

	JPT_ENSURE(FindMedianSortedArrays_BinarySearch({ 1, 3 }, { 2 }) == 2.0);
	JPT_ENSURE(FindMedianSortedArrays_BinarySearch({ 1, 2 }, { 3, 4 }) == 2.5);
	JPT_ENSURE(FindMedianSortedArrays_BinarySearch({ 1, 3 }, { 2, 7 }) == 2.5);

	return true;
}