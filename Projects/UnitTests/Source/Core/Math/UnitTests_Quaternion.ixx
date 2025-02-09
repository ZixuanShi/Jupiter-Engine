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
	return jpt::AreValuesClose(lhs.x, rhs.x, 0.05f) && 
		   jpt::AreValuesClose(lhs.y, rhs.y, 0.05f) && 
		   jpt::AreValuesClose(lhs.z, rhs.z, 0.05f) && 
		   jpt::AreValuesClose(lhs.w, rhs.w, 0.05f);
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
	jptQuat.Rotate(Vec3(0.0f, 1.0f, 0.0f), jpt::ToRadians(90.0f));
	glmQuat = glm::rotate(glmQuat, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	JPT_ENSURE(jptQuat == glmQuat);

	// Rotate around X axis
	jptQuat.Rotate(Vec3(1.0f, 0.0f, 0.0f), jpt::ToRadians(90.0f));
	glmQuat = glm::rotate(glmQuat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	JPT_ENSURE(jptQuat == glmQuat);

	// Rotate around Z axis
	jptQuat.Rotate(Vec3(0.0f, 0.0f, 1.0f), jpt::ToRadians(45.0f));
	glmQuat = glm::rotate(glmQuat, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	JPT_ENSURE(jptQuat == glmQuat);

	// EulerAngles
	Vec3 eulerAngles(jpt::ToRadians(90.0f), jpt::ToRadians(90.0f), jpt::ToRadians(45.0f));
	Quaternion jptQuat2 = Quaternion::Rotation(eulerAngles);
	JPT_ENSURE(jptQuat == jptQuat2);
	JPT_ENSURE(glmQuat == jptQuat2);

	return true;
}

export bool RunUnitTests_Quaternion()
{
	JPT_ENSURE(UnitTests_Quaternion_Lerp());
	JPT_ENSURE(UnitTests_Quaternion_SLerp());
	JPT_ENSURE(UnitTests_Quaternion_EulerAngles());

	return true;
}