// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Math_IsLeftOfView;

import jpt.Utilities;
import jpt.Vector3;

static bool IsLeftOfView(Vec3f viewDir, Vec3f viewPos, Vec3f point)
{
	// Calculate the vector from the view position to the point
	const Vec3f viewToPoint = (point - viewPos).Normalized();

	// Calculate the cross product of the view direction and the view to point vector
	const Vec3f cross = viewDir.Cross(viewToPoint);

	// If the cross product is pointing up, the point is to the left of the view
	return cross.y > 0;
}

export bool UnitTest_Math_IsLeftOfView()
{
	Vec3f viewDir = Vec3f::Forward();
	Vec3f viewPos = Vec3f::Zero();
	Vec3f point;

	// Test a point to the left of the view
	point = Vec3f(-1, 0, 1);
	JPT_ENSURE(!IsLeftOfView(viewDir, viewPos, point));

	// Test a point to the right of the view
	point = Vec3f(1, 0, 1);
	JPT_ENSURE(IsLeftOfView(viewDir, viewPos, point));

	// Behind the view
	point = Vec3f(-1, 0, -1);
	JPT_ENSURE(!IsLeftOfView(viewDir, viewPos, point));
	point = Vec3f(1, 0, -1);
	JPT_ENSURE(IsLeftOfView(viewDir, viewPos, point));

	// Different view direction
	viewDir = Vec3f::Backward();

	// Test a point to the left of the view
	point = Vec3f(-1, 0, -1);
	JPT_ENSURE(IsLeftOfView(viewDir, viewPos, point));

	return true;
}