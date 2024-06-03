// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_MathPractice;

import jpt.TypeDefs;
import jpt.Utilities;

import UnitTest_MathPractice_FindAngle2D;

// Main entry point for math practice. import any test modules above and add them to this function
export bool RunUnitTests_MathPractice()
{
	JPT_ENSURE(UnitTest_MathPractice_FindAngle2D());

	return true;
}