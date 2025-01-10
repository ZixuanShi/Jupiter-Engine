// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Math_FindAngle2D;

import jpt.Constants;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;
import jpt.Vector2;

/** @return Angle in degrees between two vectors representing position in 2D 
	@note   Doesn't reflect the direction to turn. Only scalar of angle  */
float FindAngle2D(Vec2f sourcePos, Vec2f sourceFacingDir, Vec2f targetPos)
{
	const Vec2f toTargetDir = (targetPos - sourcePos).Normalized();
	return sourceFacingDir.AngleDegrees(toTargetDir);
}

/** @return Angle in dgrees from -180 to 180 between 2 position respective to facing dir */
float FindSignedAngle2D(Vec2f sourcePos, Vec2f sourceFacingDir, Vec2f targetPos)
{
	const Vec2f toTargetDir = (targetPos - sourcePos).Normalized();
	return sourceFacingDir.AngleSignedDegrees(toTargetDir);
}

export bool UnitTests_Math_FindAngle2D()
{
	JPT_ENSURE(jpt::AreValuesClose(FindAngle2D(Vec2f(0, 0), Vec2f(1, 0), Vec2f(10, 0)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindAngle2D(Vec2f(10, 0), Vec2f(1, 0), Vec2f(-10, 0)), 180.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindAngle2D(Vec2f(10, 0), Vec2f(0, 1), Vec2f(-10, 0)), 90.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindAngle2D(Vec2f(10, 0), Vec2f(0, -1), Vec2f(-10, 0)), 90.0f));

	JPT_ENSURE(jpt::AreValuesClose(FindSignedAngle2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f( 10.0f,   0.0f)),    0.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindSignedAngle2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f( 10.0f,  10.0f)),   45.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindSignedAngle2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f(  0.0f,  10.0f)),   90.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindSignedAngle2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f(-10.0f,  10.0f)),  135.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindSignedAngle2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f(-10.0f,   0.0f)),  180.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindSignedAngle2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f(-10.0f, -10.0f)), -135.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindSignedAngle2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f(  0.0f, -10.0f)),  -90.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindSignedAngle2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f( 10.0f, -10.0f)),  -45.0f));

	return true;
}