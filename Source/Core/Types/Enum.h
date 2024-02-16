// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.TypeDefs;
import jpt.DynamicArray;
import jpt.StaticArray;
import jpt.String;
import jpt.Utilities;
import jpt.HashMap;

void BuildNames(jpt::HashMap<uint8, jpt::String>& namesMap, uint8& count, uint8& start, const char* pSource);

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
		__VA_ARGS__\
	};\
	\
private:\
	inline static jpt::HashMap<uint8, jpt::String> m_namesMap;\
	inline static TSize m_count = 0;\
	inline static TSize m_start = 0;\
	\
	TSize m_value = 0;\
	\
public:\
	EnumName()\
	{\
		BuildNames(m_namesMap, m_count, m_start, #__VA_ARGS__);\
	}\
	\
	EnumName(EData data)\
		: m_value(data)\
	{\
		BuildNames(m_namesMap, m_count, m_start, #__VA_ARGS__);\
	}\
	\
	EnumName& operator=(EData data)\
	{\
		m_value = data;\
		return *this;\
	}\
};