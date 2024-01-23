// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Macros.h"
#include "Debugging/Assert.h"

export module jpt.ToString;

import jpt.Concepts;
import jpt.TypeTraits;
import jpt.Utilities;
import jpt.String;

export namespace jpt
{
	template<typename T>
	concept EnabledToString = requires(T object) { object.ToString(); };

	template<typename T>
	concept EnabledToWString = requires(T object) { object.ToWString(); };

	template<typename T>
	concept NoBuiltInToStringPrimitive = Integral<T> || Floating<T> || IsSameType<T, bool> || StringLiteral<T>;	// Add any additional primitive types if implemented later

	// Any non-primitive object that has ToString() implemented
	template<EnabledToString T>
	String ToString(const T& object)
	{
		return object.ToString();
	}

	template<EnabledToString T>
	WString ToWString(const T& object)
	{
		return object.ToWString();
	}

	// int, uint
	template<BasicStringType StringT = jpt::String, Integral IntT>
	StringT ToString(IntT integer)
	{
		using CharT = StringT::CharT;

		CharT* integerCString = IntegerToCStr<CharT>(integer);
		StringT integerString;
		integerString.MoveString(integerCString);
		return integerString;
	}

	// float, double
	template<BasicStringType StringT = jpt::String, Floating FloatT>
	StringT ToString(FloatT value)
	{
		using CharT = StringT::CharT;

		CharT* floatCString = FloatToCStr<CharT>(value);
		StringT floatString;
		floatString.MoveString(floatCString);
		return floatString;
	}

	// bool
	template<BasicStringType StringT = jpt::String>
	StringT ToString(bool value)
	{
		using CharT = StringT::CharT;

		StringT boolStr;
		if (value)
		{
			boolStr = JPT_GET_PROPER_STRING(CharT, true);
		}
		else
		{
			boolStr = JPT_GET_PROPER_STRING(CharT, false);
		}
		return boolStr;
	}

	// char
	template<BasicStringType StringT = jpt::String>
	StringT ToString(typename StringT::CharT c)
	{
		return StringT(c);
	}
}