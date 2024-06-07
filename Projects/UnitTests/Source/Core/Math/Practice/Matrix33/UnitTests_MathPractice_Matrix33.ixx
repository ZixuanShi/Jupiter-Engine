// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTest_MathPractice_Matrix33;

import jpt.CoreModules;

export bool UnitTest_MathPractice_Matrix33()
{
	Matrix33f matrix33 = Matrix33f::Identity();
	Vec2f point = Vec2f(1.0f, 2.0f);

	JPT_ENSURE(Matrix33f::Translate(matrix33, point) == Matrix33f(1.0f, 0.0f, 1.0f,
		                                                          0.0f, 1.0f, 2.0f,
		                                                          0.0f, 0.0f, 1.0f));

	JPT_ENSURE(Matrix33f::Scale(matrix33, point) == Matrix33f(1.0f, 0.0f, 0.0f,
															  0.0f, 2.0f, 0.0f,
		                                                      0.0f, 0.0f, 1.0f));

	JPT_ENSURE(Matrix33f::RotateDegrees(matrix33, 90.0f) == Matrix33f(0.0f, -1.0f, 0.0f,
		                                                              1.0f, 0.0f, 0.0f,
		                                                              0.0f, 0.0f, 1.0f));

	return true;
}