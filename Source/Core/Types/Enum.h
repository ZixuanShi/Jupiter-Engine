// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <initializer_list>

import jpt.TypeDefs;
import jpt.DynamicArray;
import jpt.String;

void EnumBuilder(jpt::DynamicArray<jpt::String>& names, uint8 count, const char* pSource);

	/** An Enum class supports the following operations:
		- operator++/--: prefix and postfix increment/decrement
		- ToString(): returns the string representation of the enum
		- FromString(): returns the enum value from the string representation
	*/
#define JPT_ENUM(EnumName, ...)\
class EnumName\
{\
public:\
	using TSize = uint8; /* If ran out of space later, change this to larger uint */ \
	\
	enum EData : uint8\
	{\
		__VA_ARGS__, Count, Start = 0\
	};\
	\
private:\
	jpt::DynamicArray<jpt::String> m_names;\
	TSize m_value = Start;\
	\
public:\
	EnumName()\
	{\
		EnumBuilder(m_names, Count, #__VA_ARGS__);\
	}\
	\
	EnumName(EData data)\
		: m_value(data)\
	{\
		EnumBuilder(m_names, Count, #__VA_ARGS__);\
	}\
	\
	EnumName& operator=(EData data)\
	{\
		m_value = data;\
		return *this;\
	}\
	\
private:\
};