
// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module UnitTests_Matrix44;

import jpt.Matrix44;
import jpt.Vector3;
import jpt.Vector4;
import jpt.Math;
import jpt.Utilities;
import jpt.String;

// https://www.andre-gaschler.com/rotationconverter/

void LogGlmMatrix44(const glm::mat4& m)
{
	JPT_LOG(jpt::String::Format<256>("\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f", m[0][0], m[0][1], m[0][2], m[0][3],
		m[1][0], m[1][1], m[1][2], m[1][3],
		m[2][0], m[2][1], m[2][2], m[2][3],
		m[3][0], m[3][1], m[3][2], m[3][3]));
}

bool UnitTests_Matrix44_GLM()
{
	// Identity
	glm::mat4 glmMat4 = glm::mat4(1.0f);
	//LogGlmMatrix44(glmMat4);

	Matrix44 matrix44 = Matrix44::Identity();
	//JPT_LOG(matrix44);

	// Translation
	glmMat4 = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
	//LogGlmMatrix44(glmMat4);

	matrix44.Translate(1.0f, 2.0f, 3.0f);
	//JPT_LOG(matrix44);

	// Rotation
	glmMat4 = glm::rotate(glmMat4, glm::radians(120.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glmMat4 = glm::rotate(glmMat4, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glmMat4 = glm::rotate(glmMat4, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//LogGlmMatrix44(glmMat4);

	matrix44.Rotate(jpt::ToRadians(120.0f), jpt::ToRadians(-45.0f), jpt::ToRadians(90.0f));
	//JPT_LOG(matrix44);

	// Scaling
	glmMat4 = glm::scale(glmMat4, glm::vec3(2.0f, 3.0f, 4.0f));
	//LogGlmMatrix44(glmMat4);

	matrix44.Scale(2.0f, 3.0f, 4.0f);
	//JPT_LOG(matrix44);

	return true;
}

bool UnitTests_Matrix44_Translation()
{
	return true;
}

bool UnitTests_Matrix44_Rotation()
{
	return true;
}

bool UnitTests_Matrix44_Scaling()
{
	return true;
}

bool UnitTests_Matrix44_Transpose()
{
	return true;
}

export bool RunUnitTests_Matrix44()
{
	JPT_ENSURE(UnitTests_Matrix44_GLM());

	JPT_ENSURE(UnitTests_Matrix44_Translation());
	JPT_ENSURE(UnitTests_Matrix44_Rotation());
	JPT_ENSURE(UnitTests_Matrix44_Scaling());
	JPT_ENSURE(UnitTests_Matrix44_Transpose());

	return true;
}