// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Quaternion;

import jpt.Quaternion;
import jpt.Utilities;
import jpt.Math;

bool UnitTest_Quaternion_Lerp()
{
	Quatf quaternion = Quatf::Identity();
	Quatf quaternion2 = Quatf(0.707f, 0, 0, 0.707f);
	Quatf lerpResult = Quatf::Lerp(quaternion, quaternion2, 0.5f);
	JPT_ENSURE(lerpResult == Quatf(0.354f, 0, 0, 0.854f));

	return true;
}

bool UnitTest_Quaternion_SLerp()
{
	Quatf quaternion = Quatf::Identity();
	Quatf quaternion2 = Quatf(0.707f, 0, 0, 0.707f);
	Quatf slerpResult = Quatf::Slerp(quaternion, quaternion2, 0.5f);
	JPT_ENSURE(slerpResult == Quatf(0.383f, 0, 0, 0.924f));

	return true;
}

bool UnitTest_Quaternion_Rotate()
{
	// 1
	Quatf quaternion = Quatf::Identity();
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
	Quatf quaternion2 = Quatf::FromDegrees(90, 0, 45);
	JPT_ENSURE(quaternion2 == Quatf(0.653f, -0.271f, 0.271f, 0.653f));
	JPT_ENSURE(quaternion2 == quaternion);

	// 4
	Quatf quaternion3 = Quatf::FromAxisAngle(Vec3f(0, 1, 0), jpt::ToRadians(45.0f));
	JPT_ENSURE(quaternion3 == Quatf(0.000f, 0.383f, 0.000f, 0.924f));

	// 5
	Quatf quaternion4 = Quatf::FromAxisAngle(Vec3f(1.0f, 0.0f, 0.0f), jpt::ToRadians(90.0f));
	JPT_ENSURE(quaternion4 == Quatf(0.707f, 0, 0, 0.707f));

	return true;
}

bool UnitTest_Quaternion_Multiplication()
{
	Quatf quaternion = Quatf::Identity();
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

bool UnitTest_Quaternion_EulerAngles()
{
	Quatf quaternion = Quatf::FromDegrees(Vec3f(90, 45, 90));
	JPT_ENSURE(quaternion == Quatf(0.653f, -0.271f, 0.653f, 0.271f));

	return true;
}

export bool RunUnitTests_Quaternion()
{
	Quatf quaternion = Quatf::Identity();
	JPT_ENSURE(quaternion == Quatf(0, 0, 0, 1));

	JPT_ENSURE(UnitTest_Quaternion_Rotate());
	JPT_ENSURE(UnitTest_Quaternion_Lerp());
	JPT_ENSURE(UnitTest_Quaternion_SLerp());
	JPT_ENSURE(UnitTest_Quaternion_Multiplication());
	JPT_ENSURE(UnitTest_Quaternion_AxisAngles());
	JPT_ENSURE(UnitTest_Quaternion_EulerAngles());

	return true;
}