// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_MathPractice_DistancePointToLine2D;

import jpt.CoreModules;

/** Perpendicular projection from point to line */
float DistancePointToLine2D(Vec2f point, Line2f line)
{
	const Vec3f lineData = line.GetLine();
	const float result = std::abs(lineData.x * point.x + lineData.y * point.y + lineData.z) / std::sqrt(lineData.x * lineData.x + lineData.y * lineData.y);

	return result;
}

export bool UnitTest_MathPractice_DistancePointToLine2D()
{
	Line2f line = { Vec2f(-1, 0), Vec2f(1, 0) };

	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(0, 0), line), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(1, 0), line), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(2, 0), line), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(0, 1), line), 1.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(0, 2), line), 2.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(0, 20), line), 20.0f));

	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 0)),   0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(1, 0)),   0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(2, 0)),   0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 1)),   1.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 2)),   2.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 20)), 20.0f));

	line = { Vec2f(0, -1), Vec2f(0, 1) };
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(0, 0), line), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(0, 1), line), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(0, 2), line), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(1, 0), line), 1.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(2, 0), line), 2.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(20, 0), line), 20.0f));

	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 0)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 1)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 2)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(1, 0)), 1.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(2, 0)), 2.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(20, 0)), 20.0f));

	line = { Vec2f(0, 0), Vec2f(1, 1) };
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(0, 0), line), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(1, 1), line), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(2, 2), line), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(0, 1), line), std::sqrt(2.0f) / 2.0f));
	JPT_ENSURE(jpt::AreValuesClose(DistancePointToLine2D(Vec2f(1, 0), line), std::sqrt(2.0f) / 2.0f));

	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 0)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(1, 1)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(2, 2)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 1)), std::sqrt(2.0f) / 2.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(1, 0)), std::sqrt(2.0f) / 2.0f));

	return true;
}