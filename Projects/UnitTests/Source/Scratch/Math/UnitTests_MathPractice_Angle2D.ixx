// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_MathPractice_FindAngle2D;

import jpt.CoreModules;

/** @return Angle in degrees between two vectors representing position in 2D 
	@note   Doesn't reflect the direction to turn. Only scalar of angle  */
float FindAngle2D(Vec2f sourcePos, Vec2f sourceFacingDir, Vec2f targetPos)
{
	const Vec2f toTargetDir = (targetPos - sourcePos).Normalized();
	const float dot = sourceFacingDir.Dot(toTargetDir);
	const float angleRadius = acos(dot);
	const float angleDegrees = jpt::ToDegrees(angleRadius);
	return angleDegrees;
}

export bool UnitTest_MathPractice_FindAngle2D()
{
	JPT_ENSURE(jpt::AreValuesClose(FindAngle2D(Vec2f(0, 0), Vec2f(1, 0), Vec2f(10, 0)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindAngle2D(Vec2f(10, 0), Vec2f(1, 0), Vec2f(-10, 0)), 180.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindAngle2D(Vec2f(10, 0), Vec2f(0, 1), Vec2f(-10, 0)), 90.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindAngle2D(Vec2f(10, 0), Vec2f(0, -1), Vec2f(-10, 0)), 90.0f));

	return true;
}