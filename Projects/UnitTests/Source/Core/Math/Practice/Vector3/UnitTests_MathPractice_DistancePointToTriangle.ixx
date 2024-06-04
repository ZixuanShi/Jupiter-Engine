// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTest_MathPractice_DistancePointToTriangle;

import jpt.CoreModules;

float DistancePointToTriangle(Vec3f point, Vec3f v0, Vec3f v1, Vec3f v2)
{
	JPT_IGNORE(point, v0, v1, v2);
	//Vec3f edge0 = v1 - v0;
	//Vec3f edge1 = v2 - v0;

	return 0.0f;
}

float DistancePointToTriangle(Vec3f point, const Triangle3f& triangle)
{
	return triangle.Distance(point);
}

export bool UnitTest_MathPractice_DistancePointToTriangle()
{

	return true;
}