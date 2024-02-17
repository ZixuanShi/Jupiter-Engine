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
import jpt.Limits;

//-----------------------------------------------------------------------------------------------
class EnumDummy
{
public:
	enum EEnumDummyData : TEnumSize
	{
		Apple = 5,
		Banana,
		Orange = 7
	};

	static EnumData s_data;

private:
	TEnumSize m_value = 0;

public:
	EnumDummy(EEnumDummyData data)
		: m_value(data)
	{
	}

	EnumDummy& operator=(EEnumDummyData data)
	{
		m_value = data;
		return *this; 
	}

	TEnumSize Value()    const { return m_value; }
	operator TEnumSize() const { return m_value; }
	const jpt::String& ToString() const { return s_data.names[m_value]; }
	operator const jpt::String&() const { return s_data.names[m_value]; }

	static TEnumSize Count() { return s_data.count; }
	static TEnumSize Start() { return s_data.start; }

	static const jpt::HashMap<TEnumSize, jpt::String>& Names()
	{
		return s_data.names;
	}

	static const jpt::String& Name(TEnumSize index) 
	{
		JPT_ASSERT(s_data.names.Contains(index));
		return s_data.names[index];
	}
};

EnumData EnumDummy::s_data = GenerateData("Apple = 5, Banana, Orange = 7");

//-----------------------------------------------------------------------------------------------

JPT_ENUM(EFruits, 
	Apple = 5,
	Banana,
    Orange = 7);

bool UnitTest_GlobalEnum()
{
	EnumDummy::Apple;
	EnumDummy::Banana;
	EnumDummy::Orange;

	JPT_LOG(jpt::String("EnumDummy Count: ") + jpt::ToString(EnumDummy::Count()));
	JPT_LOG(jpt::String("EnumDummy Start: ") + jpt::ToString(EnumDummy::Start()));
	JPT_LOG(EnumDummy::Names());
	JPT_LOG(EnumDummy::Name(EnumDummy::Apple));

	EnumDummy dummy = EnumDummy::Apple;
	dummy = EnumDummy::Orange;
	JPT_LOG(dummy.Value());
	JPT_LOG(dummy);


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
