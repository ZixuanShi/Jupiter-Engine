// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_MathPractice;

import jpt.TypeDefs;
import jpt.Utilities;

import UnitTests_MathPractice_FindAngle2D;
import UnitTests_MathPractice_FindAngleAndDir2D;
import UnitTests_MathPractice_DistancePointToLine2D;
import UnitTests_MathPractice_DistancePointToLineSegment2D;
import UnitTests_MathPractice_DistancePointToRay2D;
import UnitTests_MathPractice_VectorRotation2D;
import UnitTest_MathPractice_FindPerpendicularVector;
import UnitTest_MathPractice_DistancePointToTriangle3D;
import UnitTest_MathPractice_DistancePointToPlane3D;

// Main entry point for math practice. import any test modules above and add them to this function
export bool RunUnitTests_MathPractice()
{
	// Vector2
	JPT_ENSURE(UnitTest_MathPractice_FindAngle2D());
	JPT_ENSURE(UnitTest_MathPractice_FindAngleAndDir2D());
	JPT_ENSURE(UnitTest_MathPractice_DistancePointToLine2D());
	JPT_ENSURE(UnitTest_MathPractice_DistancePointToLineSegment2D());
	JPT_ENSURE(UnitTest_MathPractice_DistancePointToRay2D());
	JPT_ENSURE(UnitTest_MathPractice_VectorRotation2D());

	// Vector3
	JPT_ENSURE(UnitTest_MathPractice_FindPerpendicularVector());
	JPT_ENSURE(UnitTest_MathPractice_DistancePointToTriangle3D());
	JPT_ENSURE(UnitTest_MathPractice_DistancePointToPlane3D());

	return true;
}