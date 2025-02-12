// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Vector3;

import jpt.Vector3;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;
import jpt.Constants;

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
	Vec3f i = Vec3(1.0f, 0.0f, 0.0f).Normalized();
	Vec3f k = Vec3(0.0f, 0.0f, 1.0f).Normalized();

	Vec3f j = k.Cross(i);
	JPT_ENSURE(j == Vec3f(0.0f, 1.0f, 0.0f));

	j = i.Cross(k);
	JPT_ENSURE(j == Vec3f(0.0f, -1.0f, 0.0f));

    return true;
}

static bool UnitTests_Vec3f_Angle()
{
	Vec3f from;
	Vec3f to;
	Precision angle = 0.0f;

	// 90 degrees
	// x to y
	from = Vec3::Right();
	to = Vec3::Up();

	angle = from.Angle(to);
	JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi));

	// y to z
	from = Vec3::Up();
	to = Vec3::Forward();

	angle = from.Angle(to);
	JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi));

	// z to x
	from = Vec3::Forward();
	to = Vec3::Right();

	angle = from.Angle(to);
	JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi));

	// 45 degrees
	// x to x + y
	from = Vec3::Right();
	to = Vec3::Right() + Vec3::Up();

	angle = from.Angle(to);
	JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi / 2.0f));

	// y to y + z
	from = Vec3::Up();
	to = Vec3::Up() + Vec3::Forward();

	angle = from.Angle(to);
	JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi / 2.0f));

	// z to z + x
	from = Vec3::Forward();
	to = Vec3::Forward() + Vec3::Right();

	angle = from.Angle(to);
	JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi / 2.0f));

	return true;
}

static bool UnitTests_Vec3f_AngleSigned()
{
	Vec3f from;
	Vec3f to;
    Precision angle = 0.0f;

	// 90 degrees
    // x to y
	from = Vec3::Right();
	to = Vec3::Up();

    angle = from.AngleSigned(to, Vec3::Right());
    JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

	angle = from.AngleSigned(to, Vec3::Up());
	JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

	angle = from.AngleSigned(to, Vec3::Forward());
	JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi));

    // y to z
	from = Vec3::Up();
	to = Vec3::Forward();

	angle = from.AngleSigned(to, Vec3::Up());
	JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

	angle = from.AngleSigned(to, Vec3::Forward());
	JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

	angle = from.AngleSigned(to, Vec3::Right());
	JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi));

	// z to x
	from = Vec3::Forward();
	to = Vec3::Right();

	angle = from.AngleSigned(to, Vec3::Forward());
	JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

	angle = from.AngleSigned(to, Vec3::Right());
	JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

	angle = from.AngleSigned(to, Vec3::Up());
	JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi));

	// -90 degrees
	// x to -y
	from = Vec3::Right();
	to   = Vec3::Down();

	angle = from.AngleSigned(to, Vec3::Right());
	JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

	angle = from.AngleSigned(to, Vec3::Up());
	JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

	angle = from.AngleSigned(to, Vec3::Forward());
	JPT_ENSURE(jpt::AreValuesClose(angle, -HalfPi, static_cast<Precision>(0.005f)));

	// y to -z
	from = Vec3::Up();
	to = Vec3::Backward();

	angle = from.AngleSigned(to, Vec3::Up());
	JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

	angle = from.AngleSigned(to, Vec3::Forward());
	JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

	angle = from.AngleSigned(to, Vec3::Right());
	JPT_ENSURE(jpt::AreValuesClose(angle, -HalfPi, static_cast<Precision>(0.005f)));

	// z to -x
	from = Vec3::Forward();
	to = Vec3::Left();

	angle = from.AngleSigned(to, Vec3::Forward());
	JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

	angle = from.AngleSigned(to, Vec3::Right());
	JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

	angle = from.AngleSigned(to, Vec3::Up());
	JPT_ENSURE(jpt::AreValuesClose(angle, -HalfPi, static_cast<Precision>(0.005f)));

    return true;
}

export bool RunUnitTests_Vector3()
{
    JPT_ENSURE(UnitTests_Vec3f_Operations());
    JPT_ENSURE(UnitTests_Vec3f_Cross());
	JPT_ENSURE(UnitTests_Vec3f_Angle());
	JPT_ENSURE(UnitTests_Vec3f_AngleSigned());

    return true;
}
