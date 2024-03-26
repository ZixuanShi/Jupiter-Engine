// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Core/Types/Enum.h"

export module UnitTests_Enum_Global;

// Global Enum
export JPT_ENUM_UINT8(Fruit_Global,
	Apple = 5,
	Banana,
	Orange = 9,
	Grape);

export JPT_ENUM_UINT8(Weapon_Global,
	Sword,
	Spear,
	Hammer,
	Axe);

// Global class containing an enum
export class GlobalEnumContainer
{
public:
	JPT_ENUM_UINT8(Fruit_Nested,
		Apple = 5,
		Banana,
		Orange = 9,
		Grape);

	JPT_ENUM_UINT8(Weapon_Nested,
		Sword,
		Spear,
		Hammer,
		Axe);
};