// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Quaternion;

import jpt.Quaternion;
import jpt.Utilities;
import jpt.Math;
import jpt.Matrix44;

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
	return true;
}

export bool RunUnitTests_Quaternion()
{
	JPT_ENSURE(UnitTests_Quaternion_Lerp());
	JPT_ENSURE(UnitTests_Quaternion_SLerp());

	return true;
}