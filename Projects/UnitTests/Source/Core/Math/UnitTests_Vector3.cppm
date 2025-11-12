// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Vector3;

import jpt.Vector3;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;
import jpt.Constants;

static bool UnitTests_Vec3f_Length()
{
    Vec3f v(1.0f, 2.0f, 2.0f);
    JPT_ENSURE(v.Length() == 3.0f);
    JPT_ENSURE(v.Length2() == 9.0f);
    
    Vec3f zero(0.0f, 0.0f, 0.0f);
    JPT_ENSURE(zero.Length() == 0.0f);
    JPT_ENSURE(zero.Length2() == 0.0f);
    
    Vec3f unitX(1.0f, 0.0f, 0.0f);
    JPT_ENSURE(unitX.Length() == 1.0f);
    JPT_ENSURE(unitX.Length2() == 1.0f);
    
    Vec3f unitY(0.0f, 1.0f, 0.0f);
    JPT_ENSURE(unitY.Length() == 1.0f);
    JPT_ENSURE(unitY.Length2() == 1.0f);
    
    Vec3f unitZ(0.0f, 0.0f, 1.0f);
    JPT_ENSURE(unitZ.Length() == 1.0f);
    JPT_ENSURE(unitZ.Length2() == 1.0f);

    Vec3f negative(-1.0f, -2.0f, -2.0f);
    JPT_ENSURE(negative.Length() == 3.0f);
    JPT_ENSURE(negative.Length2() == 9.0f);
    
    return true;
}

static bool UnitTests_Vec3f_Distance()
{
    // Same point
    Vec3f a(1.0f, 2.0f, 3.0f);
    Vec3f b(1.0f, 2.0f, 3.0f);
    JPT_ENSURE(a.Distance(b) == 0.0f);
    JPT_ENSURE(a.Distance2(b) == 0.0f);

    // Distance along X axis
    Vec3f origin(0.0f, 0.0f, 0.0f);
    Vec3f xPoint(3.0f, 0.0f, 0.0f);
    JPT_ENSURE(origin.Distance(xPoint) == 3.0f);
    JPT_ENSURE(origin.Distance2(xPoint) == 9.0f);

    // Distance along Y axis
    Vec3f yPoint(0.0f, 4.0f, 0.0f);
    JPT_ENSURE(origin.Distance(yPoint) == 4.0f);
    JPT_ENSURE(origin.Distance2(yPoint) == 16.0f);

    // Distance along Z axis
    Vec3f zPoint(0.0f, 0.0f, 5.0f);
    JPT_ENSURE(origin.Distance(zPoint) == 5.0f);
    JPT_ENSURE(origin.Distance2(zPoint) == 25.0f);

    // 3D distance (Pythagorean theorem: sqrt(1^2 + 2^2 + 2^2) = 3)
    Vec3f p1(0.0f, 0.0f, 0.0f);
    Vec3f p2(1.0f, 2.0f, 2.0f);
    JPT_ENSURE(p1.Distance(p2) == 3.0f);
    JPT_ENSURE(p1.Distance2(p2) == 9.0f);

    // Negative coordinates
    Vec3f neg1(-1.0f, -2.0f, -2.0f);
    Vec3f neg2(0.0f, 0.0f, 0.0f);
    JPT_ENSURE(neg1.Distance(neg2) == 3.0f);
    JPT_ENSURE(neg1.Distance2(neg2) == 9.0f);

    // Symmetry test (distance A to B = distance B to A)
    Vec3f v1(1.0f, 2.0f, 3.0f);
    Vec3f v2(4.0f, 6.0f, 8.0f);
    JPT_ENSURE(v1.Distance(v2) == v2.Distance(v1));
    JPT_ENSURE(v1.Distance2(v2) == v2.Distance2(v1));

    // Known distance: (0,0,0) to (3,4,0) = 5
    Vec3f xyOrigin(0.0f, 0.0f, 0.0f);
    Vec3f xyPoint(3.0f, 4.0f, 0.0f);
    JPT_ENSURE(xyOrigin.Distance(xyPoint) == 5.0f);
    JPT_ENSURE(xyOrigin.Distance2(xyPoint) == 25.0f);

    return true;
}

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

    angle = Vec3::Angle(from, to);
    JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi));

    // y to z
    from = Vec3::Up();
    to = Vec3::Forward();

    angle = Vec3::Angle(from, to);
    JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi));

    // z to x
    from = Vec3::Forward();
    to = Vec3::Right();

    angle = Vec3::Angle(from, to);
    JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi));

    // 45 degrees
    // x to x + y
    from = Vec3::Right();
    to = Vec3::Right() + Vec3::Up();

    angle = Vec3::Angle(from, to.Normalized());
    JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi / 2.0f));

    // y to y + z
    from = Vec3::Up();
    to = Vec3::Up() + Vec3::Forward();

    angle = Vec3::Angle(from, to.Normalized());
    JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi / 2.0f));

    // z to z + x
    from = Vec3::Forward();
    to = Vec3::Forward() + Vec3::Right();

    angle = Vec3::Angle(from, to.Normalized());
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

    angle = Vec3::AngleSigned(from, to, Vec3::Right());
    JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

    angle = Vec3::AngleSigned(from, to, Vec3::Up());
    JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

    angle = Vec3::AngleSigned(from, to, Vec3::Forward());
    JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi));

    // y to z
    from = Vec3::Up();
    to = Vec3::Forward();

    angle = Vec3::AngleSigned(from, to, Vec3::Up());
    JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

    angle = Vec3::AngleSigned(from, to, Vec3::Forward());
    JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

    angle = Vec3::AngleSigned(from, to, Vec3::Right());
    JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi));

    // z to x
    from = Vec3::Forward();
    to = Vec3::Right();

    angle = Vec3::AngleSigned(from, to, Vec3::Forward());
    JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

    angle = Vec3::AngleSigned(from, to, Vec3::Right());
    JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

    angle = Vec3::AngleSigned(from, to, Vec3::Up());
    JPT_ENSURE(jpt::AreValuesClose(angle, HalfPi));

    // -90 degrees
    // x to -y
    from = Vec3::Right();
    to   = Vec3::Down();

    angle = Vec3::AngleSigned(from, to, Vec3::Right());
    JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

    angle = Vec3::AngleSigned(from, to, Vec3::Forward());
    JPT_ENSURE(jpt::AreValuesClose(angle, -HalfPi, static_cast<Precision>(0.005f)));

    // y to -z
    from = Vec3::Up();
    to = Vec3::Backward();

    angle = Vec3::AngleSigned(from, to, Vec3::Up());
    JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

    angle = Vec3::AngleSigned(from, to, Vec3::Forward());
    JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

    angle = Vec3::AngleSigned(from, to, Vec3::Right());
    JPT_ENSURE(jpt::AreValuesClose(angle, -HalfPi, static_cast<Precision>(0.005f)));

    // z to -x
    from = Vec3::Forward();
    to = Vec3::Left();

    angle = Vec3::AngleSigned(from, to, Vec3::Forward());
    JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

    angle = Vec3::AngleSigned(from, to, Vec3::Right());
    JPT_ENSURE(jpt::AreValuesClose(angle, 0.0f));

    angle = Vec3::AngleSigned(from, to, Vec3::Up());
    JPT_ENSURE(jpt::AreValuesClose(angle, -HalfPi, static_cast<Precision>(0.005f)));

    return true;
}

static bool UnitTests_Vec3f_Math()
{
    Vec3f v1(-30.0f,  90.0f,  125.0f);
    Vec3f v2(  0.0f, -66.0f, -222.0f);
    Vec3f v3(0.0f, HalfPi, Pi);

    // To Radians
    JPT_ENSURE(jpt::ToRadians(v1) == Vec3f(-0.5235988f, 1.570796f, 2.181661f));

    // To Degrees
    JPT_ENSURE(jpt::ToDegrees(v3) == Vec3f(0.0f, 90.0f, 180.0f));

    // Lerp
    JPT_ENSURE(jpt::Lerp(v1, v2, 0.0f) == v1);
    JPT_ENSURE(jpt::Lerp(v1, v2, 0.25f) == Vec3f(-22.5f, 51.0f, 38.25f));
    JPT_ENSURE(jpt::Lerp(v1, v2, 0.5f) == Vec3f(-15.0f, 12.0f, -48.5f));
    JPT_ENSURE(jpt::Lerp(v1, v2, 0.75f) == Vec3f(-7.5f, -27.0f, -135.25f));
    JPT_ENSURE(jpt::Lerp(v1, v2, 1.0f) == v2);

    return true;
}

export bool RunUnitTests_Vector3()
{
    JPT_ENSURE(UnitTests_Vec3f_Length());
    JPT_ENSURE(UnitTests_Vec3f_Distance());
    JPT_ENSURE(UnitTests_Vec3f_Operations());
    JPT_ENSURE(UnitTests_Vec3f_Cross());
    JPT_ENSURE(UnitTests_Vec3f_Angle());
    JPT_ENSURE(UnitTests_Vec3f_AngleSigned());
    JPT_ENSURE(UnitTests_Vec3f_Math());
   
    return true;
}
