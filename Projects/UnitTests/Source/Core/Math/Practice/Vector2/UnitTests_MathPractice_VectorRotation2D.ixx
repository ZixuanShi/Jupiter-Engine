// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_MathPractice_VectorRotation2D;

import jpt.CoreModules;

export bool UnitTest_MathPractice_VectorRotation2D()
{
	Vec2f v1(0.0f, 2.0f);
	Vec2f v2(1.0f, -1.0f);
	Vec2f v3(-1.0f, -1.0f);
	
	v1.RotateDegrees(49.0f);
	v2.RotateDegrees(49.0f);
	v3.RotateDegrees(49.0f);
	JPT_ENSURE(v1 == Vec2f(-1.5f, 1.3f));
	JPT_ENSURE(v2 == Vec2f(1.4f, 0.1f));
	JPT_ENSURE(v3 == Vec2f(0.1f, -1.4f));

	JPT_ENSURE(Vec2f::RotateDegrees(v1, -49.0f) == Vec2f(0.0f, 2.0f));
	JPT_ENSURE(Vec2f::RotateDegrees(v2, -49.0f) == Vec2f(1.0f, -1.0f));
	JPT_ENSURE(Vec2f::RotateDegrees(v3, -49.0f) == Vec2f(-1.0f, -1.0f));

	return true;
}