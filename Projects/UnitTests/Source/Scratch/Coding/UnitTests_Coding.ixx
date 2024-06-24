// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Coding;

import jpt.Utilities;

import UnitTests_Coding_FindMedianSortedArrays;
import UnitTests_Coding_SubArraySum;
import UnitTests_Coding_IsPalindrome;
import UnitTests_Coding_TwoSum;
import UnitTests_Coding_RomanToInteger;

export bool RunUnitTests_Coding()
{
	JPT_ENSURE(UnitTest_Coding_FindMedianSortedArrays());
	JPT_ENSURE(UnitTest_Coding_SubArraySum());
	JPT_ENSURE(UnitTest_Coding_IsPalindrome());
	JPT_ENSURE(UnitTest_Coding_TwoSum());
	JPT_ENSURE(UnitTest_Coding_RomanToInteger());

	return true;
}