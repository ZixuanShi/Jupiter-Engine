// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTest_MathPractice_FindPerpendicularVector;

import jpt.CoreModules;

Vec3f FindPerpendicularVector(const Vec3f& v1, const Vec3f& v2)
{
	Vec3f result = v1.Cross(v2);
	return result;
}

export bool UnitTest_MathPractice_FindPerpendicularVector()
{
	JPT_ENSURE(FindPerpendicularVector(Vec3f(0.0f, 1.0f, 0.0f), Vec3f(1.0f, 0.0f, 2.0f)) == Vec3f(2.0f, 0.0f, -1.0f));

	return true;
}