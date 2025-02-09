// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Vector3;

import jpt.Vector3;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;

static bool UnitTests_Vec3f_Operations()
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

static bool UnitTests_Vec3f_Cross()
{
	Vec3f a = Vec3(1.0f, 0.0f, 0.0f).Normalized();
	Vec3f b = Vec3(0.0f, 0.0f, 1.0f).Normalized();

	Vec3f c = b.Cross(a);
	JPT_ENSURE(c == Vec3f(0.0f, 1.0f, 0.0f));

	c = a.Cross(b);
	JPT_ENSURE(c == Vec3f(0.0f, -1.0f, 0.0f));

    return true;
}

export bool RunUnitTests_Vector3()
{
    JPT_ENSURE(UnitTests_Vec3f_Operations());
    JPT_ENSURE(UnitTests_Vec3f_Cross());

    return true;
}
