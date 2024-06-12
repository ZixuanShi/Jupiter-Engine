// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Matrix44;

import jpt.CoreModules;

export bool RunUnitTests_Matrix44()
{
	// Identity
	Matrix44f matrix44 = Matrix44f::Identity;
	JPT_ENSURE(matrix44.m[0] == Vec4f(1.0f, 0.0f, 0.0f, 0.0f));
	JPT_ENSURE(matrix44.m[1] == Vec4f(0.0f, 1.0f, 0.0f, 0.0f));
	JPT_ENSURE(matrix44.m[2] == Vec4f(0.0f, 0.0f, 1.0f, 0.0f));
	JPT_ENSURE(matrix44.m[3] == Vec4f(0.0f, 0.0f, 0.0f, 1.0f));

	// Transpose
	matrix44 = Matrix44f::Identity;
	matrix44.Transpose();
	JPT_ENSURE(matrix44.m[0] == Vec4f(1.0f, 0.0f, 0.0f, 0.0f));
	JPT_ENSURE(matrix44.m[1] == Vec4f(0.0f, 1.0f, 0.0f, 0.0f));
	JPT_ENSURE(matrix44.m[2] == Vec4f(0.0f, 0.0f, 1.0f, 0.0f));
	JPT_ENSURE(matrix44.m[3] == Vec4f(0.0f, 0.0f, 0.0f, 1.0f));

	matrix44 = Matrix44f{ Vec4f(1.0f, 2.0f, 3.0f, 4.0f), 
		                  Vec4f(5.0f, 6.0f, 7.0f, 8.0f), 
		                  Vec4f(9.0f, 10.0f, 11.0f, 12.0f), 
		                  Vec4f(13.0f, 14.0f, 15.0f, 16.0f) };
	//JPT_LOG(matrix44);
	matrix44.Transpose();
	JPT_ENSURE(matrix44.m[0] == Vec4f(1.0f, 5.0f, 9.0f, 13.0f));
	JPT_ENSURE(matrix44.m[1] == Vec4f(2.0f, 6.0f, 10.0f, 14.0f));
	JPT_ENSURE(matrix44.m[2] == Vec4f(3.0f, 7.0f, 11.0f, 15.0f));
	JPT_ENSURE(matrix44.m[3] == Vec4f(4.0f, 8.0f, 12.0f, 16.0f));

	// Translation
	Matrix44f translation = Matrix44f::Translation(Vec3f(1.0f, 2.0f, 3.0f));
	Vec3f translationVector = Vec3f(4.0f, 5.0f, 6.0f);
	Vec3f translationResult = translation * translationVector;
	JPT_ENSURE(translationResult == Vec3f(5.0f, 7.0f, 9.0f));

	matrix44 = Matrix44f(Vec4f(1.0f, 2.0f, 3.0f, 4.0f),
		                 Vec4f(5.0f, 6.0f, 7.0f, 8.0f),
		                 Vec4f(9.0f, 10.0f, 11.0f, 12.0f),
		                 Vec4f(13.0f, 14.0f, 15.0f, 16.0f));
	Matrix44f matrix44Translation = matrix44 * translation;
	JPT_ENSURE(matrix44Translation.m[0] == Vec4f(1.0f, 2.0f, 3.0f, 18.0f));
	JPT_ENSURE(matrix44Translation.m[1] == Vec4f(5.0f, 6.0f, 7.0f, 46.0f));
	JPT_ENSURE(matrix44Translation.m[2] == Vec4f(9.0f, 10.0f, 11.0f, 74.0f));
	JPT_ENSURE(matrix44Translation.m[3] == Vec4f(13.0f, 14.0f, 15.0f, 102.0f));

	return true;
}