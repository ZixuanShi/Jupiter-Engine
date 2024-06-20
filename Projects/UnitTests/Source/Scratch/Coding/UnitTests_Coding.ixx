// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Coding;

import jpt.Utilities;

import UnitTests_Coding_FindMedianSortedArrays;
import UnitTests_Coding_SubArraySum;

export bool RunUnitTests_Coding()
{
	JPT_ENSURE(UnitTest_Coding_FindMedianSortedArrays());
	JPT_ENSURE(UnitTest_Coding_SubArraySum());

	return true;
}