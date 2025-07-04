// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"
#include "Debugging/Logger.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module UnitTests_Matrix33;

import jpt.Matrix33;
import jpt.Vector3;
import jpt.Vector2;
import jpt.Utilities;
import jpt.Math;
import jpt.String;

void LogGlmMatrix(const glm::mat3& m)
{
    const jpt::String str = jpt::String::Format<128>("\n%.3f, %.3f, %.3f\n%.3f, %.3f, %.3f\n%.3f, %.3f, %.3f", 
        m[0][0], m[0][1], m[0][2],
        m[1][0], m[1][1], m[1][2],
        m[2][0], m[2][1], m[2][2]);

    JPT_LOG(str);
}

bool operator==(const Matrix33& lhs, const glm::mat3& rhs)
{
    return jpt::AreValuesClose(lhs.m[0].x, rhs[0].x, 0.05f) && jpt::AreValuesClose(lhs.m[0].y, rhs[0].y, 0.05f) && jpt::AreValuesClose(lhs.m[0].z, rhs[0].z, 0.05f) &&
           jpt::AreValuesClose(lhs.m[1].x, rhs[1].x, 0.05f) && jpt::AreValuesClose(lhs.m[1].y, rhs[1].y, 0.05f) && jpt::AreValuesClose(lhs.m[1].z, rhs[1].z, 0.05f) &&
           jpt::AreValuesClose(lhs.m[2].x, rhs[2].x, 0.05f) && jpt::AreValuesClose(lhs.m[2].y, rhs[2].y, 0.05f) && jpt::AreValuesClose(lhs.m[2].z, rhs[2].z, 0.05f);
}

bool UnitTests_Matrix33_Mul()
{    
    glm::mat3 glmMat1 = glm::mat3(1.0f);
    Matrix33 jptMat1 = Matrix33::Identity();
    JPT_ENSURE(jptMat1 == glmMat1);

    // Rotation Matrix
    glm::mat3 glmMat2 = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glmMat1 *= glmMat2;

    jptMat1.Rotate(jpt::ToRadians(90.0f));    
    JPT_ENSURE(jptMat1 == glmMat1);

    // Vector2 multiplication
    glm::vec3 glmVec1 = glm::vec3(2.0f, 3.0f, 0.0f);
    Vec3 jptVec1 = Vec3(2.0f, 3.0f, 0.0f);

    glm::vec3 glmVec2 = glmMat1 * glmVec1;
    Vec3 jptVec2 = jptMat1 * jptVec1;

    JPT_ENSURE(jpt::AreValuesClose(jptVec2.x, glmVec2.x, 0.05f));
    JPT_ENSURE(jpt::AreValuesClose(jptVec2.y, glmVec2.y, 0.05f));
    JPT_ENSURE(jpt::AreValuesClose(jptVec2.z, glmVec2.z, 0.05f));

    return true;
}

bool UnitTests_Matrix33_Translation()
{    
    Vec2 translation = Vec2();
    Matrix33 jptMat1 = Matrix33::Translation(Vec2(2.0f, 3.0f));

    JPT_ENSURE(jptMat1 * translation == Vec2(2.0f, 3.0f));

    return true;
}

bool UnitTests_Matrix33_Rotation()
{
    Vec2 rotation = Vec2(1.0f, 0.0f);
    Matrix33 rotationMatrix = Matrix33::Rotation(jpt::ToRadians(90.0f));

    JPT_ENSURE(rotationMatrix * rotation == Vec2(0.0f, 1.0f));

    return true;
}

bool UnitTests_Matrix33_Scaling()
{
    Vec2 scaling = Vec2(1.0f, 1.0f);
    Matrix33 scalingMatrix = Matrix33::Scaling(Vec2(2.0f, 3.0f));

    JPT_ENSURE(scalingMatrix * scaling == Vec2(2.0f, 3.0f));

    return true;
}

bool UnitTests_Matrix33_Transpose()
{
    Matrix33 jptMat = Matrix33::Identity();

    jptMat.Rotate(jpt::ToRadians(90.0f));
    JPT_ENSURE(jptMat[0] == Vec3(0.0f, 1.0f, 0.0f));
    JPT_ENSURE(jptMat[1] == Vec3(-1.0f, 0.0f, 0.0f));
    JPT_ENSURE(jptMat[2] == Vec3(0.0f, 0.0f, 1.0f));

    jptMat.Transpose();
    JPT_ENSURE(jptMat[0] == Vec3(0.0f, -1.0f, 0.0f));
    JPT_ENSURE(jptMat[1] == Vec3(1.0f, 0.0f, 0.0f));
    JPT_ENSURE(jptMat[2] == Vec3(0.0f, 0.0f, 1.0f));

    return true;
}

bool UnitTests_Matrix33_Inverse()
{
    Vec2 originV = Vec2(1.0f, 1.0f);
    Matrix33 jptMat = Matrix33::Identity();

    // Translation
    jptMat.Translate(Vec2(2.0f, 3.0f));

    // Rotation
    jptMat.Rotate(jpt::ToRadians(90.0f));

    // Scaling
    jptMat.Scale(2.0f);

    Vec2 v = jptMat * originV;
    
    jptMat.Invert();

    Vec2 invV = jptMat * v;

    JPT_ENSURE(invV == originV);

    return true;
}

export bool RunUnitTests_Matrix33()
{
    JPT_ENSURE(UnitTests_Matrix33_Mul());
    JPT_ENSURE(UnitTests_Matrix33_Translation());
    JPT_ENSURE(UnitTests_Matrix33_Rotation());
    JPT_ENSURE(UnitTests_Matrix33_Scaling());
    JPT_ENSURE(UnitTests_Matrix33_Transpose());
    JPT_ENSURE(UnitTests_Matrix33_Inverse());

    return true;
}

