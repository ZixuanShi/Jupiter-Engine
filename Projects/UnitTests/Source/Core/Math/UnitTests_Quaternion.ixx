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
	Quatf quaternion = Quatf::Identity;
	Vec3f rotationAxis = { 1,0,0 };
	quaternion.Rotate(rotationAxis, jpt::ToRadians(90.0f));
	JPT_ENSURE(quaternion == Quatf(0.707f, 0, 0, 0.707f));

	rotationAxis = { 0,0,1 };
	quaternion.Rotate(rotationAxis, jpt::ToRadians(45.0f));
	JPT_ENSURE(quaternion == Quatf(0.653f, 0.271f, 0.271f, 0.653f));

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

export bool RunUnitTests_Quaternion()
{
	Quatf quaternion = Quatf::Identity;
	JPT_ENSURE(quaternion == Quatf(0, 0, 0, 1));

	JPT_ENSURE(UnitTest_Quaternion_Rotate());
	JPT_ENSURE(UnitTest_Quaternion_Lerp());
	JPT_ENSURE(UnitTest_Quaternion_SLerp());
	JPT_ENSURE(UnitTest_Quaternion_Multiplication());

	return true;
}