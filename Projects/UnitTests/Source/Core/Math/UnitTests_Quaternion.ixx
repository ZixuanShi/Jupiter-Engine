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
	//JPT_LOG(lerpResult);

	Quatf slerpResult = Quatf::Slerp(quaternion, quaternion2, 0.5f);
	JPT_ENSURE(slerpResult == Quatf(0.383f, 0, 0, 0.924f));
	//JPT_LOG(slerpResult);
	
	Quatf mulResult = slerpResult * quaternion2;
	JPT_ENSURE(mulResult == Quatf(0.924f, 0, 0, 0.383f));
	//JPT_LOG(mulResult);

	return true;
}