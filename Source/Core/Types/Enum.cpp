// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Enum.h"

#include "Core/Minimal/Macros.h"
#include "Debugging/Logger.h"

using namespace jpt;

// Apple = 1, Banana, Orange = 7
EnumData GenerateData(const char* pSource)
{
	EnumData data;

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

	data.count = static_cast<TEnumSize>(tokens.Size());
	data.start = start;

	return data;
}