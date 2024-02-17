// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Core/Types/Enum.h"

export module UnitTests_Enum_Global;

// Global Enum
export JPT_ENUM(EFruit_Global,
	Apple = 5,
	Banana,
	Orange = 9,
	Grape);

// Global class containing an enum
export class GlobalEnumContainer
{
public:
	JPT_ENUM(EFruit_GlobalEnumContainer,
		Apple = 5,
		Banana,
		Orange = 9,
		Grape);
};