
// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Coding;

import jpt.Utilities;

import UnitTests_Coding_Scratch;
import UnitTests_Coding_FindMedianSortedArrays;
import UnitTests_Coding_SubArraySum;
import UnitTests_Coding_IsPalindrome;
import UnitTests_Coding_TwoSum;
import UnitTests_Coding_RomanToInteger;
import UnitTests_Coding_MaxSubArray;
import UnitTests_Coding_DeleteDuplicatesFromList;
import UnitTests_Coding_MergeCloseVertices;

export bool RunUnitTests_Coding()
{
	JPT_ENSURE(UnitTest_Coding_Scratch());
	JPT_ENSURE(UnitTest_Coding_FindMedianSortedArrays());
	JPT_ENSURE(UnitTest_Coding_SubArraySum());
	JPT_ENSURE(UnitTest_Coding_IsPalindrome());
	JPT_ENSURE(UnitTest_Coding_TwoSum());
	JPT_ENSURE(UnitTest_Coding_RomanToInteger());
	JPT_ENSURE(UnitTest_Coding_MaxSubArray());
	JPT_ENSURE(UnitTest_Coding_DeleteDuplicatesFromList());
	JPT_ENSURE(UnitTest_Coding_MergeCloseVertices());

	return true;
}