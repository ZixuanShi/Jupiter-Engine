// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Vector2;

import jpt.Vector2;
import jpt.Utilities;
import jpt.TypeDefs;
import jpt.Math;

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
    JPT_ENSURE(Vec2f::Lerp(v, Vec2f(4.0f, 4.0f), 0.5f) == Vec2f(3.0f, 3.0f));

    return true;
}

bool RunUnitTests_Vector2_Angle()
{
	Vec2f source(0.0f, 1.0f);
	Vec2f target(1.0f, 1.0f);

	JPT_ENSURE(jpt::AreValuesClose(Vec2f::Angle(source, target), jpt::ToRadians(45.0f)));
	JPT_ENSURE(jpt::AreValuesClose(Vec2f::AngleSigned(source, target), jpt::ToRadians(-45.0f)));

	Vec2 vector1(1.0f, 0.0f);
	Vec2 vector2(1.0f, 0.0f);
	vector1.Rotate(jpt::ToRadians(45.0f));
	JPT_ENSURE(jpt::AreValuesClose(vector1.Angle(vector2), jpt::ToRadians(45.0f)));

    return true;
}

bool RunUnitTests_Vector2_Rotation()
{
    Vec2f v1(0.0f, 2.0f);
    Vec2f v2(1.0f, -1.0f);
    Vec2f v3(-1.0f, -1.0f);

    v1.Rotate(jpt::ToRadians(49.0f));
    v2.Rotate(jpt::ToRadians(49.0f));
    v3.Rotate(jpt::ToRadians(49.0f));
    JPT_ENSURE(v1 == Vec2f(-1.5f, 1.3f));
    JPT_ENSURE(v2 == Vec2f(1.4f, 0.1f));
    JPT_ENSURE(v3 == Vec2f(0.1f, -1.4f));

    JPT_ENSURE(Vec2f::Rotate(v1, jpt::ToRadians(-49.0f)) == Vec2f(0.0f, 2.0f));
    JPT_ENSURE(Vec2f::Rotate(v2, jpt::ToRadians(-49.0f)) == Vec2f(1.0f, -1.0f));
    JPT_ENSURE(Vec2f::Rotate(v3, jpt::ToRadians(-49.0f)) == Vec2f(-1.0f, -1.0f));

    v1.Rotate(jpt::ToRadians(360.0f));
    v2.Rotate(jpt::ToRadians(360.0f));
    v3.Rotate(jpt::ToRadians(360.0f));
    JPT_ENSURE(v1 == Vec2f(-1.5f, 1.3f));
    JPT_ENSURE(v2 == Vec2f(1.4f, 0.1f));
    JPT_ENSURE(v3 == Vec2f(0.1f, -1.4f));

    v3 = Vec2f(10.0f, 10.0f);	// pivot
    v1 = Vec2f(0.0f, 0.0f);
    v2 = Vec2f(0.0f, 0.0f);
    v1.RotateAround(v3, jpt::ToRadians(45.0f));
    JPT_ENSURE(v1 == Vec2f(10.0f, -4.14213562f));
    JPT_ENSURE(Vec2f::RotateAround(v2, v3, jpt::ToRadians(45.0f)) == Vec2f(10.0f, -4.14213562f))

    return true;
}

export bool RunUnitTests_Vector2()
{
    JPT_ENSURE(UnitTests_Vec2f_Operations());
    JPT_ENSURE(UnitTests_Vec2f_Functionalities());
    JPT_ENSURE(RunUnitTests_Vector2_Angle());
    JPT_ENSURE(RunUnitTests_Vector2_Rotation());
    
    return true;
}
