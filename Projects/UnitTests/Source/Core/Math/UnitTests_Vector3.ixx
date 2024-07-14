// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Vector3;

import jpt.Vector3;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;

static bool UnitTest_Vec3f_Operations()
{
    Vec3f v(1.0f, 2.0f, 3.0f);

    // Other vector
    JPT_ENSURE(v + Vec3f(3.0f, 2.0f, 1.0f) == Vec3f(4.0f, 4.0f, 4.0f));
    JPT_ENSURE(v - Vec3f(3.0f, 2.0f, 1.0f) == Vec3f(-2.0f, 0.0f, 2.0f));
    JPT_ENSURE(v * Vec3f(3.0f, 2.0f, 1.0f) == Vec3f(3.0f, 4.0f, 3.0f));
    JPT_ENSURE(v / Vec3f(3.0f, 2.0f, 1.0f) == Vec3f(1.0f / 3.0f, 1.0f, 3.0f));

    v += Vec3f(3.0f, 2.0f, 1.0f);
    JPT_ENSURE(v == Vec3f(4.0f, 4.0f, 4.0f));

    v -= Vec3f(3.0f, 2.0f, 1.0f);
    JPT_ENSURE(v == Vec3f(1.0f, 2.0f, 3.0f));

    v *= Vec3f(3.0f, 2.0f, 1.0f);
    JPT_ENSURE(v == Vec3f(3.0f, 4.0f, 3.0f));

    v /= Vec3f(3.0f, 2.0f, 1.0f);
    JPT_ENSURE(v == Vec3f(1.0f, 2.0f, 3.0f));

    // Scalar
    JPT_ENSURE(v + 3.0f == Vec3f(4.0f, 5.0f, 6.0f));
    JPT_ENSURE(v - 3.0f == Vec3f(-2.0f, -1.0f, 0.0f));
    JPT_ENSURE(v * 3.0f == Vec3f(3.0f, 6.0f, 9.0f));
    JPT_ENSURE(v / 3.0f == Vec3f(1.0f / 3.0f, 2.0f / 3.0f, 1.0f));

    v += 3.0f;
    JPT_ENSURE(v == Vec3f(4.0f, 5.0f, 6.0f));

    v -= 3.0f;
    JPT_ENSURE(v == Vec3f(1.0f, 2.0f, 3.0f));

    v *= 3.0f;
    JPT_ENSURE(v == Vec3f(3.0f, 6.0f, 9.0f));

    v /= 3.0f;
    JPT_ENSURE(v == Vec3f(1.0f, 2.0f, 3.0f));

    return true;
}

static bool UnitTest_Vec3f_Functionalities()
{
    Vec3f v(1.0f, 2.0f, 3.0f);

    // Dot product
    JPT_ENSURE(Vec3f::Dot(v, Vec3f(3.0f, 2.0f, 1.0f)) == 10.0f);

    // Cross product
    JPT_ENSURE(Vec3f::Cross(v, Vec3f(3.0f, 2.0f, 1.0f)) == Vec3f(-4.0f, 8.0f, -4.0f));

    // Length
    JPT_ENSURE(jpt::AreValuesClose(v.Length(), 3.74165750f));

    // Length2
    JPT_ENSURE(v.Length2() == 14.0f);

    // Distance
    JPT_ENSURE(jpt::AreValuesClose(Vec3f::Distance(v, Vec3f(3.0f, 2.0f, 1.0f)), 2.82842708f));

    // Distance2
    JPT_ENSURE(jpt::AreValuesClose(Vec3f::Distance2(v, Vec3f(3.0f, 2.0f, 1.0f)), 8.0f));

    // Normalize
    JPT_ENSURE(Vec3f::Normalize(v) == Vec3f(0.26726124f, 0.53452248f, 0.80178373f));

    // Lerp
    JPT_ENSURE(Vec3f::Lerp(v, Vec3f(3.0f, 2.0f, 1.0f), 0.5f) == Vec3f(2.0f, 2.0f, 2.0f));

    return true;
}

export bool RunUnitTests_Vector3()
{
    JPT_ENSURE(UnitTest_Vec3f_Operations());
    JPT_ENSURE(UnitTest_Vec3f_Functionalities());

    return true;
}
