// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include "Core/Types/Enum.h"

export module UnitTests_Enum;

JPT_ENUM(EFruits, 
	Apple = 5,
	Banana, 
	Orange = 9,
	Grape);

bool UnitTest_GlobalEnum()
{
	// Global data
	JPT_ENSURE(EFruits::Count() == 4, "");
	JPT_ENSURE(EFruits::Min() == EFruits::Apple, "");
	JPT_ENSURE(EFruits::Max() == EFruits::Grape, "");

	// Comparing
	EFruits fruit(EFruits::Banana);
	JPT_ENSURE(fruit == 6, "");
	JPT_ENSURE(fruit == "Banana", "");
	JPT_ENSURE(fruit == EFruits::Banana, "");
	JPT_ENSURE(fruit > EFruits::Apple, "");
	JPT_ENSURE(fruit < EFruits::Orange, "");
	JPT_ENSURE(fruit != EFruits::Grape, "");

	// Assigning
	fruit = EFruits::Orange;
	JPT_ENSURE(fruit == 9, "");
	JPT_ENSURE(fruit == "Orange", "");
	JPT_ENSURE(fruit == EFruits::Orange, "");

	// Iterating
	// for (const auto& pair : fruit)
	for (const auto& [key, value] : fruit) // Structure binding is available too
	{
		if (key == 5)
			JPT_ENSURE(value == "Apple", "");

		if (key == 6)
			JPT_ENSURE(value == "Banana", "");

		if (key == 9)
			JPT_ENSURE(value == "Orange", "");

		if (key == 10)
			JPT_ENSURE(value == "Grape", "");
	}

	return true;
}

bool UnitTest_LocalEnum()
{
	return true;
}

export bool RunUnitTests_Enum()
{
	JPT_ENSURE(UnitTest_GlobalEnum(), "UnitTest_GlobalEnum Failed");
	JPT_ENSURE(UnitTest_LocalEnum(), "UnitTest_LocalEnum Failed");

	return true;
}
