// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "Enum.h"

namespace jpt
{
	bool locIsValid(char c)
	{
		return c && c != ',' && c != ' ' && c != '\t';
	}

	bool locIsWhiteSpace(char c)
	{
		return c == ',' || c == ' ' || c == '\t';
	}

	EnumSearchInfo BuildEnumStringInfos(const char* rawEnumString, EnumStringInfo* infos, int32 start, size_t count)
	{
		const EnumSearchInfo searchInfo = { rawEnumString, infos, start, count };
		JPT_ASSERT(searchInfo.m_startValue + searchInfo.m_count < ULONGLONG_MAX, "searchInfo exceeds maximum integer value");

		// iterate through string finding the comma delimiters
		size_t pos = 0;
		for (size_t i = 0; i < count; ++i)
		{
			infos[i].m_start = pos;

			// search until comma, whitespace or end of string for the name of the enum
			while (locIsValid(rawEnumString[pos]))
			{
				++pos;
			}

			JPT_ASSERT(pos != infos[i].m_start, "Empty string found in the middle of an enum");
			infos[i].m_end = pos;

			// Skip comma and subsequence whitespace to next identifier
			while (locIsWhiteSpace(rawEnumString[pos]))
			{
				++pos;
			}

			JPT_ASSERT(rawEnumString[pos] || i == count - 1, "Enum string ended too early");
		}

		JPT_ASSERT(pos < ULONGLONG_MAX, "Enum string length total must be less than 65k");
		return searchInfo;
	}

	jpt::string GetSubString(int32 value, const EnumSearchInfo& searchInfo)
	{
		value -= searchInfo.m_startValue;
		JPT_ASSERT(value >= 0 && value < searchInfo.m_count, "Invalid value to search for string_view");

		const char* enumString = searchInfo.m_rawEnumString;
		const EnumStringInfo* infos = searchInfo.m_nameParts;

		const jpt::string enumStringStart(enumString + infos[value].m_start);
		return enumStringStart.substr(0, infos[value].Len());
	}

	jpt::string_view GetSubStringView(int32 value, const EnumSearchInfo& searchInfo)
	{
		value -= searchInfo.m_startValue;
		JPT_ASSERT(value >= 0 && value < searchInfo.m_count, "Invalid value to search for string_view");

		const char* enumString = searchInfo.m_rawEnumString;
		const EnumStringInfo* infos = searchInfo.m_nameParts;

		return jpt::string_view(enumString + infos[value].m_start, infos[value].Len());
	}
}