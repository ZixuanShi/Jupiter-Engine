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

private:
	static const EnumData s_data;
	TEnumSize m_value = 0;

public:
	/** Static global API */
	constexpr static TEnumSize Count() { return s_data.count; }
	constexpr static TEnumSize Min()   { return s_data.min; }
	constexpr static TEnumSize Max()   { return s_data.max; }
	constexpr static const jpt::HashMap<TEnumSize, jpt::String>& Names();
	constexpr static const jpt::String& Name(TEnumSize index);

public:
	/** Member API */
	constexpr EnumDummy(TEnumSize data);
	constexpr EnumDummy& operator=(TEnumSize data);

	// Modifier
	constexpr EnumDummy& operator++();
	constexpr EnumDummy& operator--();

	// Iteration
	constexpr auto begin()  const { return s_data.names.begin();  }
	constexpr auto end()    const { return s_data.names.end();    }
	constexpr auto cbegin() const { return s_data.names.cbegin(); }
	constexpr auto cend()   const { return s_data.names.cend();   }

	// Comparison
	template<jpt::Integral TInt>
	constexpr bool operator==(TInt value) const;
	constexpr bool operator==(const char* str) const;

	// Numeric value access
	constexpr TEnumSize Value()    const { return m_value; }
	constexpr operator TEnumSize() const { return m_value; }

	// String value access
	constexpr const jpt::String& ToString() const { return s_data.names[m_value]; }
	constexpr operator const jpt::String&() const { return s_data.names[m_value]; }
};

const EnumData EnumDummy::s_data = GenerateData("Apple = 5, Banana, Orange = 7");

constexpr EnumDummy::EnumDummy(TEnumSize data)
	: m_value(data)
{
}

constexpr EnumDummy& EnumDummy::operator=(TEnumSize data)
{
	m_value = data;
	return *this;
}

constexpr EnumDummy& EnumDummy::operator++()
{
	++m_value;
	JPT_ASSERT(s_data.names.Contains(m_value));
	return *this;
}

constexpr EnumDummy& EnumDummy::operator--()
{
	--m_value;
	JPT_ASSERT(s_data.names.Contains(m_value));
	return *this;
}

template<jpt::Integral TInt>
constexpr bool EnumDummy::operator==(TInt value) const
{
	return m_value == static_cast<TEnumSize>(value);
}

constexpr bool EnumDummy::operator==(const char* str) const
{
	return ToString() == str;
}

constexpr const jpt::HashMap<TEnumSize, jpt::String>& EnumDummy::Names()
{
	return s_data.names;
}

constexpr const jpt::String& EnumDummy::Name(TEnumSize index)
{
	JPT_ASSERT(s_data.names.Contains(index));
	return s_data.names[index];
}

bool UnitTest_TestEnum()
{
	JPT_ENSURE(EnumDummy::Count() == 3, "");
	JPT_ENSURE(EnumDummy::Min() == EnumDummy::Apple, "");
	JPT_ENSURE(EnumDummy::Max()   == EnumDummy::Orange, "");

	EnumDummy dummy(EnumDummy::Apple);
	JPT_ENSURE(dummy == 5, "");
	JPT_ENSURE(dummy == EnumDummy::Apple, "");
	JPT_ENSURE(dummy != EnumDummy::Orange, "");
	JPT_ENSURE(dummy == "Apple", "");

	dummy = EnumDummy::Banana;
	JPT_ENSURE(dummy == 6, "");
	JPT_ENSURE(dummy == EnumDummy::Banana, "");
	JPT_ENSURE(dummy == "Banana", "");

	--dummy;
	JPT_ENSURE(dummy == 5, "");
	JPT_ENSURE(dummy == EnumDummy::Apple, "");
	JPT_ENSURE(dummy == "Apple", "");

	return true;
}

//-----------------------------------------------------------------------------------------------

bool UnitTest_GlobalEnum()
{
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
	JPT_ENSURE(UnitTest_TestEnum(), "UnitTest_TestEnum Failed");
	JPT_ENSURE(UnitTest_GlobalEnum(), "UnitTest_GlobalEnum Failed");
	//JPT_ENSURE(UnitTest_LocalEnum(), "UnitTest_LocalEnum Failed");
	//JPT_ENSURE(UnitTest_NormalEnum(), "UnitTest_NormalEnum Failed");

	return true;
}
