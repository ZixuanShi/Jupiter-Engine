// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Box3;

import jpt.Box3;
import jpt.Vector3;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTests_Box3()
{
    Box3f box1(Vec3f(-10, -10, -5), Vec3f(10, 10, 5));
    JPT_ENSURE(box1.Center() == Vec3f(0, 0, 0));
    JPT_ENSURE(box1.Size() == Vec3f(20, 20, 10));
    JPT_ENSURE(box1.HalfSize() == Vec3f(10, 10, 5));
    JPT_ENSURE(box1.Volume() == 4000);

    JPT_ENSURE(box1.Inside(Vec3f(0, 0, 0)));
    JPT_ENSURE(box1.Inside(Vec3f(-5, -5, -2)));
    JPT_ENSURE(box1.Inside(Vec3f(5, 5, 2)));
    JPT_ENSURE(!box1.Inside(Vec3f(-15, -15, -7)));
    JPT_ENSURE(!box1.Inside(Vec3f(15, 15, 7)));

    Box3f box2(Vec3f(-5, -5, -2), Vec3f(15, 15, 2));
    JPT_ENSURE(box1.Intersects(box2));

    Box3f box3(Vec3f(15, 15, 2), Vec3f(25, 25, 7));
    JPT_ENSURE(!box1.Intersects(box3));

    return true;
}

export bool RunUnitTests_Box3()
{
    JPT_ENSURE(UnitTests_Box3());

    return true;
}
