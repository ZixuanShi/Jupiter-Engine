// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_MathPractice_PointInsideTriangle2D;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;
import jpt.Triangle2;
import jpt.Vector2;

bool PointInsideTriangle2D(Vec2f point, Triangle2f triangle2)
{
	// Barycentric coordinates

	const Vec2f A = triangle2.a;
	const Vec2f B = triangle2.b;
	const Vec2f C = triangle2.c;

	const float u = (B.y - C.y) * (point.x - C.x) + (C.x - B.x) * (point.y - C.y);
	const float v = (C.y - A.y) * (point.x - C.x) + (A.x - C.x) * (point.y - C.y);
	const float w = 1.0f - u - v;

	return u >= 0.0f && v >= 0.0f && w >= 0.0f;
}

export bool UnitTest_MathPractice_PointInsideTriangle2D()
{
	Triangle2f triangle2(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f(0.0f, 1.0f));
	Vec2f point(0.5f, 0.5f);

	JPT_ENSURE(PointInsideTriangle2D(point, triangle2));
	JPT_ENSURE(triangle2.Inside(point));

	triangle2 = Triangle2f(Vec2f(10.0f, 10.0f), Vec2f(11.0f, 10.0f), Vec2f(10.0f, 11.0f));

	JPT_ENSURE(!PointInsideTriangle2D(point, triangle2));
	JPT_ENSURE(!triangle2.Inside(point));

	return true;
}