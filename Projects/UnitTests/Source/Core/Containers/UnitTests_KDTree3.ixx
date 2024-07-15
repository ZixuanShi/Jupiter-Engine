// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_KDTree3;

import jpt.KDTree3;
import jpt.DynamicArray;
import jpt.Vector3;
import jpt.TypeDefs;
import jpt.Utilities;

jpt::DynamicArray<Vec3f> GetTestData()
{
	jpt::DynamicArray<Vec3f> points =
	{
		{ 0.00f, 0.00f, 0.00f },
		{ 0.00f, 0.01f, 0.00f },

		{ 2.00f, 2.00f, 2.00f },
		{ 2.01f, 2.00f, 2.00f },

		{ 3.00f, 3.00f, 3.01f },
		{ 3.00f, 3.00f, 3.00f },

		{ 4.00f, 4.00f, 4.00f },
		{ 4.00f, 4.00f, 4.01f },
		{ 4.00f, 4.00f, 4.00f },

		{ 15.0f, 10.0f,  9.0f },
		{ 20.0f,  0.0f, 19.0f },
		{ 15.0f, 90.0f, 22.0f },
		{  7.0f, 18.0f, 26.0f },
	};

	return points;
}

bool UnitTest_KDTree3()
{
	// Creating
    KDTree3f tree(GetTestData());
	JPT_ENSURE(tree.Count() == 13);

	// Has
	JPT_ENSURE(tree.Has({ 3.00f, 3.00f, 3.00f }));
	JPT_ENSURE(tree.Has({ 4.00f, 4.00f, 4.00f }));
	JPT_ENSURE(tree.Has({ 15.0f, 90.0f, 22.0f }));
	JPT_ENSURE(!tree.Has({ 99.0f, 99.0f, 99.0f }));

	// Adding
	tree.Add({ 3.00f, 3.00f, 3.00f });
	tree.Add({ 4.00f, 4.00f, 4.01f });
	tree.Add({ 15.0f, 90.0f, 22.01f });
	JPT_ENSURE(tree.Count() == 16);

	// Find Nearest
	jpt::DynamicArray<Vec3f> nearest = tree.FindNearest({ 3.00f, 3.00f, 3.00f }, 0.1f);
	JPT_ENSURE(nearest.Count() == 3);
	
	nearest = tree.FindNearest({ 4.00f, 4.00f, 4.00f }, 0.1f);
	JPT_ENSURE(nearest.Count() == 4);

	nearest = tree.FindNearest({ 15.0f, 90.0f, 22.0f }, 0.1f);
	JPT_ENSURE(nearest.Count() == 2);

	nearest = tree.FindNearest({ 7.0f, 18.0f, 26.0f }, 0.1f);
	JPT_ENSURE(nearest.Count() == 1);

	nearest = tree.FindNearest({ 99.0f, 99.0f, 99.0f }, 0.1f);
	JPT_ENSURE(nearest.Count() == 0);

	// Erasing
	tree.Erase({ 3.00f, 3.00f, 3.00f });
	tree.Erase({ 3.00f, 3.00f, 3.00f });
	tree.Erase({ 3.00f, 3.00f, 3.01f });
	nearest = tree.FindNearest({ 3.00f, 3.00f, 3.00f }, 0.1f);
	JPT_ENSURE(nearest.Count() == 0);

	tree.Erase({ 7.0f, 18.0f, 26.0f });
	nearest = tree.FindNearest({ 7.0f, 18.0f, 26.0f }, 0.1f);
	JPT_ENSURE(nearest.Count() == 0);

	return true;
}

export bool RunUnitTests_KDTree3()
{
    JPT_ENSURE(UnitTest_KDTree3());

    return true;
}
