// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Matrix33;

import jpt.Matrix33;
import jpt.Vector3;
import jpt.Vector2;
import jpt.Utilities;
import jpt.Math;

static bool UnitTests_Matrix33_Mul()
{	
	Matrix33f matrix33 = Matrix33f::Identity();
	Matrix33f matrixMul = Matrix33f(1.0f, 2.0f, 3.0f,
		                                  4.0f, 5.0f, 6.0f, 
		                                  7.0f, 8.0f, 9.0f);
	Matrix33f matrixMulResult = matrix33 * matrixMul;
	JPT_ENSURE(matrixMulResult.m[0] == Vec3f(1.0f, 2.0f, 3.0f));
	JPT_ENSURE(matrixMulResult.m[1] == Vec3f(4.0f, 5.0f, 6.0f));
	JPT_ENSURE(matrixMulResult.m[2] == Vec3f(7.0f, 8.0f, 9.0f));

	matrix33 = Matrix33f(2.0f, 0.0f, 0.0f,
		                 0.0f, 2.0f, 0.0f,
		                 0.0f, 0.0f, 3.0f);
	matrixMulResult = matrix33 * matrixMul;
	JPT_ENSURE(matrixMulResult.m[0] == Vec3f(2.0f, 4.0f, 6.0f));
	JPT_ENSURE(matrixMulResult.m[1] == Vec3f(8.0f, 10.0f, 12.0f));
	JPT_ENSURE(matrixMulResult.m[2] == Vec3f(21.0f, 24.0f, 27.0f));

	matrix33 = Matrix33f(1.0f, -5.0f, 3.0f,
		                 0.0f, -2.0f, 6.0f,
		                 7.0f, 2.0f, -4.0f);
	matrixMul = Matrix33f(-8.0f, 6.0f, 1.0f,
		                 7.0f, 0.0f, -3.0f,
		                 2.0f, 4.0f, 5.0f);
	matrixMulResult = matrix33 * matrixMul;
	JPT_ENSURE(matrixMulResult.m[0] == Vec3f(-37.0f, 18.0f, 31.0f));
	JPT_ENSURE(matrixMulResult.m[1] == Vec3f(-2.0f, 24.0f, 36.0f));
	JPT_ENSURE(matrixMulResult.m[2] == Vec3f(-50.0f, 26.0f, -19.0f));

	return true;
}

static bool UnitTests_Matrix33_Translation()
{	
	Matrix33f matrixTranslation = Matrix33f::Translation(Vec2f(1.0f, 2.0f));
	Vec2f vec2 = Vec2f(3.0f, 4.0f);
	Vec2f vec2Result = matrixTranslation * vec2;
	JPT_ENSURE(vec2Result == Vec2f(4.0f, 6.0f));

	matrixTranslation= Matrix33f(1, 2, 3,
		                         4, 5, 6,
		                         7, 8, 9);
	Matrix33f matrixTranslation2 = Matrix33f::Translation(Vec2f(5.0f, 6.0f));
	Matrix33f matrixTranslationResult = matrixTranslation * matrixTranslation2;
	JPT_ENSURE(matrixTranslationResult.m[0] == Vec3f(1.0f, 2.0f, 20.0f));
	JPT_ENSURE(matrixTranslationResult.m[1] == Vec3f(4.0f, 5.0f, 56.0f));
	JPT_ENSURE(matrixTranslationResult.m[2] == Vec3f(7.0f, 8.0f, 92.0f));

	matrixTranslation = Matrix33f::Identity();
	matrixTranslation.Translate(Vec2f(1.0f, 2.0f));
	vec2 = Vec2f(3.0f, 4.0f);
	vec2Result = matrixTranslation * vec2;
	JPT_ENSURE(vec2Result == Vec2f(4.0f, 6.0f));

	return true;
}

static bool UnitTests_Matrix33_Rotation()
{
	Matrix33f matrixRotation = Matrix33f::RotationDegrees(45.0f);
	Vec2f vec2 = Vec2f(1.0f, 0.0f);
	Vec2f vec2Result = matrixRotation * vec2;
	JPT_ENSURE(vec2Result == Vec2f(0.707106769f, 0.707106769f));

	matrixRotation = Matrix33f::Identity();
	matrixRotation.Rotate(jpt::ToRadians(90.0f));
	vec2 = Vec2f(10.0f, 10.0f);
	vec2Result = matrixRotation * vec2;
	JPT_ENSURE(vec2Result == Vec2f(-10.0f, 10.0f));

	matrixRotation.Rotate(jpt::ToRadians(-45.0f));
	vec2 = Vec2f(10.0f, 10.0f);
	vec2Result = matrixRotation * vec2;
	JPT_ENSURE(vec2Result == Vec2f(0.0f, 14.1421366f));

	return true;
}

static bool UnitTests_Matrix33_Scaling()
{
	Matrix33f matrixScale = Matrix33f::Scaling(5.0f);
	Vec2f vec2 = Vec2f(1.0f, 1.0f);
	Vec2f vec2Result = matrixScale * vec2;
	JPT_ENSURE(vec2Result == Vec2f(5.0f, 5.0f));

	matrixScale = Matrix33f::Identity();
	matrixScale.Scale(Vec2f(2.0f, 3.0f));
	vec2 = Vec2f(1.0f, 1.0f);
	vec2Result = matrixScale * vec2;
	JPT_ENSURE(vec2Result == Vec2f(2.0f, 3.0f));

	matrixScale.Scale(2.0f);
	vec2 = Vec2f(1.0f, 1.0f);
	vec2Result = matrixScale * vec2;
	JPT_ENSURE(vec2Result == Vec2f(4.0f, 6.0f));

	return true;
}

export bool RunUnitTests_Matrix33()
{
	// Matrix identity
	Matrix33f matrix33 = Matrix33f::Identity();
	JPT_ENSURE(matrix33.m[0] == Vec3f(1.0f, 0.0f, 0.0f));
	JPT_ENSURE(matrix33.m[1] == Vec3f(0.0f, 1.0f, 0.0f));
	JPT_ENSURE(matrix33.m[2] == Vec3f(0.0f, 0.0f, 1.0f));

	UnitTests_Matrix33_Mul();
	UnitTests_Matrix33_Translation();
	UnitTests_Matrix33_Rotation();
	UnitTests_Matrix33_Scaling();

	// Log
	//JPT_LOG(matrix33);

	return true;
}

