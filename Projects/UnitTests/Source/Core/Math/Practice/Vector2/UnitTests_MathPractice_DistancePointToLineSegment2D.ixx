// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_MathPractice_DistancePointToLineSegment2D;

import jpt.CoreModules;

/** Closest distance from point to line */
float DistancePointToLineSegment2D(Vec2f point, LineSegment2f line)
{
	const Vec2f direction = line.Direction();
	const float length2 = direction.Length2();

	// If the line segment is a point, return the distance between the point and the segment's endpoint
	if (jpt::AreValuesClose(length2, 0.0f))
	{
		return (point - line.a).Length();
	}

	// Calculate the parameter t of the closest point on the line segment
	const Vec2f toPoint = point - line.a;
	float t = Vec2f::Dot(toPoint, direction) / length2;
	jpt::ClampTo(t, 0.0f, 1.0f);

	// Calculate the closest point on the line segment
	const Vec2f projection = line.a + direction * t;

	// Return the distance between the point and the closest point on the line segment
	const float distance = (point - projection).Length();
	return distance;
}

export bool UnitTest_MathPractice_DistancePointToLineSegment2D()
{
	Vec2f point = Vec2f(0.0f, 0.0f);
	LineSegment2f line = LineSegment2f(Vec2f(1.0f, 0.0f), Vec2f(1.0f, 1.0f));

	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLineSegment2D(point, line), 1.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(point), 1.0f));

	point = Vec2f(-3.0f, 0.0f);
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLineSegment2D(point, line), 4.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(point), 4.0f));

	point = Vec2f(3.0f, 0.0f);
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLineSegment2D(point, line), 2.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(point), 2.0f));

	point = Vec2f(0.0f, 0.0f);
	line = LineSegment2f(Vec2f(1.0f, 0.0f), Vec2f(0.0f, 1.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLineSegment2D(point, line), 0.707106769f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(point), 0.707106769f));

	return true;
}