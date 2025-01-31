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

bool UnitTests_Quaternion_Rotate()
{
	// 1
	Quaternion quaternion = Quaternion::Identity();
	quaternion.RotateX(jpt::ToRadians(90.0f));
	JPT_ENSURE(quaternion == Quaternion(0.707f, 0, 0, 0.707f));
	quaternion.RotateX(jpt::ToRadians(90.0f));
	JPT_ENSURE(quaternion == Quaternion(1, 0, 0, 0));
	quaternion.RotateX(jpt::ToRadians(-90.0f));
	JPT_ENSURE(quaternion == Quaternion(0.707f, 0, 0, 0.707f));

	// 2
	quaternion.RotateZ(jpt::ToRadians(45.0f));
	JPT_ENSURE(quaternion == Quaternion(0.653f, -0.271f, 0.271f, 0.653f));

	// 3
	Quaternion quaternion2 = Quaternion::FromDegrees(90, 0, 45);
	JPT_ENSURE(quaternion2 == Quaternion(0.653f, -0.271f, 0.271f, 0.653f));
	JPT_ENSURE(quaternion2 == quaternion);

	// 4
	Quaternion quaternion3 = Quaternion::FromAxisAngle(Vec3f(0, 1, 0), jpt::ToRadians(45.0f));
	JPT_ENSURE(quaternion3 == Quaternion(0.000f, 0.383f, 0.000f, 0.924f));

	// 5
	Quaternion quaternion4 = Quaternion::FromAxisAngle(Vec3f(1.0f, 0.0f, 0.0f), jpt::ToRadians(90.0f));
	JPT_ENSURE(quaternion4 == Quaternion(0.707f, 0, 0, 0.707f));

	//JPT_LOG(quaternion);

	return true;
}

bool UnitTests_Quaternion_Multiplication()
{
	Quaternion quaternion = Quaternion::Identity();
	Quaternion quaternion2 = Quaternion(0.707f, 0, 0, 0.707f);
	Quaternion mulResult = quaternion * quaternion2;
	JPT_ENSURE(mulResult == Quaternion(0.707f, 0, 0, 0.707f));

	quaternion.RotateY(jpt::ToRadians(30.0f));
	mulResult = quaternion * quaternion2;
	JPT_ENSURE(mulResult == Quaternion(0.683f, 0.183f, -0.183f, 0.683f));

	return true;
}

bool UnitTests_Quaternion_AxisAngles()
{
	Quaternion quaternion = Quaternion::FromAxisAngle(Vec3f(0, 1, 0), jpt::ToRadians(45.0f));
	JPT_ENSURE(quaternion == Quaternion(0.0f,0.383f, 0.0f, 0.924f));

	quaternion = Quaternion::FromAxisAngle(Vec3f(1, 0, 0), jpt::ToRadians(90.0f));
	JPT_ENSURE(quaternion == Quaternion(0.707f, 0, 0, 0.707f));

	return true;
}

bool UnitTests_Quaternion_EulerAngles()
{
	{
		Vec3f degrees = Vec3f(89, 45, 90);
		Quaternion quaternion = Quaternion::FromDegrees(degrees);
		JPT_ENSURE(quaternion == Quaternion(0.651f, -0.265f, 0.276f, 0.656f));
		JPT_ENSURE(quaternion.ToEulerDegrees() == degrees);
	}
	{
		Vec3f degrees = Vec3f(12, 136, 25);
		Quaternion quaternion = Quaternion::FromDegrees(degrees);
		JPT_ENSURE(quaternion == Quaternion(0.238f, 0.892f, -0.014f, 0.385f));
		JPT_ENSURE(quaternion.ToEulerDegrees() == degrees);
	}
	return true;
}

bool UnitTests_Quaternion_ToMatrix()
{
	Matrix44 matrix = Matrix44::FromDegrees(180, -60, 22);
	Quaternion quaternion = Quaternion::FromDegrees(180, -60, 22);
	JPT_ENSURE(matrix == quaternion.ToMatrix());

	matrix = Matrix44::FromDegrees(90, 45, 90);
	quaternion = Quaternion::FromDegrees(90, 45, 90);
	JPT_ENSURE(matrix == quaternion.ToMatrix());

	return true;
}

export bool RunUnitTests_Quaternion()
{
	Quaternion quaternion = Quaternion::Identity();
	JPT_ENSURE(quaternion == Quaternion(0, 0, 0, 1));

	JPT_ENSURE(UnitTests_Quaternion_Rotate());
	JPT_ENSURE(UnitTests_Quaternion_Lerp());
	JPT_ENSURE(UnitTests_Quaternion_SLerp());
	JPT_ENSURE(UnitTests_Quaternion_Multiplication());
	JPT_ENSURE(UnitTests_Quaternion_AxisAngles());
	JPT_ENSURE(UnitTests_Quaternion_EulerAngles());
	JPT_ENSURE(UnitTests_Quaternion_ToMatrix());

	return true;
}