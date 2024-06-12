// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_MathPractice_FindAngleAndDir2D;

import jpt.CoreModules;

/** @return Angle in dgrees from -180 to 180 between 2 position respective to facing dir */
float FindAngleAndDir2D(Vec2f sourcePos, Vec2f sourceFacingDir, Vec2f targetPos)
{
	const Vec2f toTargetDir = (targetPos - sourcePos).Normalized();
	float atan2 = std::atan2f(sourceFacingDir.y, sourceFacingDir.x) - std::atan2f(toTargetDir.y, toTargetDir.x);

	// Ranged from -180 to 180
	if (atan2 > PI)
	{
		atan2 -= 2 * PI;
	}
	else if (atan2 < -PI)
	{
		atan2 += 2 * PI;
	}
	
	const float angleDegrees = jpt::ToDegrees(atan2);
	return angleDegrees;
}

export bool UnitTest_MathPractice_FindAngleAndDir2D()
{
	JPT_ENSURE(jpt::AreValuesClose(FindAngleAndDir2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f( 10.0f,   0.0f)),    0.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindAngleAndDir2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f( 10.0f,  10.0f)),  -45.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindAngleAndDir2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f(  0.0f,  10.0f)),  -90.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindAngleAndDir2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f(-10.0f,  10.0f)), -135.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindAngleAndDir2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f(-10.0f,   0.0f)), -180.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindAngleAndDir2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f(-10.0f, -10.0f)),  135.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindAngleAndDir2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f(  0.0f, -10.0f)),   90.0f));
	JPT_ENSURE(jpt::AreValuesClose(FindAngleAndDir2D(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f( 10.0f, -10.0f)),   45.0f));

	return true;
}