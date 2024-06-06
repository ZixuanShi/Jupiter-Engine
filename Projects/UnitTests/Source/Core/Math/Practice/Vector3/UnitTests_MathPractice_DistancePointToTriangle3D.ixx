// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTest_MathPractice_DistancePointToTriangle3D;

import jpt.CoreModules;

float DistancePointToTriangle3D(Vec3f point, Triangle3f triangle)
{
	// Compute the edges of the triangle
	Vec3f edgeBa = triangle.b - triangle.a;
	Vec3f edgeCa = triangle.c - triangle.a;

	// Compute the normal of the triangle
	Vec3f normal = edgeBa.Cross(edgeCa);
	normal.Normalize();

	// Compute the vector from the point to the triangle
	float distance = std::abs(normal.Dot(point - triangle.a));

	return distance;
}

export bool UnitTest_MathPractice_DistancePointToTriangle3D()
{
	Vec3f point(0.0f, 0.0f, 0.0f);
	Triangle3f triangle(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToTriangle3D(point, triangle), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(triangle.Distance(point), 0.0f));

	triangle = Triangle3f(Vec3f(0.0f, 0.0f, 1.0f), Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToTriangle3D(point, triangle), 0.577350259f));
	JPT_ENSURE(jpt::AreValuesClose(triangle.Distance(point), 0.577350259f));

	point = (0.0f, 0.0f, 0.0f);
	triangle = Triangle3f(Vec3f(-1.0f, 0.0f, 0.0f), Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.5f, 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToTriangle3D(point, triangle), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(triangle.Distance(point), 0.0f));

	point = (0.0f, 0.0f, 10.0f);
	triangle = Triangle3f(Vec3f(-1.0f, 0.0f, 0.0f), Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.5f, 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToTriangle3D(point, triangle), 10.0f));
	JPT_ENSURE(jpt::AreValuesClose(triangle.Distance(point), 10.0f));

	point = (-10.0f, 05.0f, 10.0f);
	triangle = Triangle3f(Vec3f(-1.0f, 0.0f, 1.0f), Vec3f(1.0f, 0.0f, 1.0f), Vec3f(0.0f, 0.5f, 1.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToTriangle3D(point, triangle), 9.0f));
	JPT_ENSURE(jpt::AreValuesClose(triangle.Distance(point), 9.0f));

	return true;
}