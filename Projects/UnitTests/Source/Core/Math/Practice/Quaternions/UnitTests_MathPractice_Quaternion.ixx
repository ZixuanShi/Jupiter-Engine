// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTest_MathPractice_Quaternion;

import jpt.CoreModules;

export bool UnitTest_MathPractice_Quaternion()
{
	Quaternionf quaternion = Quaternionf::Identity();
	JPT_ENSURE(quaternion == Quaternionf(0, 0, 0, 1));


	return true;
}