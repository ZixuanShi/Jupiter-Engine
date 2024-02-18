// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.TypeDefs;
import jpt.String;
import jpt.StringUtils;
import jpt.HashMap;
import jpt.DynamicArray;
import jpt.Limits;

/** If an Enum ran out of space later, change this to a larger uint */
using TEnumSize = uint8; 

/** Contains shared enum data across all instances and public */
struct EnumData
{
	TEnumSize count = 0;	/**< How many values this enum has */
	TEnumSize min = jpt::LimitsOf<TEnumSize>::kMax;	/**< The min value of this enum */
	TEnumSize max = jpt::LimitsOf<TEnumSize>::kMin;	/**< The max value of this enum */
	jpt::HashMap<TEnumSize, jpt::String> names;		/**< The names map of the enum values */
};

/** Generate the shared enum data 
	@param pSource		A string contains all the enum values, separated by ',' accepts value assignment too */
EnumData GenerateData(const char* pSource);

/** Enum wrapper supports the followings:
	- Static global API for shared EnumData
	- Comparing with numeric and string
	- Conversion to numeric and string
	- Common Math operators
	- Iteration through all values. Range-based is also supported

	@examples: 	
		// Local Enum for current file
		JPT_ENUM(ELocal, A, B = 2, C = 5);
		
		// Global Enum for all files as module
		export JPT_ENUM(EGlobal, A, B = 2, C = 5);
		
		// Nested Enum in class
		export class GlobalEnumContainer
		{
		public:
			JPT_ENUM(ENested, A, B = 2, C = 5);
		}; */
#define JPT_ENUM(EnumName, ...)\
class EnumName\
{\
public:\
	enum EValues : TEnumSize\
	{\
		__VA_ARGS__\
	};\
	\
private:\
	inline static const EnumData s_data = GenerateData(#__VA_ARGS__);\
	TEnumSize m_value = 0;	/**< The actual enum value of one instance */ \
	\
public:\
	/** Static global API. See EnumData for more details at Enum.h */\
	/**	@example: EFoo::Count() */\
	\
	constexpr static TEnumSize Count() { return s_data.count; }\
	constexpr static TEnumSize Min()   { return s_data.min; }\
	constexpr static TEnumSize Max()   { return s_data.max; }\
	constexpr static const jpt::HashMap<TEnumSize, jpt::String>& Names() { return s_data.names; }\
	constexpr static const jpt::String& Name(TEnumSize index)\
	{\
		JPT_ASSERT(s_data.names.Contains(index));\
		return s_data.names[index];\
	}\
	\
public:\
	/** Member Constructor & operator= */\
	constexpr EnumName() = default;\
	\
	constexpr EnumName(EnumName::EValues value)	: m_value(value) {}\
	\
	constexpr EnumName& operator=(EnumName::EValues value)\
	{\
		m_value = value; \
		return *this; \
	}\
	\
	template<jpt::Integral TInt>\
	constexpr EnumName(TInt integer): m_value(static_cast<TEnumSize>(integer)) {}\
	\
	template<jpt::Integral TInt> \
	constexpr EnumName& operator=(TInt integer)\
	{\
		m_value = static_cast<TEnumSize>(integer); \
		return *this; \
	}\
	\
	/** Math operators */\
	/** If you are using math operators. Make sure you exactly know the result is a valid JPT_ENUM(YourEnum)'s value */\
    /** JPT_ENUM's values are not guaranteed linear and contigous, you will have assertion failed if that's the case */\
	template<jpt::Integral TInt>\
	constexpr EnumName& operator+=(TInt offset)\
	{\
		m_value += static_cast<TEnumSize>(offset);\
		JPT_ASSERT(s_data.names.Contains(m_value));\
		return *this;\
	}\
	template<jpt::Integral TInt>\
	constexpr EnumName& operator-=(TInt offset)\
	{\
		m_value -= static_cast<TEnumSize>(offset);\
		JPT_ASSERT(s_data.names.Contains(m_value));\
		return *this;\
	}\
	constexpr EnumName& operator++()\
	{\
		return *this += 1;\
	}\
	constexpr EnumName& operator--()\
	{\
		return *this -= 1;\
	}\
	template<jpt::Integral TInt>\
	constexpr EnumName operator+(TInt offset)\
	{\
		EnumName copy = *this;\
		return copy += offset;\
	}\
	template<jpt::Integral TInt>\
	constexpr EnumName operator-(TInt offset)\
	{\
		EnumName copy = *this;\
		return copy -= offset;\
	}\
	\
	/** Iteration */\
	/** Supports range-based, iterated, and numeric if items are linear and contigous */\
	constexpr auto begin()  const { return s_data.names.begin();  }\
	constexpr auto end()    const { return s_data.names.end();    }\
	constexpr auto cbegin() const { return s_data.names.cbegin(); }\
	constexpr auto cend()   const { return s_data.names.cend();   }\
	\
	/** Comparison */\
	template<jpt::Integral TInt>\
	constexpr bool operator==(TInt value) const { return m_value == static_cast<TEnumSize>(value); }\
	constexpr bool operator==(const char* str) const { return ToString() == str; }\
	\
	/** Numeric value access */\
	constexpr TEnumSize Value()    const { return m_value; }\
	constexpr operator TEnumSize() const { return m_value; }\
	\
	/** String value access */\
	constexpr const jpt::String& ToString()  const { return s_data.names[m_value]; }\
	constexpr operator const jpt::String&()  const { return s_data.names[m_value]; }\
};

