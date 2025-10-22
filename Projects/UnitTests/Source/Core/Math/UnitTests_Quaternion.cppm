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
    JPT_ENSURE(lerpResult == Quaternion(0.354f, 0, 0, 0.854f));

    return true;
}

bool UnitTests_Quaternion_SLerp()
{
    Quaternion quaternion = Quaternion::Identity();
    Quaternion quaternion2 = Quaternion(0.707f, 0, 0, 0.707f);
    Quaternion slerpResult = Quaternion::Slerp(quaternion, quaternion2, 0.5f);
    JPT_ENSURE(slerpResult == Quaternion(0.383f, 0, 0, 0.924f));

    return true;
}

bool UnitTests_Quaternion_EulerAngles()
{
    Quaternion jptQuat = Quaternion::Identity();
    glm::quat glmQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    JPT_ENSURE(jptQuat == glmQuat);

    // Rotate around Y axis
    jptQuat.RotateEulerAngles(Vec3(0.0f, 1.0f, 0.0f), jpt::ToRadians(90.0f));
    glmQuat = glm::rotate(glmQuat, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    JPT_ENSURE(jptQuat == glmQuat);

    // Rotate around X axis
    jptQuat.RotateEulerAngles(Vec3(1.0f, 0.0f, 0.0f), jpt::ToRadians(90.0f));
    glmQuat = glm::rotate(glmQuat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    JPT_ENSURE(jptQuat == glmQuat);

    // Rotate around Z axis
    jptQuat.RotateEulerAngles(Vec3(0.0f, 0.0f, 1.0f), jpt::ToRadians(45.0f));
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
    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Up());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Backward());

    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Down());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Forward());

    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Left() axis 90 degrees 4 times
    quat = Quaternion::Identity();
    quat.RotateEulerAngles(Vec3::Left(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Down());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Forward());

    quat.RotateEulerAngles(Vec3::Left(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat.RotateEulerAngles(Vec3::Left(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Up());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Backward());

    quat.RotateEulerAngles(Vec3::Left(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Right() axis -90 degrees 4 times
    quat = Quaternion::Identity();
    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Down());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Forward());

    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Up());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Backward());

    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Right() axis 45 degrees 8 times
    quat = Quaternion::Identity();
    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.0f, 0.707f, 0.707f));
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3(0.0f, 0.707f, -0.707f));

    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Up());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Backward());

    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.0f, 0.707f, -0.707f));
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3(0.0f, -0.707f, -0.707f));

    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.0f, -0.707f, -0.707f));
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3(0.0f, -0.707f, 0.707f));

    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Down());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Forward());

    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.0f, -0.707f, 0.707f));
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3(0.0f, 0.707f, 0.707f));

    quat.RotateEulerAngles(Vec3::Right(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    return true;
}

bool UnitTests_Quaternion_Direction_Y()
{
    Quaternion quat = Quaternion::Identity();

    // Rotate around Vec3::Up() axis 90 degrees 4 times
    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Left());
    JPT_ENSURE(quat.Right() == Vec3::Forward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Right());
    JPT_ENSURE(quat.Right() == Vec3::Backward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Down() axis 90 degrees 4 times
    quat = Quaternion::Identity();
    quat.RotateEulerAngles(Vec3::Down(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Right());
    JPT_ENSURE(quat.Right() == Vec3::Backward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Down(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Down(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Left());
    JPT_ENSURE(quat.Right() == Vec3::Forward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Down(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Up() axis -90 degrees 4 times
    quat = Quaternion::Identity();
    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Right());
    JPT_ENSURE(quat.Right() == Vec3::Backward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Left());
    JPT_ENSURE(quat.Right() == Vec3::Forward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Up() axis 45 degrees 8 times
    quat = Quaternion::Identity();
    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(-0.707f, 0.0f, 0.707f));
    JPT_ENSURE(quat.Right() == Vec3(0.707f, 0.0f, 0.707f));
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Left());
    JPT_ENSURE(quat.Right() == Vec3::Forward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(-0.707f, 0.0f, -0.707f));
    JPT_ENSURE(quat.Right() == Vec3(-0.707f, 0.0f, 0.707f));
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Backward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.707f, 0.0f, -0.707f));
    JPT_ENSURE(quat.Right() == Vec3(-0.707f, 0.0f, -0.707f));
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Right());
    JPT_ENSURE(quat.Right() == Vec3::Backward());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3(0.707f, 0.0f, 0.707f));
    JPT_ENSURE(quat.Right() == Vec3(0.707f, 0.0f, -0.707f));
    JPT_ENSURE(quat.Up() == Vec3::Up());

    quat.RotateEulerAngles(Vec3::Up(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    return true;
}

bool UnitTests_Quaternion_Direction_Z()
{
    Quaternion quat = Quaternion::Identity();

    // Rotate around Vec3::Forward() axis 90 degrees 4 times
    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Down());  // 
    JPT_ENSURE(quat.Up() == Vec3::Right());

    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Up());
    JPT_ENSURE(quat.Up() == Vec3::Left());

    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Backward() axis 90 degrees 4 times
    quat = Quaternion::Identity();
    quat.RotateEulerAngles(Vec3::Backward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Up());
    JPT_ENSURE(quat.Up() == Vec3::Left());

    quat.RotateEulerAngles(Vec3::Backward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat.RotateEulerAngles(Vec3::Backward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Down());
    JPT_ENSURE(quat.Up() == Vec3::Right());

    quat.RotateEulerAngles(Vec3::Backward(), jpt::ToRadians(90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Forward() axis -90 degrees 4 times
    quat = Quaternion::Identity();
    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Up());
    JPT_ENSURE(quat.Up() == Vec3::Left());

    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Down());
    JPT_ENSURE(quat.Up() == Vec3::Right());

    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(-90.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    // Rotate around Vec3::Forward() axis 45 degrees 8 times
    quat = Quaternion::Identity();
    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3(0.707f, -0.707f, 0.0f));
    JPT_ENSURE(quat.Up() == Vec3(0.707f, 0.707f, 0.0f));

    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Down());
    JPT_ENSURE(quat.Up() == Vec3::Right());

    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3(-0.707f, -0.707f, 0.0f));
    JPT_ENSURE(quat.Up() == Vec3(0.707f, -0.707f, 0.0f));

    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Left());
    JPT_ENSURE(quat.Up() == Vec3::Down());

    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3(-0.707f, 0.707f, 0.0f));
    JPT_ENSURE(quat.Up() == Vec3(-0.707f, -0.707f, 0.0f));

    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Up());
    JPT_ENSURE(quat.Up() == Vec3::Left());

    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3(0.707f, 0.707f, 0.0f));
    JPT_ENSURE(quat.Up() == Vec3(-0.707f, 0.707f, 0.0f));

    quat.RotateEulerAngles(Vec3::Forward(), jpt::ToRadians(45.0f));
    JPT_ENSURE(quat.Forward() == Vec3::Forward());
    JPT_ENSURE(quat.Right() == Vec3::Right());
    JPT_ENSURE(quat.Up() == Vec3::Up());

    return true;
}

export bool RunUnitTests_Quaternion()
{
    JPT_ENSURE(UnitTests_Quaternion_Lerp());
    JPT_ENSURE(UnitTests_Quaternion_SLerp());
    JPT_ENSURE(UnitTests_Quaternion_EulerAngles());

    JPT_ENSURE(UnitTests_Quaternion_Direction_X());
    JPT_ENSURE(UnitTests_Quaternion_Direction_Y());
    JPT_ENSURE(UnitTests_Quaternion_Direction_Z());

    return true;
}