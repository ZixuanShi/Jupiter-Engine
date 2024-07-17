// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Triangle3;

import jpt.Triangle3;
import jpt.Vector3;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;

bool UnitTests_Triangle3_Normal()
{
    JPT_ENSURE(Triangle3f(Vec3f(0, 0, 0), Vec3f(1, 0, 0), Vec3f(0, 1, 0)).Normal() == Vec3f(0, 0, 1));
    JPT_ENSURE(Triangle3f(Vec3f(0, 0, 0), Vec3f(0, 1, 0), Vec3f(1, 0, 0)).Normal() == Vec3f(0, 0, -1));

    return true;
}

bool UnitTests_Triangle3_PointInside()
{
	Triangle3f triangle(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));

	Vec3f point(0.5f, 0.5f, 0.0f);
	JPT_ENSURE(triangle.Inside(point));

	point = Vec3f(10.0f, 10.0f, 10.0f);
	JPT_ENSURE(!triangle.Inside(point));

	point = Vec3f(0.1f, 0.0f, 0.0f);
	JPT_ENSURE(triangle.Inside(point));

	point = Vec3f(0.1f, 0.0f, 0.0f);
	JPT_ENSURE(triangle.Inside(point));

	point = Vec3f(0.1f, 0.5f, 0.0f);
	JPT_ENSURE(triangle.Inside(point));

	point = Vec3f(0.1f, -0.5f, 0.0f);
	JPT_ENSURE(!triangle.Inside(point));

	point = Vec3f(-0.1f, 0.0f, 0.0f);
	JPT_ENSURE(!triangle.Inside(point));

    return true;
}

bool UnitTests_Triangle3_DistanceToPoint()
{
	Vec3f point(0.0f, 0.0f, 0.0f);
	Triangle3f triangle(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(triangle.Distance(point), 0.0f));

	triangle = Triangle3f(Vec3f(0.0f, 0.0f, 1.0f), Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(triangle.Distance(point), 0.577350259f));

	point = (0.0f, 0.0f, 0.0f);
	triangle = Triangle3f(Vec3f(-1.0f, 0.0f, 0.0f), Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.5f, 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(triangle.Distance(point), 0.0f));

	point = (0.0f, 0.0f, 10.0f);
	triangle = Triangle3f(Vec3f(-1.0f, 0.0f, 0.0f), Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.5f, 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(triangle.Distance(point), 10.0f));

	point = (-10.0f, 05.0f, 10.0f);
	triangle = Triangle3f(Vec3f(-1.0f, 0.0f, 1.0f), Vec3f(1.0f, 0.0f, 1.0f), Vec3f(0.0f, 0.5f, 1.0f));
	JPT_ENSURE(jpt::AreValuesClose(triangle.Distance(point), 9.0f));

    return true;
}

export bool RunUnitTests_Triangle3()
{
    JPT_ENSURE(UnitTests_Triangle3_Normal());
    JPT_ENSURE(UnitTests_Triangle3_PointInside());
    JPT_ENSURE(UnitTests_Triangle3_DistanceToPoint());

    return true;
}
