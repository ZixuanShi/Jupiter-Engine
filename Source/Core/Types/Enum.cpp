// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Enum.h"

using namespace jpt;

import jpt.Optional;
import jpt.StringView;
import jpt.Utilities;
 
/** @return		Each individual enum data tokens */
DynamicArray<String> GetTokens(const char* pSource)
{
	String source = pSource;
	source.Replace(" ", "");	
	return source.Split(',');
}

enum class locOperation
{
	LeftShift,
	Or
};

template<Integral TInt>
Optional<TInt> EvaluateOperator(StringView expression, StringView operatorStr, locOperation operation)
{
	// valueStr could be either a number or a flag bitshift. Such as "Name=5", "Name=(1<<2)". We need to evaluate it. 
	if (const size_t shiftIndex = expression.Find(operatorStr.ConstBuffer()); shiftIndex != npos)
	{
		const StringView left  = expression.SubStr(0, shiftIndex);
		const StringView right = expression.SubStr(shiftIndex + operatorStr.Count(), expression.Count() - shiftIndex - operatorStr.Count());
		const TInt leftValue  = CStrToInteger<char, TInt>(left.ConstBuffer(),  left.Count());
		const TInt rightValue = CStrToInteger<char, TInt>(right.ConstBuffer(), right.Count());

		switch (operation)
		{
		case locOperation::LeftShift:
			return (leftValue << rightValue);

		case locOperation::Or:
			return (leftValue | rightValue);

		default:
			break;
		}
	}

	return Optional<TInt>();
}

template<Integral TInt>
TInt Evaluate(StringView valueStr)
{
	// Remove parenthesis if present
	StringView expression = valueStr;
	if (expression.Front() == '(' && expression.Back() == ')')
	{
		expression = expression.SubStr(1, expression.Count() - 2);
	}

	// valueStr could be either a number or a flag bitshift. Such as "Name=5", "Name=(1<<2)". We need to evaluate it. 
	if (Optional<TInt> result = EvaluateOperator<TInt>(expression, "<<", locOperation::LeftShift); result.HasValue())
	{
		return result.Value();
	}
	// Hex. starts with 0x
	else if (const size_t hexIndex = expression.Find("0x"); hexIndex == 0)
	{
		return CStrToInteger<char, TInt>(expression.ConstBuffer(), expression.Count(), EIntBase::Hex);
	}

	return CStrToInteger<char, TInt>(expression.ConstBuffer(), expression.Count());
}

template<Integral TInt>
EnumData<TInt> GenerateData(const char* pSource)
{
	EnumData<TInt> data;

	DynamicArray<String> tokens = GetTokens(pSource);
	data.names.Reserve(tokens.Count());

	// Parse each token to extract name and value.
	TInt key = 0;
	for (String& token : tokens)
	{
		String name;

		// If token Has an equal sign, then it is a name=value pair. We need to extract the value and assign it to the key.
		// Example: "Name", Name=5", "Name=(1<<2)", "Nanme=0x10", "Name=0xFF00|Foo"
		if (const size_t equalIndex = token.Find('='); equalIndex != npos)
		{
			name = token.SubStr(0, equalIndex);

			const String valueStr = token.SubStr(equalIndex + 1, token.Count() - equalIndex - 1);
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