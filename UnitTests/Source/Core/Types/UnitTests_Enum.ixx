// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include "Core/Types/Enum.h"

export module UnitTests_Enum;

import jpt.TypeDefs;
import jpt.RandomNumberGenerator;
import jpt.String;
import jpt.StringUtils;
import jpt.HashMap;
import jpt.DynamicArray;
import jpt.StaticArray;
import jpt.Limits;

//-----------------------------------------------------------------------------------------------
struct Data
{
	size_t count;
	size_t start;
	jpt::HashMap<TEnumSize, jpt::String> names;
};

class EnumDummy
{
public:
	enum EEnumDummyData : TEnumSize
	{
		Apple = 1,
		Banana,
		Orange = 7
	};
	static Data s_data;

private:
	TEnumSize m_value = 0;

public:
	EnumDummy()
	{
	}

	EnumDummy(EEnumDummyData data)
		: m_value(data)
	{
		
	}

	EnumDummy& operator=(EEnumDummyData data)
	{
		m_value = data;
		return *this; 
	}
};

// Apple = 1, Banana, Orange = 7
static Data GenerateData(const char* pSource)
{
	Data data;

	// Parse source
	jpt::String source = pSource;
	source.Replace(" ", "");	

	// Apple=1,
	// Banana,
	// Orange=7
	jpt::DynamicArray<jpt::String> tokens = source.Split(',');

	TEnumSize key = 0;
	TEnumSize start = jpt::LimitsOf<TEnumSize>::kMax;
	for (size_t i = 0; i < tokens.Size(); ++i)
	{
		jpt::String token = tokens[i];
		jpt::String name = token;

		if (size_t equalIndex = tokens[i].Find('='); equalIndex != jpt::npos)
		{
			name = tokens[i].SubStr(0, equalIndex);
			jpt::String valueStr = tokens[i].SubStr(equalIndex + 1, tokens[i].Size() - equalIndex - 1);
			key = jpt::CStrToInteger<char, TEnumSize>(valueStr.ConstBuffer());
		}

		if (key < start)
		{
			start = key;
		}

		data.names[key] = name;
		++key;
	}

	data.count = tokens.Size();
	data.start = start;

	return data;
}

Data EnumDummy::s_data = GenerateData("Apple = 1, Banana, Orange = 7");

//-----------------------------------------------------------------------------------------------

JPT_ENUM(EFruits, 
	Apple = 1,
	Banana,
    Orange = 7);

bool UnitTest_GlobalEnum()
{
	EnumDummy::Apple;
	EnumDummy::Banana;
	EnumDummy::Orange;

	JPT_LOG(EnumDummy::s_data.count);
	JPT_LOG(EnumDummy::s_data.start);
	JPT_LOG(EnumDummy::s_data.names);


	EFruits fruit = EFruits::Apple;
	EFruits otherFruit = EFruits::Banana;

	EFruits::Apple;
	EFruits::Banana;
	EFruits::Orange;

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
	JPT_ENSURE(UnitTest_GlobalEnum(), "UnitTest_GlobalEnum Failed");
	//JPT_ENSURE(UnitTest_LocalEnum(), "UnitTest_LocalEnum Failed");
	//JPT_ENSURE(UnitTest_NormalEnum(), "UnitTest_NormalEnum Failed");

	return true;
}
