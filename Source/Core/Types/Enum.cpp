// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Enum.h"

using namespace jpt;

import jpt.Optional;
import jpt.StringView;
import jpt.Utilities;
import jpt.Pair;

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
Optional<Pair<TInt, TInt>> EvaluateOperator(const String& expression, const String& operatorStr)
{
    const size_t operatorIndex = expression.Find(operatorStr.ConstBuffer());
    if (operatorIndex == npos)
    {
        return Optional<Pair<TInt, TInt>>();
    }

    const String lhsStr = expression.SubStr(0, operatorIndex);
    const String rhsStr = expression.SubStr(operatorIndex + operatorStr.Count(), expression.Count() - operatorIndex - operatorStr.Count());

    const TInt lhs = CStrToInteger<char, TInt>(lhsStr.ConstBuffer(), lhsStr.Count());
    const TInt rhs = CStrToInteger<char, TInt>(rhsStr.ConstBuffer(), rhsStr.Count());

    return Pair(lhs, rhs);
}

/** @return     Evaluated value numeric of the expression
    @param valueStr        An expression of value to assign. Operators may be involved. i.e. (1<<2), (1|2|4|8), (Foo|Bar|Baz) with alias, etc.
    @param evaluated    Previously evaluated items. Used when assigning values with alias */
template<Integral TInt>
TInt Evaluate(const String& valueStr)
{
    String expression = valueStr;

    // Remove parenthesis if present
    if (expression.Front() == '(' && expression.Back() == ')')
    {
        expression = expression.SubStr(1, expression.Count() - 2);
    }

    // If is a pure number, return it.
    if (IsInteger(valueStr.ConstBuffer()))
    {
        return CStrToInteger<char, TInt>(expression.ConstBuffer(), expression.Count());
    }
    else if (IsHexInteger(valueStr.ConstBuffer()))
    {
        return CStrToInteger<char, TInt>(expression.ConstBuffer(), expression.Count(), EIntBase::Hex);
    }

    // Operators involved, evaluate them.
    if (Optional<Pair<TInt, TInt>> pair = EvaluateOperator<TInt>(expression, "<<"); pair.HasValue())
    {
        const Pair<TInt, TInt> p = pair.Value();
        return p.first << p.second;
    }    

    return 0;
}

template<Integral TInt>
EnumData<TInt> GenerateData(const char* pSource)
{
    EnumData<TInt> data;

    const String source = locGetEnumSourceStr(pSource);

    DynamicArray<String> tokens = source.Split(',');
    data.names.ResizeBuckets(tokens.Count());

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