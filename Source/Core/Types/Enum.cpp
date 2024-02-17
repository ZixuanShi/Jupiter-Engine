// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Enum.h"

#include "Core/Minimal/Macros.h"
#include "Debugging/Logger.h"

using namespace jpt;

EnumData GenerateData(const char* pSource)
{
	EnumData data;

	// Parse source
	jpt::String source = pSource;
	source.Replace(" ", "");

	jpt::DynamicArray<jpt::String> tokens = source.Split(',');

	TEnumSize key = 0;
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

		if (key < data.min)
		{
			data.min = key;
		}

		if (key > data.max)
		{
			data.max = key;
		}

		data.names[key] = name;
		++key;
	}

	data.count = static_cast<TEnumSize>(tokens.Size());

	return data;
}