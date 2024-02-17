// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include "Core/Types/Enum.h"

export module UnitTests_Enum;

import UnitTests_Enum_Global;

template<typename EType>
bool UnitTest_Enum()
{
	// Static data
	JPT_ENSURE(EType::Count() == 4, "");
	JPT_ENSURE(EType::Min() == EType::Apple, "");
	JPT_ENSURE(EType::Max() == EType::Grape, "");

	// Comparing
	EType fruit(EType::Banana);
	JPT_ENSURE(fruit == 6, "");
	JPT_ENSURE(fruit == "Banana", "");
	JPT_ENSURE(fruit == EType::Banana, "");
	JPT_ENSURE(fruit > EType::Apple, "");
	JPT_ENSURE(fruit < EType::Orange, "");
	JPT_ENSURE(fruit != EType::Grape, "");

	// Math operations
	--fruit;
	JPT_ENSURE(fruit == 5, "");
	JPT_ENSURE(fruit == "Apple", "");
	JPT_ENSURE(fruit == EType::Apple, "");

	++fruit;
	JPT_ENSURE(fruit == 6, "");
	JPT_ENSURE(fruit == "Banana", "");
	JPT_ENSURE(fruit == EType::Banana, "");

	fruit += 3;
	JPT_ENSURE(fruit == 9, "");
	JPT_ENSURE(fruit == "Orange", "");
	JPT_ENSURE(fruit == EType::Orange, "");

	fruit -= 3;
	JPT_ENSURE(fruit == 6, "");
	JPT_ENSURE(fruit == "Banana", "");
	JPT_ENSURE(fruit == EType::Banana, "");

	JPT_ENSURE(fruit + 3 == 9, "");
	JPT_ENSURE(fruit + 3 == "Orange", "");
	JPT_ENSURE(fruit + 3 == EType::Orange, "");

	JPT_ENSURE(fruit - 1 == 5, "");
	JPT_ENSURE(fruit - 1 == "Apple", "");
	JPT_ENSURE(fruit - 1 == EType::Apple, "");

	// Assigning
	fruit = EType::Orange;
	JPT_ENSURE(fruit == 9, "");
	JPT_ENSURE(fruit == "Orange", "");
	JPT_ENSURE(fruit == EType::Orange, "");

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
	JPT_ENSURE(fruit == EType::Apple, "");
	JPT_ENSURE(fruit == "Apple", "");

	// int64
	int64 j = 9;
	fruit = j;
	JPT_ENSURE(fruit == 9, "");
	JPT_ENSURE(fruit == EType::Orange, "");
	JPT_ENSURE(fruit == "Orange", "");

	return true;
}

JPT_ENUM(EFruit_Local,
	Apple = 5,
	Banana,
	Orange = 9,
	Grape);

export bool RunUnitTests_Enum()
{
	JPT_ENSURE(UnitTest_Enum<EFruit_Local>(),  "UnitTest_Enum_Local Failed");
	JPT_ENSURE(UnitTest_Enum<EFruit_Global>(),  "UnitTest_Enum_Global Failed");
	JPT_ENSURE(UnitTest_Enum<GlobalEnumContainer::EFruit_GlobalEnumContainer>(), "UnitTest_Enum_GlobalEnumContainer::EFruit_GlobalEnumContainer Failed");

	return true;
}
