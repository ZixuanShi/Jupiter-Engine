// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Enum.h"

using namespace jpt;

import jpt.Optional;
import jpt.StringView;
import jpt.Utilities;

enum class locOperation
{
	LeftShift,
	Or
};

static String locGetEnumSourceStr(const char* pSource)
{
	String source = pSource;

	// Remove spaces
	source.Replace(" ", ""); 

	// Remove trailing commas
	if (source.Back() == ',')
	{
		source.PopBack();
	}

	return source;
}

template<Integral TInt>
Optional<TInt> EvaluateOperator(const String& expression, const String& operatorStr, locOperation operation)
{
	// valueStr could be either a number or a flag bitshift. Such as "Name=5", "Name=(1<<2)". We need to evaluate it. 
	if (const size_t shiftIndex = expression.Find(operatorStr.ConstBuffer()); shiftIndex != npos)
	{
		const String left  = expression.SubStr(0, shiftIndex);
		const String right = expression.SubStr(shiftIndex + operatorStr.Count(), expression.Count() - shiftIndex - operatorStr.Count());
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
TInt Evaluate(const String& valueStr)
{
	// Remove parenthesis if present
	String expression = valueStr;
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
	else if (expression.BeginsWith("0x"))
	{
		return CStrToInteger<char, TInt>(expression.ConstBuffer(), expression.Count(), EIntBase::Hex);
	}

	return CStrToInteger<char, TInt>(expression.ConstBuffer(), expression.Count());
}

template<Integral TInt>
EnumData<TInt> GenerateData(const char* pSource)
{
	EnumData<TInt> data;

	const String source = locGetEnumSourceStr(pSource);

	DynamicArray<String> tokens = source.Split(',');
	data.names.Reserve(tokens.Count());

	// Parse each token to extract name and value.
	TInt value = 0;
	for (String& token : tokens)
	{
		String name;

		// If token Has an equal sign, then it is a name=value pair. We need to extract the value and assign it to the key.
		if (const size_t equalIndex = token.Find('='); equalIndex != npos)
		{
			name = token.SubStr(0, equalIndex);

			const String valueStr = token.SubStr(equalIndex + 1, token.Count() - equalIndex - 1);
			value  = Evaluate<TInt>(valueStr);
		}
		else
		{
			name = Move(token);
		}

		// Update min and max values
		if (value < data.min)
		{
			data.min = value;
		}
		if (value > data.max)
		{
			data.max = value;
		}

		// This token has been processed. Store the name and increment the value.
		data.names[value] = Move(name);
		++value;
	}

	return data;
}

// Explicit template instantiations
template EnumData<uint8>  GenerateData(const char* pSource);
template EnumData<uint16> GenerateData(const char* pSource);
template EnumData<uint32> GenerateData(const char* pSource);
template EnumData<uint64> GenerateData(const char* pSource);