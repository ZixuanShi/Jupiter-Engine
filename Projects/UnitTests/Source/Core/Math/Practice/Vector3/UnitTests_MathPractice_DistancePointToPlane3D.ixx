// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTest_MathPractice_DistancePointToPlane3D;

import jpt.CoreModules;

float DistancePointToPlane3D(Vec3f point, Vec3f a, Vec3f b, Vec3f c)
{
	const Vec3f edgeBA = b - a;
	const Vec3f edgeCA = c - a;

	const Vec3f cross = Vec3f::Cross(edgeBA, edgeCA);
	const Vec3f normal = cross.Normalized();

	const Vec3f pointToPlane = a - point;
	const float distance = std::abs(normal.Dot(pointToPlane));

	return distance;
}

export bool UnitTest_MathPractice_DistancePointToPlane3D()
{

	return true;
}