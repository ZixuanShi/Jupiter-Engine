// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Quaternion;

import jpt.CoreModules;

export bool RunUnitTests_Quaternion()
{
	Quatf quaternion = Quatf::Identity;
	JPT_ENSURE(quaternion == Quatf(0, 0, 0, 1));

	Quatf quaternion2 = Quatf(0.707f, 0, 0, 0.707f);
	Quatf lerpResult = Quatf::Lerp(quaternion, quaternion2, 0.5f);
	JPT_ENSURE(lerpResult == Quatf(0.354f, 0, 0, 0.854f));

	Quatf slerpResult = Quatf::Slerp(quaternion, quaternion2, 0.5f);
	JPT_ENSURE(slerpResult == Quatf(0.383f, 0, 0, 0.924f));
	
	Quatf mulResult = slerpResult * quaternion2;
	JPT_ENSURE(mulResult == Quatf(0.924f, 0, 0, 0.383f));

	quaternion = Quatf::Identity;
	Vec3f rotationAxis = { 1,0,0 };
	quaternion.Rotate(rotationAxis, jpt::ToRadians(90.0f));
	JPT_ENSURE(quaternion == Quatf(0.707f, 0, 0, 0.707f));

	rotationAxis = { 0,0,1 };
	quaternion.Rotate(rotationAxis, jpt::ToRadians(45.0f));
	JPT_ENSURE(quaternion == Quatf(0.653f, 0.271f, 0.271f, 0.653f));

	return true;
}