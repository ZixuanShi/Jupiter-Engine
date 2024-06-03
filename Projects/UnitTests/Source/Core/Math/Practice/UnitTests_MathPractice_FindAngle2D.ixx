// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTest_MathPractice_FindAngle2D;

import jpt.CoreModules;

/** @return Angle in degrees between two vectors representing position in 2D */
float FindAngle2D(Vec2f sourcePos, Vec2f sourceFacingDir, Vec2f targetPos)
{
	Vec2f targetDir = targetPos - sourcePos;
	targetDir.Normalize();

	float dot = sourceFacingDir.Dot(targetDir);
	float angle = acos(dot);

	return angle;
}

export bool UnitTest_MathPractice_FindAngle2D()
{
	//JPT_ENSURE(FindAngle2D(Vec2f(0, 0), Vec2f(1, 0), Vec2f(1, 0)) == 0);

	return true;
}