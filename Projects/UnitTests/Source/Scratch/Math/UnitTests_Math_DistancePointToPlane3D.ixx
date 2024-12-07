// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Math_DistancePointToPlane3D;

import jpt.Vector3;
import jpt.Utilities;
import jpt.Math;

float DistancePointToPlane3D(Vec3f point, Vec3f a, Vec3f b, Vec3f c)
{
	const Vec3f edgeBA = b - a;
	const Vec3f edgeCA = c - a;

	const Vec3f cross = Vec3f::Cross(edgeBA, edgeCA);
	const Vec3f normal = cross.Normalized();

	const Vec3f pointToPlane = point - a;
	const float distance = jpt::Abs(Vec3f::Dot(pointToPlane, normal));

	return distance;
}

export bool UnitTests_Math_DistancePointToPlane3D()
{
	Vec3f point = Vec3f(0.0f, 0.0f, 0.0f);
	Vec3f a = Vec3f(0.0f, 0.0f, 0.0f);
	Vec3f b = Vec3f(0.0f, 0.0f, 0.0f);
	Vec3f c = Vec3f(0.0f, 0.0f, 0.0f);

	JPT_ENSURE(jpt::AreValuesClose(DistancePointToPlane3D(point, a, b, c), 0.0f));

	point = Vec3f(0.0f, 0.0f, 20.0f);
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToPlane3D(point, a, b, c), 0.0f));

	point = Vec3f(20.0f, 20.0f, 20.0f);
	a = Vec3f(10.0f, 0.5f, 8.0f);
	b = Vec3f(0.7f, 0.0f, 0.0f);
	c = Vec3f(0.0f, 6.0f, 0.0f);
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToPlane3D(point, a, b, c), 1.14197397));

	return true;
}