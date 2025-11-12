// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

export module UnitTests_Quaternion;

import jpt.Quaternion;
import jpt.Utilities;
import jpt.Math;
import jpt.Matrix44;
import jpt.Vector3;

bool operator==(const Quaternion& lhs, const glm::quat& rhs)
{
    return jpt::AreValuesClose(lhs.x, rhs.x) &&
        jpt::AreValuesClose(lhs.y, rhs.y) &&
        jpt::AreValuesClose(lhs.z, rhs.z) &&
        jpt::AreValuesClose(lhs.w, rhs.w);
}

bool UnitTests_Quaternion_Identity()
{
    // Test identity quaternion
    Quaternion identity = Quaternion::Identity();
    JPT_ENSURE(identity.x == 0.0f);
    JPT_ENSURE(identity.y == 0.0f);
    JPT_ENSURE(identity.z == 0.0f);
    JPT_ENSURE(identity.w == 1.0f);

    // Identity should have length 1
    JPT_ENSURE(jpt::AreValuesClose(identity.Length(), 1.0f));
    JPT_ENSURE(jpt::AreValuesClose(identity.Length2(), 1.0f));

    // Identity multiplied by any quaternion should return that quaternion
    Quaternion test(0.5f, 0.3f, 0.2f, 0.8f);
    test.Normalize();
    Quaternion result = identity * test;
    JPT_ENSURE(result == test);

    result = test * identity;
    JPT_ENSURE(result == test);

    return true;
}

bool UnitTests_Quaternion_Construction()
{
    // Test default constructor
    Quaternion quat1;
    JPT_ENSURE(quat1 == Quaternion::Identity());

    // Test parameterized constructor
    Quaternion quat2(1.0f, 2.0f, 3.0f, 4.0f);
    JPT_ENSURE(quat2.x == 1.0f);
    JPT_ENSURE(quat2.y == 2.0f);
    JPT_ENSURE(quat2.z == 3.0f);
    JPT_ENSURE(quat2.w == 4.0f);

    return true;
}

bool UnitTests_Quaternion_Normalize()
{
    // Test normalization
    Quaternion quat(1.0f, 2.0f, 3.0f, 4.0f);
    Precision originalLength = quat.Length();
    JPT_ENSURE(jpt::AreValuesClose(originalLength, 5.477225575f, 0.001f));

    quat.Normalize();
    JPT_ENSURE(jpt::AreValuesClose(quat.Length(), 1.0f, 0.001f));
    JPT_ENSURE(quat == Quaternion(0.182574f, 0.365148f, 0.547723f, 0.730297f));

    // Test Normalized() doesn't modify original
    Quaternion quat2(2.0f, 3.0f, 4.0f, 5.0f);
    Quaternion normalized = quat2.Normalized();
    JPT_ENSURE(jpt::AreValuesClose(normalized.Length(), 1.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(quat2.Length(), 7.348469f, 0.001f)); // Original unchanged

    // Test normalizing zero quaternion
    Quaternion zero(0.0f, 0.0f, 0.0f, 0.0f);
    zero.Normalize();
    JPT_ENSURE(zero.x == 0.0f && zero.y == 0.0f && zero.z == 0.0f && zero.w == 0.0f);

    return true;
}

bool UnitTests_Quaternion_Multiplication()
{
    // Test quaternion multiplication order matters
    Quaternion q1 = Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(90.0f));
    Quaternion q2 = Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(90.0f));

    Quaternion result1 = q1 * q2;
    Quaternion result2 = q2 * q1;
    JPT_ENSURE(!(result1 == result2)); // Should be different

    // Test multiplication by identity
    Quaternion test(0.5f, 0.3f, 0.2f, 0.8f);
    test.Normalize();
    Quaternion result = test * Quaternion::Identity();
    JPT_ENSURE(result == test);

    // Test compound multiplication
    Quaternion quat = Quaternion::Identity();
    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(30.0f));
    JPT_ENSURE(jpt::AreValuesClose(quat.Length(), 1.0f, 0.001f)); // Should stay normalized

    return true;
}

bool UnitTests_Quaternion_Dot()
{
    // Test dot product
    Quaternion q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion q2(0.0f, 1.0f, 0.0f, 0.0f);
    JPT_ENSURE(jpt::AreValuesClose(q1.Dot(q2), 0.0f));

    Quaternion q3(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion q4(2.0f, 3.0f, 4.0f, 5.0f);
    Precision dot = q3.Dot(q4);
    JPT_ENSURE(jpt::AreValuesClose(dot, 40.0f)); // 1*2 + 2*3 + 3*4 + 4*5 = 40

    // Dot product with itself equals length squared
    Quaternion q5(2.0f, 3.0f, 4.0f, 1.0f);
    JPT_ENSURE(jpt::AreValuesClose(q5.Dot(q5), q5.Length2()));

    // Dot product is commutative
    JPT_ENSURE(jpt::AreValuesClose(q3.Dot(q4), q4.Dot(q3)));

    return true;
}

bool UnitTests_Quaternion_Lerp()
{
    // Test basic lerp
    Quaternion start = Quaternion::Identity();
    Quaternion end = Quaternion(0.707f, 0, 0, 0.707f);
    Quaternion lerpResult = Quaternion::Lerp(start, end, 0.5f);
    JPT_ENSURE(lerpResult == Quaternion(0.3535f, 0, 0, 0.8535f));

    // Test lerp at boundaries
    Quaternion lerp0 = Quaternion::Lerp(start, end, 0.0f);
    JPT_ENSURE(lerp0 == start);

    Quaternion lerp1 = Quaternion::Lerp(start, end, 1.0f);
    JPT_ENSURE(lerp1 == end);

    // Test lerp with different quaternions
    Quaternion q1 = Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(0.0f));
    Quaternion q2 = Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(90.0f));
    Quaternion lerpMid = Quaternion::Lerp(q1, q2, 0.5f);

    // Lerp result should need normalization for proper rotation
    Precision lerpLength = lerpMid.Length();
    JPT_ENSURE(lerpLength < 1.0f || lerpLength > 1.0f); // Usually not unit length

    return true;
}

bool UnitTests_Quaternion_Slerp()
{
    // Test basic slerp
    Quaternion start = Quaternion::Identity();
    Quaternion end = Quaternion(0.707f, 0, 0, 0.707f);
    Quaternion slerpResult = Quaternion::Slerp(start, end, 0.5f);
    JPT_ENSURE(slerpResult == Quaternion(0.382638f, 0, 0, 0.923851f));

    // Test slerp at boundaries
    Quaternion slerp0 = Quaternion::Slerp(start, end, 0.0f);
    JPT_ENSURE(jpt::AreValuesClose(slerp0.x, start.x, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(slerp0.y, start.y, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(slerp0.z, start.z, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(slerp0.w, start.w, 0.001f));

    Quaternion slerp1 = Quaternion::Slerp(start, end, 1.0f);
    JPT_ENSURE(jpt::AreValuesClose(slerp1.x, end.x, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(slerp1.y, end.y, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(slerp1.z, end.z, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(slerp1.w, end.w, 0.001f));

    // Test slerp maintains unit length
    Quaternion q1 = Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(30.0f));
    Quaternion q2 = Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(120.0f));
    Quaternion slerpMid = Quaternion::Slerp(q1, q2, 0.5f);
    JPT_ENSURE(jpt::AreValuesClose(slerpMid.Length(), 1.0f, 0.001f));

    // Test slerp with very close quaternions (should fallback to lerp)
    Quaternion q3 = Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    Quaternion q4 = Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.001f));
    Quaternion slerpClose = Quaternion::Slerp(q3, q4, 0.5f);
    JPT_ENSURE(jpt::AreValuesClose(slerpClose.Length(), 1.0f, 0.001f));

    return true;
}

bool UnitTests_Quaternion_AxisAngle()
{
    Quaternion jptQuat = Quaternion::Identity();
    glm::quat glmQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    JPT_ENSURE(jptQuat == glmQuat);

    // Rotate around X axis
    jptQuat *= Quaternion::FromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), jpt::ToRadians(90.0f));
    glmQuat = glm::rotate(glmQuat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    JPT_ENSURE(jptQuat == glmQuat);

    // Rotate around Y axis
    jptQuat *= Quaternion::FromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), jpt::ToRadians(90.0f));
    glmQuat = glm::rotate(glmQuat, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    JPT_ENSURE(jptQuat == glmQuat);

    // Rotate around Z axis
    jptQuat *= Quaternion::FromAxisAngle(Vec3(0.0f, 0.0f, 1.0f), jpt::ToRadians(45.0f));
    glmQuat = glm::rotate(glmQuat, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    JPT_ENSURE(jptQuat == glmQuat);

    // EulerAngles
    Vec3 eulerAngles(jpt::ToRadians(90.0f), jpt::ToRadians(90.0f), jpt::ToRadians(45.0f));
    Quaternion jptQuat2 = Quaternion::FromEulerAngles(eulerAngles);
    JPT_ENSURE(jptQuat == jptQuat2);
    JPT_ENSURE(glmQuat == jptQuat2);

    // Test zero rotation
    Quaternion zeroRot = Quaternion::FromAxisAngle(Vec3::Right(), 0.0f);
    JPT_ENSURE(zeroRot == Quaternion::Identity());

    // Test 360 degree rotation
    Quaternion fullRot = Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(360.0f));
    JPT_ENSURE(jpt::AreValuesClose(fullRot.w, -1.0f, 0.001f)); // 360° = -identity in quaternion space

    // Test negative rotation
    Quaternion negRot = Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(-45.0f));
    Quaternion posRot = Quaternion::FromAxisAngle(Vec3::Backward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(negRot == posRot); // Should be equivalent

    return true;
}

bool UnitTests_Quaternion_EulerAngles()
{
    // Test that converting to quaternion and back produces equivalent rotation
    // (not necessarily identical Euler angles due to multiple representations)
    Vec3 eulerOriginal(jpt::ToRadians(30.0f), jpt::ToRadians(45.0f), jpt::ToRadians(60.0f));
    Quaternion quat = Quaternion::FromEulerAngles(eulerOriginal);
    
    Matrix44 matQuat = Matrix44::FromQuaternion(quat);
    Matrix44 matEuler = Matrix44::FromEulerAngles(eulerOriginal);
    JPT_ENSURE(matQuat == matEuler);

    // Test that quaternion matches Matrix44 conversion
    Vec3 eulerTest(jpt::ToRadians(30.0f), jpt::ToRadians(45.0f), jpt::ToRadians(60.0f));
    Quaternion quatTest = Quaternion::FromEulerAngles(eulerTest);
    Matrix44 matrixTest = Matrix44::FromEulerAngles(eulerTest);
    Matrix44 matrixFromQuat = Matrix44::FromQuaternion(quatTest);

    // Both should produce the same transformation matrix
    JPT_ENSURE(matrixTest == matrixFromQuat);

    return true;
}

bool UnitTests_Quaternion_Conjugate()
{
    Quaternion quat = Quaternion(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion conjQuat = quat.Conjugate();
    JPT_ENSURE(conjQuat == Quaternion(-1.0f, -2.0f, -3.0f, 4.0f));

    // Test normalized quaternion
    quat = Quaternion(1.0f, 2.0f, 3.0f, 4.0f);
    quat.Normalize();
    conjQuat = quat.Conjugate();
    JPT_ENSURE(conjQuat == Quaternion(-0.182574f, -0.365148f, -0.547723f, 0.730297f));

    // For unit quaternion, q * q* = identity
    quat *= conjQuat;
    JPT_ENSURE(quat == Quaternion::Identity());

    // Test conjugate of identity
    Quaternion identityConj = Quaternion::Identity().Conjugate();
    JPT_ENSURE(identityConj == Quaternion::Identity());

    // Test double conjugate returns original
    Quaternion test(0.5f, 0.3f, 0.2f, 0.8f);
    Quaternion doubleConj = test.Conjugate().Conjugate();
    JPT_ENSURE(doubleConj == test);

    return true;
}

bool UnitTests_Quaternion_Inverse()
{
    // Test inverse of non-unit quaternion
    Quaternion quat = Quaternion(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion invQuat = quat.Inverse();
    JPT_ENSURE(invQuat == Quaternion(-0.033333f, -0.066667f, -0.1f, 0.133333f));

    // q * q^-1 = identity
    Quaternion result = quat * invQuat;
    JPT_ENSURE(jpt::AreValuesClose(result.x, 0.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(result.y, 0.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(result.z, 0.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(result.w, 1.0f, 0.001f));

    // Test inverse of unit quaternion
    quat = Quaternion(1.0f, 2.0f, 3.0f, 4.0f);
    quat.Normalize();
    invQuat = quat.Inverse();
    JPT_ENSURE(invQuat == Quaternion(-0.182574f, -0.365148f, -0.547723f, 0.730297f));

    quat *= invQuat;
    JPT_ENSURE(quat == Quaternion::Identity());

    // For unit quaternion, inverse equals conjugate
    Quaternion unitQuat = Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    Quaternion unitInv = unitQuat.Inverse();
    Quaternion unitConj = unitQuat.Conjugate();
    JPT_ENSURE(unitInv == unitConj);

    // Test inverse of identity
    Quaternion identityInv = Quaternion::Identity().Inverse();
    JPT_ENSURE(identityInv == Quaternion::Identity());

    return true;
}

bool UnitTests_Quaternion_Operators()
{
    // Test addition
    Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion q2(2.0f, 3.0f, 4.0f, 5.0f);
    Quaternion sum = q1 + q2;
    JPT_ENSURE(sum == Quaternion(3.0f, 5.0f, 7.0f, 9.0f));

    // Test subtraction
    Quaternion diff = q2 - q1;
    JPT_ENSURE(diff == Quaternion(1.0f, 1.0f, 1.0f, 1.0f));

    // Test scalar multiplication
    Quaternion scaled = q1 * 2.0f;
    JPT_ENSURE(scaled == Quaternion(2.0f, 4.0f, 6.0f, 8.0f));

    scaled = 2.0f * q1;
    JPT_ENSURE(scaled == Quaternion(2.0f, 4.0f, 6.0f, 8.0f));

    // Test scalar division
    Quaternion divided = q1 / 2.0f;
    JPT_ENSURE(divided == Quaternion(0.5f, 1.0f, 1.5f, 2.0f));

    // Test equality
    Quaternion q3(1.0f, 2.0f, 3.0f, 4.0f);
    JPT_ENSURE(q1 == q3);
    JPT_ENSURE(!(q1 == q2));

    return true;
}

bool UnitTests_Quaternion_Direction_X()
{
    Quaternion quat = Quaternion::Identity();

    // Rotate around Vec3::Right() axis 90 degrees 4 times
    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Up());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Backward());

    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Down());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Forward());

    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Left() axis 90 degrees 4 times
    quat = Quaternion::Identity();
    quat *= Quaternion::FromAxisAngle(Vec3::Left(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Down());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Forward());

    quat *= Quaternion::FromAxisAngle(Vec3::Left(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat *= Quaternion::FromAxisAngle(Vec3::Left(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Up());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Backward());

    quat *= Quaternion::FromAxisAngle(Vec3::Left(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Right() axis -90 degrees 4 times
    quat = Quaternion::Identity();
    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Down());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Forward());

    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Up());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Backward());

    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Right() axis 45 degrees 8 times
    quat = Quaternion::Identity();
    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.0f, 0.707107f, 0.707107f));
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3(0.0f, 0.707107f, -0.707107f));

    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Up());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Backward());

    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.0f, 0.707107f, -0.707107f));
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3(0.0f, -0.707107f, -0.707107f));

    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.0f, -0.707107f, -0.707107f));
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3(0.0f, -0.707107f, 0.707107f));

    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Down());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Forward());

    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.0f, -0.707107f, 0.707107f));
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3(0.0f, 0.707107f, 0.707107f));

    quat *= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    return true;
}

bool UnitTests_Quaternion_Direction_Y()
{
    Quaternion quat = Quaternion::Identity();

    // Rotate around Vec3::Up() axis 90 degrees 4 times
    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Left());
    JPT_ENSURE(quat.Right() == Vec3::Forward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Right());
    JPT_ENSURE(quat.Right() == Vec3::Backward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Down() axis 90 degrees 4 times
    quat = Quaternion::Identity();
    quat *= Quaternion::FromAxisAngle(Vec3::Down(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Right());
    JPT_ENSURE(quat.Right() == Vec3::Backward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Down(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Down(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Left());
    JPT_ENSURE(quat.Right() == Vec3::Forward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Down(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Up() axis -90 degrees 4 times
    quat = Quaternion::Identity();
    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Right());
    JPT_ENSURE(quat.Right() == Vec3::Backward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Left());
    JPT_ENSURE(quat.Right() == Vec3::Forward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Up() axis 45 degrees 8 times
    quat = Quaternion::Identity();
    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(-0.707107f, 0.0f, 0.707107f));
    JPT_ENSURE(quat.Right() == Vec3(0.707107f, 0.0f, 0.707107f));
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Left());
    JPT_ENSURE(quat.Right() == Vec3::Forward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(-0.707107f, 0.0f, -0.707107f));
    JPT_ENSURE(quat.Right() == Vec3(-0.707107f, 0.0f, 0.707107f));
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.707107f, 0.0f, -0.707107f));
    JPT_ENSURE(quat.Right() == Vec3(-0.707107f, 0.0f, -0.707107f));
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Right());
    JPT_ENSURE(quat.Right() == Vec3::Backward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.707107f, 0.0f, 0.707107f));
    JPT_ENSURE(quat.Right() == Vec3(0.707107f, 0.0f, -0.707107f));
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat *= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    return true;
}

bool UnitTests_Quaternion_Direction_Z()
{
    Quaternion quat = Quaternion::Identity();

    // Rotate around Vec3::Forward() axis 90 degrees 4 times
    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Down());
    JPT_ENSURE(quat.Up() == Vec3::Right());

    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Up());
    JPT_ENSURE(quat.Up() == Vec3::Left());

    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Backward() axis 90 degrees 4 times
    quat = Quaternion::Identity();
    quat *= Quaternion::FromAxisAngle(Vec3::Backward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Up());
    JPT_ENSURE(quat.Up() == Vec3::Left());

    quat *= Quaternion::FromAxisAngle(Vec3::Backward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat *= Quaternion::FromAxisAngle(Vec3::Backward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Down());
    JPT_ENSURE(quat.Up() == Vec3::Right());

    quat *= Quaternion::FromAxisAngle(Vec3::Backward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Forward() axis -90 degrees 4 times
    quat = Quaternion::Identity();
    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Up());
    JPT_ENSURE(quat.Up() == Vec3::Left());

    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Down());
    JPT_ENSURE(quat.Up() == Vec3::Right());

    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Forward() axis 45 degrees 8 times
    quat = Quaternion::Identity();
    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3(0.707107f, -0.707107f, 0.0f));
    JPT_ENSURE(quat.Up() == Vec3(0.707107f, 0.707107f, 0.0f));

    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Down());
    JPT_ENSURE(quat.Up() == Vec3::Right());

    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3(-0.707107f, -0.707107f, 0.0f));
    JPT_ENSURE(quat.Up() == Vec3(0.707107f, -0.707107f, 0.0f));

    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3(-0.707107f, 0.707107f, 0.0f));
    JPT_ENSURE(quat.Up() == Vec3(-0.707107f, -0.707107f, 0.0f));

    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Up());
    JPT_ENSURE(quat.Up() == Vec3::Left());

    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3(0.707107f, 0.707107f, 0.0f));
    JPT_ENSURE(quat.Up() == Vec3(-0.707107f, 0.707107f, 0.0f));

    quat *= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    return true;
}

export bool RunUnitTests_Quaternion()
{
    JPT_ENSURE(UnitTests_Quaternion_Identity());
    JPT_ENSURE(UnitTests_Quaternion_Construction());
    JPT_ENSURE(UnitTests_Quaternion_Normalize());
    JPT_ENSURE(UnitTests_Quaternion_Multiplication());
    JPT_ENSURE(UnitTests_Quaternion_Dot());
    JPT_ENSURE(UnitTests_Quaternion_Lerp());
    JPT_ENSURE(UnitTests_Quaternion_Slerp());
    JPT_ENSURE(UnitTests_Quaternion_AxisAngle());
    JPT_ENSURE(UnitTests_Quaternion_EulerAngles());
    JPT_ENSURE(UnitTests_Quaternion_Conjugate());
    JPT_ENSURE(UnitTests_Quaternion_Inverse());
    JPT_ENSURE(UnitTests_Quaternion_Operators());
    JPT_ENSURE(UnitTests_Quaternion_Direction_X());
    JPT_ENSURE(UnitTests_Quaternion_Direction_Y());
    JPT_ENSURE(UnitTests_Quaternion_Direction_Z());

    return true;
}