﻿
// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

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
import UnitTests_Coding_BuildPalindromeWithRemains;
import UnitTests_Coding_MaximumVisitableCities;

export bool RunUnitTests_Coding()
{
	JPT_ENSURE(UnitTests_Coding_Scratch());
	JPT_ENSURE(UnitTests_Coding_FindMedianSortedArrays());
	JPT_ENSURE(UnitTests_Coding_SubArraySum());
	JPT_ENSURE(UnitTests_Coding_IsPalindrome());
	JPT_ENSURE(UnitTests_Coding_TwoSum());
	JPT_ENSURE(UnitTests_Coding_RomanToInteger());
	JPT_ENSURE(UnitTests_Coding_MaxSubArray());
	JPT_ENSURE(UnitTests_Coding_DeleteDuplicatesFromList());
	JPT_ENSURE(UnitTests_Coding_MergeCloseVertices());
	JPT_ENSURE(UnitTests_Coding_BuildPalindromeWithRemains());
	JPT_ENSURE(UnitTests_Coding_MaximumVisitableCities());

	// https://claude.ai/chat/588f655f-8a30-4819-9812-e892ff528625

	return true;
}