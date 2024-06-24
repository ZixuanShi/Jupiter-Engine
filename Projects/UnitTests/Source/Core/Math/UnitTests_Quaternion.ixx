// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Quaternion;

import jpt.CoreModules;

bool UnitTest_Quaternion_Lerp()
{
	Quatf quaternion = Quatf::Identity;
	Quatf quaternion2 = Quatf(0.707f, 0, 0, 0.707f);
	Quatf lerpResult = Quatf::Lerp(quaternion, quaternion2, 0.5f);
	JPT_ENSURE(lerpResult == Quatf(0.354f, 0, 0, 0.854f));

	return true;
}

bool UnitTest_Quaternion_SLerp()
{
	Quatf quaternion = Quatf::Identity;
	Quatf quaternion2 = Quatf(0.707f, 0, 0, 0.707f);
	Quatf slerpResult = Quatf::Slerp(quaternion, quaternion2, 0.5f);
	JPT_ENSURE(slerpResult == Quatf(0.383f, 0, 0, 0.924f));

	return true;
}

bool UnitTest_Quaternion_Rotate()
{
	// 1
	Quatf quaternion = Quatf::Identity;
	quaternion.RotateX(jpt::ToRadians(90.0f));
	JPT_ENSURE(quaternion == Quatf(0.707f, 0, 0, 0.707f));
	quaternion.RotateX(jpt::ToRadians(90.0f));
	JPT_ENSURE(quaternion == Quatf(1, 0, 0, 0));
	quaternion.RotateX(jpt::ToRadians(-90.0f));
	JPT_ENSURE(quaternion == Quatf(0.707f, 0, 0, 0.707f));

	// 2
	quaternion.RotateZ(jpt::ToRadians(45.0f));
	JPT_ENSURE(quaternion == Quatf(0.653f, -0.271f, 0.271f, 0.653f));

	// 3
	quaternion = Quatf::Identity;
	quaternion.RotateX(jpt::ToRadians(45.0f));
	quaternion.RotateY(jpt::ToRadians(-29.0f));
	quaternion.RotateZ(jpt::ToRadians(90.0f));
	JPT_ENSURE(quaternion == Quatf(0.098f, -0.426f, 0.565f, 0.7f));

	// 4
	quaternion = Quatf::Identity;
	quaternion.RotateX(jpt::ToRadians(23.0f));
	quaternion.RotateY(jpt::ToRadians(11.0f));
	quaternion.RotateZ(jpt::ToRadians(50.0f));
	JPT_ENSURE(quaternion == Quatf(0.220f, 0.001f, 0.430f, 0.876f));

	// 5
	quaternion = Quatf::Identity;

	Quatf rotationX = Quatf::FromAxisAngle(Vec3f::Right(), jpt::ToRadians(-143.0f));
	Quatf rotationY = Quatf::FromAxisAngle(Vec3f::Up(), jpt::ToRadians(55.0f));
	Quatf rotationZ = Quatf::FromAxisAngle(Vec3f::Forward(), jpt::ToRadians(-41.0f));
	quaternion = rotationX * rotationY * rotationZ;

	JPT_ENSURE(quaternion == Quatf(-0.839f, -0.157f, -0.509f, 0.110f));

	return true;
}

bool UnitTest_Quaternion_Multiplication()
{
	Quatf quaternion = Quatf::Identity;
	Quatf quaternion2 = Quatf(0.707f, 0, 0, 0.707f);
	Quatf mulResult = quaternion * quaternion2;
	JPT_ENSURE(mulResult == Quatf(0.707f, 0, 0, 0.707f));

	quaternion.RotateY(jpt::ToRadians(30.0f));
	mulResult = quaternion * quaternion2;
	JPT_ENSURE(mulResult == Quatf(0.683f, 0.183f, -0.183f, 0.683f));

	return true;
}

bool UnitTest_Quaternion_AxisAngles()
{
	Quatf quaternion = Quatf::FromAxisAngle(Vec3f(0, 1, 0), jpt::ToRadians(45.0f));
	JPT_ENSURE(quaternion == Quatf(0.0f,0.383f, 0.0f, 0.924f));

	return true;
}

export bool RunUnitTests_Quaternion()
{
	Quatf quaternion = Quatf::Identity;
	JPT_ENSURE(quaternion == Quatf(0, 0, 0, 1));

	JPT_ENSURE(UnitTest_Quaternion_Rotate());
	JPT_ENSURE(UnitTest_Quaternion_Lerp());
	JPT_ENSURE(UnitTest_Quaternion_SLerp());
	JPT_ENSURE(UnitTest_Quaternion_Multiplication());
	JPT_ENSURE(UnitTest_Quaternion_AxisAngles());

	return true;
}