// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module UnitTests_Line3;

import jpt.Line3;
import jpt.Vector3;
import jpt.Math;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTests_Line3_Project()
{
	Vec3f point;
	Line3f line;
	Vec3f projected;
	
	point = Vec3f(0.0f, 10.0f, 0.0f);
	line = Line3f(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f));
	projected = line.Project(point);
	JPT_ENSURE(projected == Vec3f(0.0f));

	point = Vec3f(0.0f, 10.0f, 10.0f);
	line = Line3f(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f));
	projected = line.Project(point);
	JPT_ENSURE(projected == Vec3f(0.0f));

	point = Vec3f(10.0f, 10.0f, 10.0f);
	line = Line3f(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f));
	projected = line.Project(point);
	JPT_ENSURE(projected == Vec3f(0.0f));

	point = Vec3f(10.0f, 10.0f, 10.0f);
	line = Line3f(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(10.0f, 10.0f, 10.0f));
	projected = line.Project(point);
	JPT_ENSURE(projected == Vec3f(10.0f, 10.0f, 10.0f));

	point = Vec3f(10.0f, 10.0f, 10.0f);
	line = Line3f(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(5.0f, 5.0f, 5.0f));
	projected = line.Project(point);
	JPT_ENSURE(projected == Vec3f(10.0f, 10.0f, 10.0f));

	return true;
}

bool UnitTests_Line3_DistanceToPoint()
{
	Vec3f point;
	Line3f line;

	point = Vec3f(0.0f, 10.0f, 0.0f);
	line = Line3f(Vec3f(-10.0f, 0.0f, 0.0f), Vec3f(10.0f, 0.0f, 0.0f));
	JPT_ENSURE(line.Distance(point) == 10.0f);

	point = Vec3f(0.0f, 10.0f, 10.0f);
	line = Line3f(Vec3f(-10.0f, 0.0f, 0.0f), Vec3f(10.0f, 0.0f, 0.0f));
	JPT_ENSURE(jpt::AreValuesClose(line.Distance(point), 14.1421356f));

	point = Vec3f(0.0f, 10.0f, 10.0f);
	line = Line3f(Vec3f(-10.0f, 0.0f, 10.0f), Vec3f(10.0f, 0.0f, 10.0f));
	JPT_ENSURE(line.Distance(point) == 10.0f);

	point = Vec3f(0.0f, 20.0f, 0.0f);
	line = Line3f(Vec3f(-10.0f, 0.0f, 0.0f), Vec3f(10.0f, 0.0f, 0.0f));
	JPT_ENSURE(line.Distance(point) == 20.0f);

	point = Vec3f(0.0f, 20.0f, 0.0f);
	line = Line3f(Vec3f(-10.0f, 10.0f, 0.0f), Vec3f(10.0f, 10.0f, 0.0f));
	JPT_ENSURE(line.Distance(point) == 10.0f);

	point = Vec3f(-20.0f, 20.0f, 0.0f);
	line = Line3f(Vec3f(-10.0f, 10.0f, 0.0f), Vec3f(10.0f, 10.0f, 0.0f));
	JPT_ENSURE(line.Distance(point) == 10.0f);

	return true;
}

export bool RunUnitTests_Line3()
{
	JPT_ENSURE(UnitTests_Line3_Project());
	JPT_ENSURE(UnitTests_Line3_DistanceToPoint());

	return true;
}
