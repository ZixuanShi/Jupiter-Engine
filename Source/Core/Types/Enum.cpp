// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Enum.h"

#include "Core/Minimal/Macros.h"
#include "Debugging/Logger.h"

using namespace jpt;

EnumData GenerateData(const char* pSource)
{
	EnumData data;

	// Parse source to individual tokens. Each token is a name=value pair. where value is optional.
	jpt::String source = pSource;
	source.Replace(" ", "");
	const jpt::DynamicArray<jpt::String> tokens = source.Split(',');
	data.count = static_cast<TEnumSize>(tokens.Size());

	// Parse each token to extract name and value.
	TEnumSize key = 0;
	for (size_t i = 0; i < tokens.Size(); ++i)
	{
		const jpt::String& token = tokens[i];
		jpt::String name = token;

		// If token contains an equal sign, then it is a name=value pair. We need to extract the value and assign it to the key.
		if (const size_t equalIndex = tokens[i].Find('='); equalIndex != jpt::npos)
		{
			name = tokens[i].SubStr(0, equalIndex);
			const jpt::String valueStr = tokens[i].SubStr(equalIndex + 1, tokens[i].Size() - equalIndex - 1);
			key = jpt::CStrToInteger<char, TEnumSize>(valueStr.ConstBuffer());
		}

		// Update min and max values
		if (key < data.min)
		{
			data.min = key;
		}
		if (key > data.max)
		{
			data.max = key;
		}

		// This token has been processed. Store the name and increment the key.
		data.names[key] = name;
		++key;
	}

	return data;
}