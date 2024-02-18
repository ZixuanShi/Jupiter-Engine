// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once
#include "Debugging/Logger.h"

import jpt.TypeDefs;
import jpt.Concepts;
import jpt.String;
import jpt.StringUtils;
import jpt.HashMap;
import jpt.DynamicArray;
import jpt.Limits;

/** Contains shared enum data across all instances and public */
template<jpt::Integral TInt>
struct EnumData
{
	TInt min = jpt::LimitsOf<TInt>::kMax;	/**< The min value of this enum */
	TInt max = jpt::LimitsOf<TInt>::kMin;	/**< The max value of this enum */
	jpt::HashMap<TInt, jpt::String> names;	/**< The names map of the enum values */
};

/** Generate the shared enum data 
	@param pSource		A string contains all the enum values, separated by ',' accepts value assignment too */
template<jpt::Integral TInt>
EnumData<TInt> GenerateData(const char* pSource);

/** Enum wrapper supports the followings:
	- Static global API for shared EnumData
	- Comparing with numeric and string
	- Conversion to numeric and string
	- Common Math operators
	- Iteration through all values. Range-based is also supported

	@param EnumName		The name of the enum. Like EFruit. Prefix with 'E' is naming convention
	@param TSize		The size of the enum. uint8, uint16, uint32, uint64. 
						Make sure you choose the right size for optimization and Explicit template instantiations at the bottom Enum.cpp
	@examples: 	
		// Local Enum for current file
		JPT_ENUM_UINT8(ELocal, 
		A, 
		B = 2, 
		C = 5);

		// Local Enum flag
		JPT_ENUM_UINT8(ELocalFlag,
		A = (1 << 0),
		B = (1 << 1),
		C = (1 << 2));
		
		// Global Enum for all files as module
		export JPT_ENUM_UINT8(EGlobal, 
		A, 
		B = 2, 
		C = 5);
		
		// Nested Enum in class
		export class GlobalEnumContainer
		{
		public:
			JPT_ENUM_UINT64(ENested, 
			A, 
			B = 2, 
			C = 5);
		}; */
#define JPT_ENUM(EnumName, TSize, ...)\
class EnumName\
{\
public:\
	enum EValues : TSize\
	{\
		__VA_ARGS__\
	};\
	\
private:\
	inline static const EnumData<TSize> s_data = GenerateData<TSize>(#__VA_ARGS__);\
	TSize m_value = 0;	/**< The actual enum value of one instance */ \
	\
public:\
	/** Static global API. See EnumData for more details at Enum.h */\
	/**	@example: EFoo::Count() */\
	\
	constexpr static TSize Min()   { return s_data.min; }\
	constexpr static TSize Max()   { return s_data.max; }\
	constexpr static TSize Count() { return static_cast<TSize>(s_data.names.Size()); }\
	constexpr static const jpt::HashMap<TSize, jpt::String>& Names() { return s_data.names; }\
	constexpr static const jpt::String& Name(TSize index) { JPT_ASSERT(s_data.names.Contains(index)); return s_data.names[index]; }\
	\
public:\
	/** Member Constructor & operator= */\
	constexpr EnumName() = default;\
	constexpr EnumName(EnumName::EValues value)	: m_value(value) {}\
	constexpr EnumName& operator=(EnumName::EValues value) { m_value = value; return *this; }\
	\
	template<jpt::Integral TInt = TSize>\
	constexpr EnumName(TInt integer)\
		: m_value(static_cast<TSize>(integer)) \
	{\
	}\
	\
	template<jpt::Integral TInt = TSize> \
	constexpr EnumName& operator=(TInt integer) \
	{ \
		m_value = static_cast<TSize>(integer); \
		return *this; \
	} \
	\
	/** Math operators */\
	/** If you are using math operators. Make sure you exactly know the result is a valid JPT_ENUM(YourEnum)'s value */\
    /** JPT_ENUM's values are not guaranteed linear and contigous, you will have assertion failed if that's the case */\
	\
	/** +=, -= */\
	template<jpt::Integral TInt = TSize>\
	constexpr EnumName& operator+=(TInt offset)\
	{\
		m_value += static_cast<TSize>(offset);\
		JPT_ASSERT(s_data.names.Contains(m_value));\
		return *this;\
	}\
	template<jpt::Integral TInt = TSize>\
	constexpr EnumName& operator-=(TInt offset)\
	{\
		m_value -= static_cast<TSize>(offset);\
		JPT_ASSERT(s_data.names.Contains(m_value));\
		return *this;\
	}\
	\
	/** ++, -- */\
	constexpr EnumName& operator++() { return *this += 1; }\
	constexpr EnumName& operator--() { return *this -= 1; }\
	\
	/** +, - */\
	template<jpt::Integral TInt>\
	constexpr EnumName operator+(TInt offset) {	EnumName copy = *this; return copy += offset; }\
	template<jpt::Integral TInt>\
	constexpr EnumName operator-(TInt offset) {	EnumName copy = *this; return copy -= offset; }\
	\
	/** Bitwise */\
	constexpr EnumName& operator&=(EValues value) { m_value &= value; return *this; } \
	constexpr EnumName& operator|=(EValues value) { m_value |= value; return *this; } \
	constexpr EnumName& operator^=(EValues value) { m_value ^= value; return *this; } \
	constexpr TSize operator&(EValues value) const { return m_value & value; } \
	constexpr TSize operator|(EValues value) const { return m_value | value; } \
	constexpr TSize operator^(EValues value) const { return m_value ^ value; } \
	constexpr TSize operator~() const { return ~m_value; } \
	constexpr bool Has(EValues value) const { return (m_value & value) != 0; } \
	constexpr void Insert(EValues value) { m_value |= value; } \
	constexpr void Toggle(EValues value) { m_value ^= value; } \
	\
	/** Iteration */\
	/** Supports range-based, iterated, and numeric if items are linear and contigous */\
	constexpr auto begin()  const { return s_data.names.begin();  }\
	constexpr auto end()    const { return s_data.names.end();    }\
	constexpr auto cbegin() const { return s_data.names.cbegin(); }\
	constexpr auto cend()   const { return s_data.names.cend();   }\
	\
	/** Comparison */\
	template<jpt::Integral TInt = TSize>\
	constexpr bool operator==(TInt value)      const { return m_value == static_cast<TSize>(value); }\
	constexpr bool operator==(const char* str) const { return ToString() == str; }\
	\
	/** Numeric value access */\
	constexpr TSize Value()    const { return m_value; }\
	constexpr operator TSize() const { return m_value; }\
	\
	/** String value access */\
	constexpr const jpt::String& ToString() const { return s_data.names[m_value]; }\
	constexpr operator const jpt::String&() const { return s_data.names[m_value]; }\
};

#define JPT_ENUM_UINT8( EnumName, ...) JPT_ENUM(EnumName, uint8,  __VA_ARGS__)
#define JPT_ENUM_UINT16(EnumName, ...) JPT_ENUM(EnumName, uint16, __VA_ARGS__)
#define JPT_ENUM_UINT32(EnumName, ...) JPT_ENUM(EnumName, uint32, __VA_ARGS__)
#define JPT_ENUM_UINT64(EnumName, ...) JPT_ENUM(EnumName, uint64, __VA_ARGS__)

