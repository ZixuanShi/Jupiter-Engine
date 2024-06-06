// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_MathPractice_DistancePointToRay2D;

import jpt.CoreModules;

float DistancePointToRay2D(Vec2f point, Ray2f ray)
{
	const float dot = Vec2f::Dot(point - ray.origin, ray.direction);
	float distance = jpt::kInvalidValue<float>;

	// If the point is behind the ray's origin, return the distance to the origin
	if (dot < 0.0f)
	{
		distance = Vec2f::Distance(point, ray.origin);
	}
	else
	{
		// Otherwise, return the distance to the point on the ray closest to the point
		const Vec2f projection = ray.origin + ray.direction * (dot / ray.direction.Length2());
		distance = Vec2f::Distance(point, projection);
	}

	return distance;
}

export bool UnitTest_MathPractice_DistancePointToRay2D()
{
	Ray2f ray = { Vec2f(0.0f, 0.0f), Vec2f(1.0f, 1.0f) };
	Vec2f point = { 0.0f, 0.0f };

	JPT_ENSURE(jpt::AreValuesClose(DistancePointToRay2D(point, ray), 0.0f));
	
	point = { 1.0f, 1.0f };
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToRay2D(point, ray), 0.0f));

	point = { 2.0f, 2.0f };
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToRay2D(point, ray), 0.0f));

	point = { -1.0f, -1.0f };
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToRay2D(point, ray), 1.41421354f));

	point = { -2.0f, -2.0f };
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToRay2D(point, ray), 2.82842708f));

	point = { 0.0f, 1.0f };
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToRay2D(point, ray), 0.707106769f));

	return true;
}