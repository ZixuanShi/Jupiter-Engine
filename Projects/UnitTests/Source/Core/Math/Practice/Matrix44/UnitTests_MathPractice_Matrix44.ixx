// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTest_MathPractice_Matrix44;

import jpt.CoreModules;

export bool UnitTest_MathPractice_Matrix44()
{
	// Identity
	Matrix44f matrix44 = Matrix44f::Identity();
	JPT_ENSURE(matrix44.m[0] == Vec4f(1.0f, 0.0f, 0.0f, 0.0f));
	JPT_ENSURE(matrix44.m[1] == Vec4f(0.0f, 1.0f, 0.0f, 0.0f));
	JPT_ENSURE(matrix44.m[2] == Vec4f(0.0f, 0.0f, 1.0f, 0.0f));
	JPT_ENSURE(matrix44.m[3] == Vec4f(0.0f, 0.0f, 0.0f, 1.0f));

	return true;
}