// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_MathPractice;

import jpt.TypeDefs;
import jpt.Utilities;

import UnitTests_Math_Scratch;
import UnitTests_Math_IsLeftOfView;
import UnitTests_Math_FindAngle2D;
import UnitTests_Math_DistancePointToPlane3D;
import UnitTests_Math_MergeCloseVertices;

// Main entry point for math practice. import any test modules above and add them to this function
export bool RunUnitTests_MathPractice()
{
	JPT_ENSURE(UnitTests_Math_Scratch());
	JPT_ENSURE(UnitTests_Math_IsLeftOfView());
	JPT_ENSURE(UnitTests_Math_FindAngle2D());
	JPT_ENSURE(UnitTests_Math_DistancePointToPlane3D());
	JPT_ENSURE(UnitTests_Math_MergeCloseVertices());

	return true;
}