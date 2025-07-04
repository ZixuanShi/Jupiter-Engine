// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Triangle2;

import jpt.Triangle2;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTests_Triangle2_PointInsideTriangle()
{
    Triangle2f triangle2(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f(0.0f, 1.0f));
    Vec2f point(0.5f, 0.5f);

    JPT_ENSURE(triangle2.Inside(point));

    triangle2 = Triangle2f(Vec2f(10.0f, 10.0f), Vec2f(11.0f, 10.0f), Vec2f(10.0f, 11.0f));

    JPT_ENSURE(!triangle2.Inside(point));

    return true;
}

export bool RunUnitTests_Triangle2()
{
    JPT_ENSURE(UnitTests_Triangle2_PointInsideTriangle());

    return true;
}
