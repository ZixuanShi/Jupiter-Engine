// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Coding_FindMedianSortedArrays;

import jpt.TypeDefs;
import jpt.DynamicArray;
import jpt.Utilities;

// Given two sorted arrays nums1 and nums2 of size m and n respectively, return the median of the two sorted arrays.
// The overall run time complexity should be O(log(m+n)).
double FindMedianSortedArrays(const jpt::DynamicArray<int32>& nums1, const jpt::DynamicArray<int32>& nums2)
{
	// Merge the two arrays
	JPT_IGNORE(nums1, nums2);

	return 0.0f;
}

export bool UnitTest_Coding_FindMedianSortedArrays()
{
	//JPT_ENSURE(FindMedianSortedArrays({ 1, 3 }, { 2 }) == 2.0);
	//JPT_ENSURE(FindMedianSortedArrays({ 1, 2 }, { 3, 4 }) == 2.5);

	return true;
}