// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Box2;

import jpt.Box2;
import jpt.Vector2;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTests_Box2()
{
    Box2f box1(Vec2f(-10, -10), Vec2f(10, 10));
    JPT_ENSURE(box1.Center() == Vec2f(0, 0));
    JPT_ENSURE(box1.Size() == Vec2f(20, 20));
    JPT_ENSURE(box1.HalfSize() == Vec2f(10, 10));

    JPT_ENSURE(box1.Inside(Vec2f(0, 0)));
    JPT_ENSURE(box1.Inside(Vec2f(-5, -5)));
    JPT_ENSURE(box1.Inside(Vec2f(5, 5)));
    JPT_ENSURE(!box1.Inside(Vec2f(-15, -15)));
    JPT_ENSURE(!box1.Inside(Vec2f(15, 15)));

    Box2f box2(Vec2f(-5, -5), Vec2f(15, 15));
    JPT_ENSURE(box1.Intersects(box2));

    Box2f box3(Vec2f(15, 15), Vec2f(25, 25));
    JPT_ENSURE(!box1.Intersects(box3));

    return true;
}

export bool RunUnitTests_Box2()
{
    JPT_ENSURE(UnitTests_Box2());

    return true;
}
