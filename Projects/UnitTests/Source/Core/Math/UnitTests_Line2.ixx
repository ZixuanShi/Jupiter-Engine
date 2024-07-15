// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Line2;

import jpt.Line2;
import jpt.Vector2;
import jpt.Math;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_Line2_DistanceToPoint()
{
	Line2f line = { Vec2f(-1, 0), Vec2f(1, 0) };
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 0)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(1, 0)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(2, 0)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 1)), 1.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 2)), 2.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 20)), 20.0f));

	line = { Vec2f(0, -1), Vec2f(0, 1) };
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 0)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 1)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 2)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(1, 0)), 1.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(2, 0)), 2.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(20, 0)), 20.0f));

	line = { Vec2f(0, 0), Vec2f(1, 1) };
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 0)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(1, 1)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(2, 2)), 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(0, 1)), std::sqrt(2.0f) / 2.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(Vec2f(1, 0)), std::sqrt(2.0f) / 2.0f));

    return true;
}

export bool RunUnitTests_Line2()
{
    JPT_ENSURE(UnitTest_Line2_DistanceToPoint());

    return true;
}
