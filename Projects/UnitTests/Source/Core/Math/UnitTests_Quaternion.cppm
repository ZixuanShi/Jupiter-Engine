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

bool operator==(const Quaternion& lhs, const glm::quat& rhs)
{
    return jpt::AreValuesClose(lhs.x, rhs.x) && 
           jpt::AreValuesClose(lhs.y, rhs.y) && 
           jpt::AreValuesClose(lhs.z, rhs.z) && 
           jpt::AreValuesClose(lhs.w, rhs.w);
}

bool UnitTests_Quaternion_Lerp()
{
    Quaternion quaternion = Quaternion::Identity();
    Quaternion quaternion2 = Quaternion(0.707f, 0, 0, 0.707f);
    Quaternion lerpResult = Quaternion::Lerp(quaternion, quaternion2, 0.5f);
    JPT_ENSURE(lerpResult == Quaternion(0.353500009f, 0, 0, 0.853500009f));

    return true;
}

bool UnitTests_Quaternion_SLerp()
{
    Quaternion quaternion = Quaternion::Identity();
    Quaternion quaternion2 = Quaternion(0.707f, 0, 0, 0.707f);
    Quaternion slerpResult = Quaternion::Slerp(quaternion, quaternion2, 0.5f);
    JPT_ENSURE(slerpResult == Quaternion(0.382637650f, 0, 0, 0.923850656f));

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

    quat*= Quaternion::FromAxisAngle(Vec3::Left(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat*= Quaternion::FromAxisAngle(Vec3::Left(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Up());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Backward());

    quat*= Quaternion::FromAxisAngle(Vec3::Left(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Right() axis -90 degrees 4 times
    quat = Quaternion::Identity();
    quat*= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Down());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Forward());

    quat*= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat*= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Up());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Backward());

    quat*= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Right() axis 45 degrees 8 times
    quat = Quaternion::Identity();
    quat*= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.0f, 0.707106829f, 0.707106709f));
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3(0.0f, 0.707106709f, -0.707106829f));

    quat*= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Up());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Backward());

    quat*= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.0f, 0.707106590f, -0.707106948f));
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3(0.0f, -0.707106948f, -0.707106590f));

    quat*= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat*= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.0f, -0.707107008f, -0.707106471f));
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3(0.0f, -0.707106471f, 0.707107008f));

    quat*= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Down());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Forward());

    quat*= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.0f, -0.707106292f, 0.707107246f));
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3(0.0f, 0.707107246f, 0.707106292f));

    quat*= Quaternion::FromAxisAngle(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    return true;
}

bool UnitTests_Quaternion_Direction_Y()
{
    Quaternion quat = Quaternion::Identity();

    // Rotate around Vec3::Up() axis 90 degrees 4 times
    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Left());
    JPT_ENSURE(quat.Right() == Vec3::Forward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Right());
    JPT_ENSURE(quat.Right() == Vec3::Backward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Down() axis 90 degrees 4 times
    quat = Quaternion::Identity();
    quat*= Quaternion::FromAxisAngle(Vec3::Down(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Right());
    JPT_ENSURE(quat.Right() == Vec3::Backward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Down(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Down(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Left());
    JPT_ENSURE(quat.Right() == Vec3::Forward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Down(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Up() axis -90 degrees 4 times
    quat = Quaternion::Identity();
    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Right());
    JPT_ENSURE(quat.Right() == Vec3::Backward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Left());
    JPT_ENSURE(quat.Right() == Vec3::Forward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Up() axis 45 degrees 8 times
    quat = Quaternion::Identity();
    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(-0.707106829f, 0.0f, 0.707106709f));
    JPT_ENSURE(quat.Right() == Vec3(0.707106709f, 0.0f, 0.707106829f));
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Left());
    JPT_ENSURE(quat.Right() == Vec3::Forward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(-0.707106590f, 0.0f, -0.707106948f));
    JPT_ENSURE(quat.Right() == Vec3(-0.707106948f, 0.0f, 0.707106590f));
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.707107008f, 0.0f, -0.707106471f));
    JPT_ENSURE(quat.Right() == Vec3(-0.707106471f, 0.0f, -0.707107008f));
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Right());
    JPT_ENSURE(quat.Right() == Vec3::Backward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.707106292f, 0.0f, 0.707107246f));
    JPT_ENSURE(quat.Right() == Vec3(0.707107246f, 0.0f, -0.707106292f));
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat*= Quaternion::FromAxisAngle(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    return true;
}

bool UnitTests_Quaternion_Direction_Z()
{
    Quaternion quat = Quaternion::Identity();

    // Rotate around Vec3::Forward() axis 90 degrees 4 times
    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Down());  // 
    JPT_ENSURE(quat.Up() == Vec3::Right());

    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Up());
    JPT_ENSURE(quat.Up() == Vec3::Left());

    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Backward() axis 90 degrees 4 times
    quat = Quaternion::Identity();
    quat*= Quaternion::FromAxisAngle(Vec3::Backward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Up());
    JPT_ENSURE(quat.Up() == Vec3::Left());

    quat*= Quaternion::FromAxisAngle(Vec3::Backward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat*= Quaternion::FromAxisAngle(Vec3::Backward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Down());
    JPT_ENSURE(quat.Up() == Vec3::Right());

    quat*= Quaternion::FromAxisAngle(Vec3::Backward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Forward() axis -90 degrees 4 times
    quat = Quaternion::Identity();
    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Up());
    JPT_ENSURE(quat.Up() == Vec3::Left());

    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Down());
    JPT_ENSURE(quat.Up() == Vec3::Right());

    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Forward() axis 45 degrees 8 times
    quat = Quaternion::Identity();
    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3(0.707106709f, -0.707106829f, 0.0f));
    JPT_ENSURE(quat.Up() == Vec3(0.707106829f, 0.707106709f, 0.0f));

    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Down());
    JPT_ENSURE(quat.Up() == Vec3::Right());

    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3(-0.707106948f, -0.707106590f, 0.0f));
    JPT_ENSURE(quat.Up() == Vec3(0.707106590f, -0.707106948f, 0.0f));

    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3(-0.707106471f, 0.707107008f, 0.0f));
    JPT_ENSURE(quat.Up() == Vec3(-0.707107008f, -0.707106471f, 0.0f));

    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Up());
    JPT_ENSURE(quat.Up() == Vec3::Left());

    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3(0.707107246f, 0.707106292f, 0.0f));
    JPT_ENSURE(quat.Up() == Vec3(-0.707106292f, 0.707107246f, 0.0f));

    quat*= Quaternion::FromAxisAngle(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    return true;
}

bool UnitTests_Quaternion_Conjugate()
{
    Quaternion quat = Quaternion(1.0f, 2.0f, 3.0f, 4.0f);

    Quaternion conjQuat = quat.Conjugate();
    JPT_ENSURE(conjQuat == Quaternion(-1.0f, -2.0f, -3.0f, 4.0f));

    quat = Quaternion(1.0f, 2.0f, 3.0f, 4.0f);
    quat.Normalize();
    conjQuat = quat.Conjugate();
    JPT_ENSURE(conjQuat == Quaternion(-0.182574174f, -0.36514837f, -0.547722578f, 0.730296731f));

    quat *= conjQuat;
    JPT_ENSURE(quat == Quaternion::Identity());

    return true;
}

bool UnitTests_Quaternion_Inverse()
{
    Quaternion quat = Quaternion(1.0f, 2.0f, 3.0f, 4.0f);

    Quaternion invQuat = quat.Inverse();
    JPT_ENSURE(invQuat == Quaternion(-0.0333333351f, -0.0666666701f, -0.1f, 0.13333334f));

    quat = Quaternion(1.0f, 2.0f, 3.0f, 4.0f);
    quat.Normalize();
    invQuat = quat.Inverse();
    JPT_ENSURE(invQuat == Quaternion(-0.182574174f, -0.36514837f, -0.547722578f, 0.730296731f));

    quat *= invQuat;
    JPT_ENSURE(quat == Quaternion::Identity());

    return true;
}

export bool RunUnitTests_Quaternion()
{
    JPT_ENSURE(UnitTests_Quaternion_Lerp());
    JPT_ENSURE(UnitTests_Quaternion_SLerp());
    JPT_ENSURE(UnitTests_Quaternion_AxisAngle());
    JPT_ENSURE(UnitTests_Quaternion_Conjugate());
    JPT_ENSURE(UnitTests_Quaternion_Inverse());

    JPT_ENSURE(UnitTests_Quaternion_Direction_X());
    JPT_ENSURE(UnitTests_Quaternion_Direction_Y());
    JPT_ENSURE(UnitTests_Quaternion_Direction_Z());

    return true;
}