// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.TypeDefs;
import jpt.String;
import jpt.StringUtils;
import jpt.HashMap;
import jpt.DynamicArray;
import jpt.Limits;

using TEnumSize = uint8; /* If ran out of space later, change this to larger uint */

struct EnumData
{
	TEnumSize count = 0;
	TEnumSize min = jpt::LimitsOf<TEnumSize>::kMax;
	TEnumSize max = jpt::LimitsOf<TEnumSize>::kMin;
	jpt::HashMap<TEnumSize, jpt::String> names;
};

EnumData GenerateData(const char* pSource);

#define JPT_ENUM(EnumName, ...)\
class EnumName\
{\
public:\
	enum EData : TEnumSize\
	{\
		__VA_ARGS__ \
	};\
	\
	static EnumData s_data;\
	\
private:\
	TEnumSize m_value = 0;\
	\
public:\
	EnumName()\
	{\
		\
	}\
	\
	EnumName(EData data)\
		: m_value(data)\
	{\
		\
	}\
	\
	EnumName& operator=(EData data)\
	{\
		m_value = data;\
		return *this;\
	}\
};\

