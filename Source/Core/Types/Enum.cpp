// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Enum.h"

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

using namespace jpt;

import jpt.TypeDefs;
import jpt.Optional;
import jpt.StringView;
 
/** @return		Each individual enum data tokens */
DynamicArray<String> GetTokens(const char* pSource)
{
	jpt::String source = pSource;
	source.Replace(" ", "");	
	return source.Split(',');
}

enum class EOperation
{
	LeftShift,
	Or
};

template<jpt::Integral TInt>
Optional<TInt> EvaluateOperator(jpt::StringView expression, jpt::StringView operatorStr, EOperation operation)
{
	// valueStr could be either a number or a flag bitshift. Such as "Name=5", "Name=(1<<2)". We need to evaluate it. 
	if (const size_t shiftIndex = expression.Find(operatorStr.Buffer()); shiftIndex != jpt::npos)
	{
		const jpt::StringView left  = expression.SubStr(0, shiftIndex);
		const jpt::StringView right = expression.SubStr(shiftIndex + operatorStr.Size(), expression.Size() - shiftIndex - operatorStr.Size());
		const TInt leftValue  = jpt::CStrToInteger<char, TInt>(left.Buffer(),  left.Size());
		const TInt rightValue = jpt::CStrToInteger<char, TInt>(right.Buffer(), right.Size());

		switch (operation)
		{
		case EOperation::LeftShift:
			return (leftValue << rightValue);

		case EOperation::Or:
			return (leftValue | rightValue);

		default:
			break;
		}
	}

	return Optional<TInt>();
}

template<jpt::Integral TInt>
TInt Evaluate(jpt::StringView valueStr)
{
	// Remove parenthesis if present
	jpt::StringView expression = valueStr;
	if (expression.Front() == '(' && expression.Back() == ')')
	{
		expression = expression.SubStr(1, expression.Size() - 2);
	}

	// valueStr could be either a number or a flag bitshift. Such as "Name=5", "Name=(1<<2)". We need to evaluate it. 
	if (Optional<TInt> result = EvaluateOperator<TInt>(expression, "<<", EOperation::LeftShift); result.HasValue())
	{
		return result.Value();
	}
	// Hex. starts with 0x
	else if (const size_t hexIndex = expression.Find("0x"); hexIndex == 0)
	{
		return jpt::CStrToInteger<char, TInt>(expression.Buffer(), expression.Size(), IntBase::Hex);
	}

	return jpt::CStrToInteger<char, TInt>(expression.Buffer(), expression.Size());
}

template<jpt::Integral TInt>
EnumData<TInt> GenerateData(const char* pSource)
{
	EnumData<TInt> data;

	DynamicArray<jpt::String> tokens = GetTokens(pSource);
	data.names.Resize(tokens.Size());

	// Parse each token to extract name and value.
	TInt key = 0;
	for (jpt::String& token : tokens)
	{
		jpt::String name;

		// If token contains an equal sign, then it is a name=value pair. We need to extract the value and assign it to the key.
		// Example: "Name", Name=5", "Name=(1<<2)"
		if (const size_t equalIndex = token.Find('='); equalIndex != jpt::npos)
		{
			name = token.SubStr(0, equalIndex);

			const jpt::String valueStr = token.SubStr(equalIndex + 1, token.Size() - equalIndex - 1);
			key  = Evaluate<TInt>(valueStr);
		}
		else
		{
			name = Move(token);
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
		data.names[key] = Move(name);
		++key;
	}

	return data;
}

// Explicit template instantiations
template EnumData<uint8>  GenerateData(const char* pSource);
template EnumData<uint16> GenerateData(const char* pSource);
template EnumData<uint32> GenerateData(const char* pSource);
template EnumData<uint64> GenerateData(const char* pSource);