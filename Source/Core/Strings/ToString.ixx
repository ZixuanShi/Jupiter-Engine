// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Macros.h"
#include "Debugging/Assert.h"

export module jpt.ToString;

export import jpt.String;
export import jpt.StringUtils;

import jpt.Concepts;
import jpt.TypeTraits;
import jpt.Utilities;

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
	constexpr String ToString(const T& object)
	{
		return object.ToString();
	}

	template<EnabledToString T>
	constexpr WString ToWString(const T& object)
	{
		return object.ToWString();
	}

	// int, uint
	template<BasicStringType StringT = jpt::String, Integral TInt>
	constexpr StringT ToString(TInt integer)
	{
		using TChar = StringT::TChar;

		TChar* integerCString = IntegerToCStr<TChar>(integer);
		StringT integerString;
		integerString.MoveString(integerCString);
		return integerString;
	}

	// float, double
	template<BasicStringType StringT = jpt::String, Floating TFloat>
	constexpr StringT ToString(TFloat value)
	{
		using TChar = StringT::TChar;

		TChar* floatCString = TFloatoCStr<TChar>(value);
		StringT floatString;
		floatString.MoveString(floatCString);
		return floatString;
	}

	// bool
	template<BasicStringType StringT = jpt::String>
	constexpr StringT ToString(bool value)
	{
		using TChar = StringT::TChar;

		StringT boolStr;
		if (value)
		{
			boolStr = JPT_GET_PROPER_STRING(TChar, true);
		}
		else
		{
			boolStr = JPT_GET_PROPER_STRING(TChar, false);
		}
		return boolStr;
	}

	// char
	template<BasicStringType StringT = jpt::String>
	constexpr StringT ToString(typename StringT::TChar c)
	{
		return StringT(c);
	}
}