// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include "Core/Types/Enum.h"

export module UnitTests_Enum;

import jpt.TypeDefs;

JPT_ENUM(EFruits2, 
	Apple,
	Banana,
    Orange);

bool UnitTest_GlobalEnum()
{
	EFruits2 fruit = EFruits2::Apple;

	return true;
}

bool UnitTest_NormalEnum()
{
	enum class EArmor
	{
		Start = 0,

		Light = Start,
		Medium,
		Heavy,

		Count
	};

	EArmor armor = EArmor::Light;

	auto getArmor = [](EArmor arm)
	{
		switch (arm)
		{
		case EArmor::Light:
			return "Light";
		case EArmor::Medium:
			return "Medium";
		case EArmor::Heavy:
			return "Heavy";
		}

		return "Unknown";
	};

	JPT_LOG(getArmor(armor));

	for (EArmor i = EArmor::Start; i < EArmor::Count; i = EArmor(uint32(i) + 1))
	{
		JPT_LOG(getArmor(i));
	}

	return true;
}

bool UnitTest_LocalEnum()
{
	return true;
}

export bool RunUnitTests_Enum()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_GlobalEnum(), "UnitTest_GlobalEnum Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_LocalEnum(), "UnitTest_LocalEnum Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_NormalEnum(), "UnitTest_NormalEnum Failed");

	return true;
}
