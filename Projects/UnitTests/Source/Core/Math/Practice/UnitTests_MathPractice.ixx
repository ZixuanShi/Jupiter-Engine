// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_MathPractice;

import jpt.TypeDefs;
import jpt.Utilities;

// Vector2
import UnitTests_MathPractice_FindAngle2D;
import UnitTests_MathPractice_FindAngleAndDir2D;

// Vector3
import UnitTest_MathPractice_FindPerpendicularVector;

// Main entry point for math practice. import any test modules above and add them to this function
export bool RunUnitTests_MathPractice()
{
	// Vector2
	JPT_ENSURE(UnitTest_MathPractice_FindAngle2D());
	JPT_ENSURE(UnitTest_MathPractice_FindAngleAndDir2D());

	// Vector3
	JPT_ENSURE(UnitTest_MathPractice_FindPerpendicularVector());

	return true;
}