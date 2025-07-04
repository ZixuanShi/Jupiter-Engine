// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_LineSegment2;

import jpt.LineSegment2;
import jpt.Vector2;
import jpt.Math;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTests_LineSegment2_DistanceToPoint()
{
    Vec2f point = Vec2f(0.0f, 0.0f);
    LineSegment2f line = LineSegment2f(Vec2f(1.0f, 0.0f), Vec2f(1.0f, 1.0f));

    JPT_ENSURE(jpt::AreValuesClose(line.Distance(point), 1.0f));

    point = Vec2f(-3.0f, 0.0f);
    JPT_ENSURE(jpt::AreValuesClose(line.Distance(point), 4.0f));

    point = Vec2f(3.0f, 0.0f);
    JPT_ENSURE(jpt::AreValuesClose(line.Distance(point), 2.0f));

    point = Vec2f(0.0f, 0.0f);
    line = LineSegment2f(Vec2f(1.0f, 0.0f), Vec2f(0.0f, 1.0f));
    JPT_ENSURE(jpt::AreValuesClose(line.Distance(point), 0.707106769f));

    return true;
}

export bool RunUnitTests_LineSegment2()
{
    JPT_ENSURE(UnitTests_LineSegment2_DistanceToPoint());

    return true;
}
