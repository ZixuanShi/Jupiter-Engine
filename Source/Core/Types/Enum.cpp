// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Enum.h"

#include "Core/Minimal/Macros.h"
#include "Debugging/Logger.h"

using namespace jpt;

template<jpt::Integral TInt>
EnumData<TInt> GenerateData(const char* pSource)
{
	EnumData<TInt> data;

	// Parse source to individual tokens. Each token is a name=value pair. where value is optional.
	jpt::String source = pSource;
	source.Replace(" ", "");
	const jpt::DynamicArray<jpt::String> tokens = source.Split(',');
	data.names.Resize(tokens.Size());

	// Parse each token to extract name and value.
	TInt key = 0;
	for (size_t i = 0; i < tokens.Size(); ++i)
	{
		const jpt::String& token = tokens[i];
		jpt::String name = token;

		// If token contains an equal sign, then it is a name=value pair. We need to extract the value and assign it to the key.
		// Example: "Name", Name=5", "Name=(1<<2)"
		if (const size_t equalIndex = tokens[i].Find('='); equalIndex != jpt::npos)
		{
			name = tokens[i].SubStr(0, equalIndex);

			const jpt::String valueStr = tokens[i].SubStr(equalIndex + 1, tokens[i].Size() - equalIndex - 1);

			// valueStr could be either a number or a flag bitshift. Such as "Name=5", "Name=(1<<2)". We need to evaluate it. 
			if (const size_t shiftIndex = valueStr.Find("<<"); shiftIndex != jpt::npos)
			{
				const jpt::String left  = valueStr.SubStr(1, shiftIndex - 1);
				const jpt::String right = valueStr.SubStr(shiftIndex + 2, valueStr.Size() - shiftIndex - 3);
				const TInt leftValue  = jpt::CStrToInteger<char, TInt>(left.ConstBuffer());
				const TInt rightValue = jpt::CStrToInteger<char, TInt>(right.ConstBuffer());
				key = (leftValue << rightValue);
			}
			// valueStr is a number. Convert it to integer
			else
			{
				key = jpt::CStrToInteger<char, TInt>(valueStr.ConstBuffer());
			}
		}

		// Update min and max values
		if (key < data.min)
		{
			data.min = key;
		}
		else if (key > data.max)
		{
			data.max = key;
		}

		// This token has been processed. Store the name and increment the key.
		data.names[key] = name;
		++key;
	}

	return data;
}

// Explicit template instantiations
template EnumData<uint8>  GenerateData(const char* pSource);
template EnumData<uint16> GenerateData(const char* pSource);
template EnumData<uint32> GenerateData(const char* pSource);
template EnumData<uint64> GenerateData(const char* pSource);