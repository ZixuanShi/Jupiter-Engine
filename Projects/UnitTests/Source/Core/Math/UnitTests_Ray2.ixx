// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Ray2;

import jpt.Ray2;
import jpt.Vector2;
import jpt.Math;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_Ray2_DistanceToPoint()
{
	Ray2f ray = { Vec2f(0.0f, 0.0f), Vec2f(1.0f, 1.0f) };
	Vec2f point = { 0.0f, 0.0f };

	JPT_ENSURE(jpt::AreValuesClose(ray.Distance(point), 0.0f));

	point = { 1.0f, 1.0f };
	JPT_ENSURE(jpt::AreValuesClose(ray.Distance(point), 0.0f));

	point = { 2.0f, 2.0f };
	JPT_ENSURE(jpt::AreValuesClose(ray.Distance(point), 0.0f));

	point = { -1.0f, -1.0f };
	JPT_ENSURE(jpt::AreValuesClose(ray.Distance(point), 1.41421354f));

	point = { -2.0f, -2.0f };
	JPT_ENSURE(jpt::AreValuesClose(ray.Distance(point), 2.82842708f));

	point = { 0.0f, 1.0f };
	JPT_ENSURE(jpt::AreValuesClose(ray.Distance(point), 0.707106769f));

    return true;
}

export bool RunUnitTests_Ray2()
{
    JPT_ENSURE(UnitTest_Ray2_DistanceToPoint());

    return true;
}
