// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Vector2;

import jpt.Vector2;
import jpt.Utilities;
import jpt.TypeDefs;
import jpt.Math;

bool UnitTests_Vec2f_Length()
{
    Vec2f v(3.0f, 4.0f);
    JPT_ENSURE(v.Length() == 5.0f);
    JPT_ENSURE(v.Length2() == 25.0f);

    Vec2f zero(0.0f, 0.0f);
    JPT_ENSURE(zero.Length() == 0.0f);
    JPT_ENSURE(zero.Length2() == 0.0f);

    Vec2f unitX(1.0f, 0.0f);
    JPT_ENSURE(unitX.Length() == 1.0f);
    JPT_ENSURE(unitX.Length2() == 1.0f);

    Vec2f unitY(0.0f, 1.0f);
    JPT_ENSURE(unitY.Length() == 1.0f);
    JPT_ENSURE(unitY.Length2() == 1.0f);

    Vec2f negative(-3.0f, -4.0f);
    JPT_ENSURE(negative.Length() == 5.0f);
    JPT_ENSURE(negative.Length2() == 25.0f);

    return true;
}

bool UnitTests_Vec2f_Distance()
{
    Vec2f v1(1.0f, 2.0f);
    Vec2f v2(4.0f, 6.0f);
    JPT_ENSURE(v1.Distance(v2) == 5.0f);
    JPT_ENSURE(v1.Distance2(v2) == 25.0f);

    v1 = Vec2f(-1.0f, -2.0f);
    v2 = Vec2f(-4.0f, -6.0f);
    JPT_ENSURE(v1.Distance(v2) == 5.0f);
    JPT_ENSURE(v1.Distance2(v2) == 25.0f);

    v1 = Vec2f(0.0f, 0.0f);
    JPT_ENSURE(jpt::AreValuesClose(v1.Distance(v2), 7.21110249f));
    JPT_ENSURE(v1.Distance2(v2) == 52.0f);

    return true;
}

bool UnitTests_Vec2f_Operations()
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

bool UnitTests_Vec2f_Functionalities()
{
    Vec2f v(2.0f, 2.0f);

    // Dot product
    JPT_ENSURE(v.Dot(Vec2f(1.0f, 2.0f)) == 6.0f);

    // Length
    JPT_ENSURE(v.Length() == 2.828427f);

    // Length2 
    JPT_ENSURE(v.Length2() == 8.0f);

    // distance
    JPT_ENSURE(v.Distance(Vec2f(1.0f, 2.0f)) == 1.0f);

    // distance2
    JPT_ENSURE(v.Distance2(Vec2f(1.0f, 2.0f)) == 1.0f);

    // Normalize
    JPT_ENSURE(v.Normalized() == Vec2f(0.7071067f, 0.7071067f));

    // Lerp
    JPT_ENSURE(Lerp(v, Vec2f(4.0f, 4.0f), 0.5f) == Vec2f(3.0f, 3.0f));

    return true;
}

bool RunUnitTests_Vec2f_Angle()
{
    Vec2f source(0.0f, 1.0f);
    Vec2f target(1.0f, 1.0f);

    JPT_ENSURE(jpt::AreValuesClose(Vec2f::Angle(source, target.Normalized()), jpt::ToRadians(45.0f)));
    JPT_ENSURE(jpt::AreValuesClose(Vec2f::AngleSigned(source, target.Normalized()), jpt::ToRadians(-45.0f)));

    Vec2 vector1(1.0f, 0.0f);
    Vec2 vector2(1.0f, 0.0f);
    vector1 = vector1.Rotate(jpt::ToRadians(45.0f));
    const float angle = Vec2f::Angle(vector1, vector2);
    JPT_ENSURE(jpt::AreValuesClose(angle, jpt::ToRadians(45.0f)));

    return true;
}

bool RunUnitTests_Vec2f_Rotation()
{
    Vec2f v1(0.0f, 2.0f);
    Vec2f v2(1.0f, -1.0f);
    Vec2f v3(-1.0f, -1.0f);

    v1 = v1.Rotate(jpt::ToRadians(49.0f));
    v2 = v2.Rotate(jpt::ToRadians(49.0f));
    v3 = v3.Rotate(jpt::ToRadians(49.0f));
    JPT_ENSURE(v1 == Vec2f(-1.50941920f, 1.31211793f));
    JPT_ENSURE(v2 == Vec2f(1.41076851f, 0.0986506343f));
    JPT_ENSURE(v3 == Vec2f(0.0986506343f, -1.41076851f));

    JPT_ENSURE(v1.Rotate(jpt::ToRadians(-49.0f)) == Vec2f(0.0f, 2.0f));
    JPT_ENSURE(v2.Rotate(jpt::ToRadians(-49.0f)) == Vec2f(1.0f, -1.0f));
    JPT_ENSURE(v3.Rotate(jpt::ToRadians(-49.0f)) == Vec2f(-1.0f, -1.0f));

    v1 = v1.Rotate(jpt::ToRadians(360.0f));
    v2 = v2.Rotate(jpt::ToRadians(360.0f));
    v3 = v3.Rotate(jpt::ToRadians(360.0f));
    JPT_ENSURE(v1 == Vec2f(-1.50941944f, 1.31211770f));
    JPT_ENSURE(v2 == Vec2f(1.41076851f, 0.0986508802f));
    JPT_ENSURE(v3 == Vec2f(0.0986508802f, -1.41076851f));

    v3 = Vec2f(10.0f, 10.0f);    // pivot
    v1 = Vec2f(0.0f, 0.0f);
    v2 = Vec2f(0.0f, 0.0f);
    v1 = v1.RotateAround(v3, jpt::ToRadians(45.0f));
    JPT_ENSURE(v1 == Vec2f(10.0f, -4.14213562f));
    JPT_ENSURE(v2.RotateAround(v3, jpt::ToRadians(45.0f)) == Vec2f(10.0f, -4.14213562f))

    return true;
}

export bool RunUnitTests_Vector2()
{
    JPT_ENSURE(UnitTests_Vec2f_Length());
    JPT_ENSURE(UnitTests_Vec2f_Distance());
    JPT_ENSURE(UnitTests_Vec2f_Operations());
    JPT_ENSURE(UnitTests_Vec2f_Functionalities());
    JPT_ENSURE(RunUnitTests_Vec2f_Angle());
    JPT_ENSURE(RunUnitTests_Vec2f_Rotation());
    
    return true;
}
