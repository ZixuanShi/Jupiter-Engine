// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Vector2;

import jpt.Vector2;
import jpt.Utilities;
import jpt.TypeDefs;
import jpt.Math;

bool UnitTest_Vec2f_Operations()
{
    Vec2f v(2.0f, 2.0f);

    // Other vector
    JPT_ENSURE(v + Vec2f(1.0f, 2.0f) == Vec2f(3.0f, 4.0f));
    JPT_ENSURE(v - Vec2f(1.0f, 2.0f) == Vec2f(1.0f, 0.0f));
    JPT_ENSURE(v * Vec2f(1.0f, 2.0f) == Vec2f(2.0f, 4.0f));
    JPT_ENSURE(v / Vec2f(1.0f, 2.0f) == Vec2f(2.0f, 1.0f));

    v += Vec2f(1.0f, 2.0f);
    JPT_ENSURE(v == Vec2f(3.0f, 4.0f));

    v -= Vec2f(1.0f, 2.0f);
    JPT_ENSURE(v == Vec2f(2.0f, 2.0f));

    v *= Vec2f(1.0f, 2.0f);
    JPT_ENSURE(v == Vec2f(2.0f, 4.0f));

    v /= Vec2f(1.0f, 2.0f);
    JPT_ENSURE(v == Vec2f(2.0f, 2.0f));

    // Scalar
    JPT_ENSURE(v + 2.0f == Vec2f(4.0f, 4.0f));
    JPT_ENSURE(v - 2.0f == Vec2f(0.0f, 0.0f));
    JPT_ENSURE(v * 2.0f == Vec2f(4.0f, 4.0f));
    JPT_ENSURE(v / 2.0f == Vec2f(1.0f, 1.0f));

    v += 2.0f;
    JPT_ENSURE(v == Vec2f(4.0f, 4.0f));

    v -= 2.0f;
    JPT_ENSURE(v == Vec2f(2.0f, 2.0f));

    v *= 2.0f;
    JPT_ENSURE(v == Vec2f(4.0f, 4.0f));

    v /= 2.0f;
    JPT_ENSURE(v == Vec2f(2.0f, 2.0f));

    return true;
}

bool UnitTest_Vec2f_Functionalities()
{
    Vec2f v(2.0f, 2.0f);

    // Dot product
    JPT_ENSURE(v.Dot(Vec2f(1.0f, 2.0f)) == 6.0f);
    JPT_ENSURE(Vec2f::Dot(v, Vec2f(1.0f, 2.0f)) == 6.0f);

    // Length
    JPT_ENSURE(v.Length() == 2.828427f);
    JPT_ENSURE(Vec2f::Length(v) == 2.828427f);

    // Length2 
    JPT_ENSURE(v.Length2() == 8.0f);
    JPT_ENSURE(Vec2f::Length2(v) == 8.0f);

    // distance
    JPT_ENSURE(v.Distance(Vec2f(1.0f, 2.0f)) == 1.0f);
    JPT_ENSURE(Vec2f::Distance(v, Vec2f(1.0f, 2.0f)) == 1.0f);

    // distance2
    JPT_ENSURE(v.Distance2(Vec2f(1.0f, 2.0f)) == 1.0f);
    JPT_ENSURE(Vec2f::Distance2(v, Vec2f(1.0f, 2.0f)) == 1.0f);

    // Normalize
    JPT_ENSURE(v.Normalized() == Vec2f(0.7071067f, 0.7071067f));
    JPT_ENSURE(Vec2f::Normalized(v) == Vec2f(0.7071067f, 0.7071067f));

    // Lerp
    JPT_ENSURE(v.Lerped(Vec2f(4.0f, 4.0f), 0.5f) == Vec2f(3.0f, 3.0f));
    JPT_ENSURE(Vec2f::Lerped(v, Vec2f(4.0f, 4.0f), 0.5f) == Vec2f(3.0f, 3.0f));

    return true;
}

export bool RunUnitTests_Vector2()
{
    JPT_ENSURE(UnitTest_Vec2f_Operations());
    JPT_ENSURE(UnitTest_Vec2f_Functionalities());
    
    return true;
}
