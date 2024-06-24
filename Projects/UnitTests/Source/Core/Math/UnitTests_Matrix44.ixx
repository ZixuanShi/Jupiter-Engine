// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Matrix44;

import jpt.CoreModules;

// https://www.andre-gaschler.com/rotationconverter/

bool UnitTests_Matrix44_Translation()
{
	Matrix44f translation = Matrix44f::Translation(Vec3f(1.0f, 2.0f, 3.0f));
	JPT_ENSURE(translation.m[0] == Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
	JPT_ENSURE(translation.m[1] == Vec4f(0.0f, 1.0f, 0.0f, 2.0f));
	JPT_ENSURE(translation.m[2] == Vec4f(0.0f, 0.0f, 1.0f, 3.0f));
	JPT_ENSURE(translation.m[3] == Vec4f(0.0f, 0.0f, 0.0f, 1.0f));

	Vec3f translationVector = Vec3f(4.0f, 5.0f, 6.0f);
	Vec3f translationResult = translation * translationVector;
	JPT_ENSURE(translationResult == Vec3f(5.0f, 7.0f, 9.0f));

	return true;
}

bool UnitTests_Matrix44_Rotation()
{
	// 1
	Matrix44f rotation = Matrix44f::RotationX(jpt::ToRadians(90.0f));
	JPT_ENSURE(rotation.m[0] == Vec4f(1.0f, 0.0f,  0.0f, 0.0f));
	JPT_ENSURE(rotation.m[1] == Vec4f(0.0f, 0.0f, -1.0f, 0.0f));
	JPT_ENSURE(rotation.m[2] == Vec4f(0.0f, 1.0f,  0.0f, 0.0f));
	JPT_ENSURE(rotation.m[3] == Vec4f(0.0f, 0.0f,  0.0f, 1.0f));

	rotation.RotateX(jpt::ToRadians(90.0f));
	JPT_ENSURE(rotation.m[0] == Vec4f(1.0f,  0.0f,  0.0f, 0.0f));
	JPT_ENSURE(rotation.m[1] == Vec4f(0.0f, -1.0f,  0.0f, 0.0f));
	JPT_ENSURE(rotation.m[2] == Vec4f(0.0f,  0.0f, -1.0f, 0.0f));
	JPT_ENSURE(rotation.m[3] == Vec4f(0.0f,  0.0f,  0.0f, 1.0f));

	Vec3f rotationVector = Vec3f(4.0f, 5.0f, 6.0f);
	Vec3f rotationResult = rotation * rotationVector;
	JPT_ENSURE(rotationResult == Vec3f(4.0f, -5.0f, -6.0f));

	// 2
	rotation = Matrix44f::Identity;
	rotation.RotateX(jpt::ToRadians(45.0f));
	rotation.RotateY(jpt::ToRadians(-29.0f));
	rotation.RotateZ(jpt::ToRadians(90.0f));
	JPT_ENSURE(rotation.m[0] == Vec4f(0.000f, -0.875f, -0.485f, 0.000f));
	JPT_ENSURE(rotation.m[1] == Vec4f(0.707f,  0.343f, -0.618f, 0.000f));
	JPT_ENSURE(rotation.m[2] == Vec4f(0.707f, -0.343f,  0.618f, 0.000f));
	JPT_ENSURE(rotation.m[3] == Vec4f(0.000f,  0.000f,  0.000f, 1.000f));

	// 3
	rotation = Matrix44f::Identity;
	rotation.RotateX(jpt::ToRadians(23.0f));
	rotation.RotateY(jpt::ToRadians(11.0f));
	rotation.RotateZ(jpt::ToRadians(50.0f));
	JPT_ENSURE(rotation.m[0] == Vec4f(0.631f, -0.752f,  0.191f, 0.000f));
	JPT_ENSURE(rotation.m[1] == Vec4f(0.753f,  0.535f, -0.384f, 0.000f));
	JPT_ENSURE(rotation.m[2] == Vec4f(0.186f,  0.386f,  0.904f, 0.000f));
	JPT_ENSURE(rotation.m[3] == Vec4f(0.000f,  0.000f,  0.000f, 1.000f));

	return true;
}

bool UnitTests_Matrix44_Scaling()
{
	Matrix44f scaling = Matrix44f::Scaling(Vec3f(3, 2, 1));
	JPT_ENSURE(scaling.m[0] == Vec4f(3.0f, 0.0f, 0.0f, 0.0f));
	JPT_ENSURE(scaling.m[1] == Vec4f(0.0f, 2.0f, 0.0f, 0.0f));
	JPT_ENSURE(scaling.m[2] == Vec4f(0.0f, 0.0f, 1.0f, 0.0f));
	JPT_ENSURE(scaling.m[3] == Vec4f(0.0f, 0.0f, 0.0f, 1.0f));

	Vec3f scalingVector = Vec3f(4.0f, 5.0f, 6.0f);
	Vec3f scalingResult = scaling * scalingVector;
	JPT_ENSURE(scalingResult == Vec3f(12.0f, 10.0f, 6.0f));

	return true;
}

bool UnitTests_Matrix44_EulerAngles()
{
	Matrix44f rotation = Matrix44f::FromDegrees(180, -63, 22);
	JPT_ENSURE(rotation.m[0] == Vec4f( 0.421f, -0.170f, -0.891f, 0.000f));
	JPT_ENSURE(rotation.m[1] == Vec4f(-0.375f, -0.927f,  0.000f, 0.000f));
	JPT_ENSURE(rotation.m[2] == Vec4f(-0.826f,  0.334f, -0.454f, 0.000f));
	JPT_ENSURE(rotation.m[3] == Vec4f( 0.000f,  0.000f,  0.000f, 1.000f));

	rotation = Matrix44f::FromDegrees(90, 45, 90);
	JPT_ENSURE(rotation.m[0] == Vec4f(-0.000f, -0.707f,  0.707f, 0.000f));
	JPT_ENSURE(rotation.m[1] == Vec4f(-0.000f, -0.707f, -0.707f, 0.000f));
	JPT_ENSURE(rotation.m[2] == Vec4f( 1.000f, -0.000f, -0.000f, 0.000f));
	JPT_ENSURE(rotation.m[3] == Vec4f( 0.000f,  0.000f,  0.000f, 1.000f));

	return true;
}

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

	JPT_ENSURE(UnitTests_Matrix44_Translation());
	JPT_ENSURE(UnitTests_Matrix44_Rotation());
	JPT_ENSURE(UnitTests_Matrix44_Scaling());
	JPT_ENSURE(UnitTests_Matrix44_EulerAngles());

	return true;
}