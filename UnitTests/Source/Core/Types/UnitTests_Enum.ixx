// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include "Core/Types/Enum.h"

export module UnitTests_Enum;

import UnitTests_Enum_Global;

bool UnitTest_GlobalEnum()
{
	//EFruit_Global fruit(EFruit_Global::Banana);

	return true;
}

bool UnitTest_GlobalEnumContainer()
{
	GlobalEnumContainer::NestedEnum::s_data;

	return true;
}

JPT_ENUM(EFruit_Local,
	Apple = 5,
	Banana,
	Orange = 9,
	Grape);

bool UnitTest_LocalEnum()
{
	// Global data
	JPT_ENSURE(EFruit_Local::Count() == 4, "");
	JPT_ENSURE(EFruit_Local::Min() == EFruit_Local::Apple, "");
	JPT_ENSURE(EFruit_Local::Max() == EFruit_Local::Grape, "");

	// Comparing
	EFruit_Local fruit(EFruit_Local::Banana);
	JPT_ENSURE(fruit == 6, "");
	JPT_ENSURE(fruit == "Banana", "");
	JPT_ENSURE(fruit == EFruit_Local::Banana, "");
	JPT_ENSURE(fruit > EFruit_Local::Apple, "");
	JPT_ENSURE(fruit < EFruit_Local::Orange, "");
	JPT_ENSURE(fruit != EFruit_Local::Grape, "");

	// Assigning
	fruit = EFruit_Local::Orange;
	JPT_ENSURE(fruit == 9, "");
	JPT_ENSURE(fruit == "Orange", "");
	JPT_ENSURE(fruit == EFruit_Local::Orange, "");

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

	// Assigning with different sized integer
	// size_t
	size_t i = 5;
	fruit = i;
	JPT_ENSURE(fruit == 5, "");
	JPT_ENSURE(fruit == EFruit_Local::Apple, "");
	JPT_ENSURE(fruit == "Apple", "");

	// int64
	int64 j = 9;
	fruit = j;
	JPT_ENSURE(fruit == 9, "");
	JPT_ENSURE(fruit == EFruit_Local::Orange, "");
	JPT_ENSURE(fruit == "Orange", "");

	return true;
}

export bool RunUnitTests_Enum()
{
	JPT_ENSURE(UnitTest_GlobalEnum(), "UnitTest_GlobalEnum Failed");
	JPT_ENSURE(UnitTest_GlobalEnumContainer(), "UnitTest_GlobalEnumContainer Failed");
	JPT_ENSURE(UnitTest_LocalEnum(), "UnitTest_LocalEnum Failed");

	return true;
}
