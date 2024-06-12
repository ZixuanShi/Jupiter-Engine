// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTest_MathPractice_FindTriangleNormal;

import jpt.CoreModules;

Vec3f FindTriangleNormal(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2)
{
	Vec3f edge1 = v1 - v0;
	Vec3f edge2 = v2 - v0;
	Vec3f normal = edge1.Cross(edge2);
	normal.Normalize();
	return normal;
}

export bool UnitTest_MathPractice_FindTriangleNormal()
{
	JPT_ENSURE(FindTriangleNormal(Vec3f(0, 0, 0), Vec3f(1, 0, 0), Vec3f(0, 1, 0)) == Vec3f(0, 0, 1));
	JPT_ENSURE(Triangle3f(Vec3f(0, 0, 0), Vec3f(1, 0, 0), Vec3f(0, 1, 0)).Normal() == Vec3f(0, 0, 1));

	JPT_ENSURE(FindTriangleNormal(Vec3f(0, 0, 0), Vec3f(0, 1, 0), Vec3f(1, 0, 0)) == Vec3f(0, 0, -1));
	JPT_ENSURE(Triangle3f(Vec3f(0, 0, 0), Vec3f(0, 1, 0), Vec3f(1, 0, 0)).Normal() == Vec3f(0, 0, -1));

	return true;
}