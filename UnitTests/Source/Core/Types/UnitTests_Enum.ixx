// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include "Core/Types/Enum.h"

export module UnitTests_Enum;

import jpt.TypeDefs;

import jpt.RandomNumberGenerator;

struct Data
{
	int32 value1;
	int32 value2;
};

class Foo
{
public:
	static Data s_data;
};

static Data GenerateData()
{
	return { jpt::RNG::Global().RandInMax(100), jpt::RNG::Global().RandInMax(100) };
}

Data Foo::s_data = GenerateData();

JPT_ENUM(EFruits, 
	Apple = 1,
	Banana,
    Orange = 7);

const int32 n = 3;
enum EData
{
	Light = 1,
	Medium,
	Heavy = 7,

	Count = n,
	Start = Light
};

bool UnitTest_GlobalEnum()
{
	EFruits fruit = EFruits::Apple;
	EFruits otherFruit = EFruits::Banana;

	EFruits::Apple;
	EFruits::Banana;
	EFruits::Orange;
	//EFruits::Count;
	//EFruits::Start;

	Foo f;
	JPT_LOG(f.s_data.value1);
	JPT_LOG(f.s_data.value2);
	f.s_data.value1 += 100;
	JPT_LOG(f.s_data.value1);
	JPT_LOG(f.s_data.value2);
	JPT_LOG(Foo::s_data.value1);
	JPT_LOG(Foo::s_data.value2);

	return true;
}

bool UnitTest_NormalEnum()
{
	enum class EArmor
	{
		Light = 1,
		Medium,
		Heavy = 7,

		Count,
		Start = Light
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
	//JPT_RETURN_FALSE_IF_ERROR(!UnitTest_GlobalEnum(), "UnitTest_GlobalEnum Failed");
	//JPT_RETURN_FALSE_IF_ERROR(!UnitTest_LocalEnum(), "UnitTest_LocalEnum Failed");
	//JPT_RETURN_FALSE_IF_ERROR(!UnitTest_NormalEnum(), "UnitTest_NormalEnum Failed");

	return true;
}
