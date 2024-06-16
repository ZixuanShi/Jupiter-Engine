// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

export module jpt.ToString;

import jpt.String;
import jpt.StringUtils;
import jpt.Concepts;
import jpt.TypeTraits;
import jpt.TypeDefs;
import jpt.Utilities;

export namespace jpt
{
	template<typename T>
	concept EnabledToString = requires(T object) { object.ToString(); };

	template<typename T>
	concept EnabledToWString = requires(T object) { object.ToWString(); };

	// Add any additional primitive types if implemented later
	template<typename T>
	concept NoBuiltInToStringPrimitive = Integral<T> || Floating<T> || AreSameType<T, bool> || StringLiteral<T>;

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
	template<StringType TString = jpt::String, Integral TInt = int32>
	constexpr TString ToString(TInt integer)
	{
		using TChar = TString::TChar;

		TChar* integerCString = IntegerToCStr<TChar>(integer);
		TString integerString;
		integerString.MoveString(integerCString);
		return integerString;
	}

	// float, double
	template<StringType TString = jpt::String, Floating TFloat = float32>
	constexpr TString ToString(TFloat value)
	{
		using TChar = TString::TChar;

		TChar* floatCString = FloatToCStr<TChar>(value);
		TString floatString;
		floatString.MoveString(floatCString);
		return floatString;
	}

	// bool
	template<StringType TString = jpt::String>
	constexpr TString ToString(bool value)
	{
		using TChar = TString::TChar;

		TString boolStr;
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
	template<StringType TString = jpt::String>
	constexpr TString ToString(typename TString::TChar c)
	{
		return TString(c);
	}

	// Containers
	template<Containable TContainer>
	constexpr String ToString(const TContainer& container)
	{
		if constexpr (AreSameType<TContainer, String>)
		{
			return container;
		}
		else
		{
			String str("{ ", 2);

			for (auto itr = container.begin(); itr != container.end(); ++itr)
			{
				// Elements need to provide ToString() implementation to make this work
				str += jpt::ToString(*itr);

				// Append ", " suffix if it's not the last element
				const bool isLastElement = ((itr + 1) == container.end());
				if (!isLastElement)
				{
					str.Append(", ", 2);
				}
			}

			str.Append(" }", 2);

			return str;
		}
	}
	template<Containable TContainer>
	constexpr WString ToWString(const TContainer& container)
	{
		if constexpr (AreSameType<TContainer, WString>)
		{
			return container;
		}
		else
		{
			WString wstr(L"{ ", 2);

			for (auto itr = container.begin(); itr != container.end(); ++itr)
			{
				// Elements need to provide ToWString() implementation to make this work
				wstr += jpt::ToString<jpt::WString>(*itr);

				// Append ", " suffix if it's not the last element
				const bool isLastElement = ((itr + 1) == container.end());
				if (!isLastElement)
				{
					wstr.Append(L", ", 2);
				}
			}

			wstr.Append(L" }", 2);

			return wstr;
		}
	}

	// String to WString
	WString ToWString(const String& str)
	{
		if (str.Count() == 0)
		{
			return WString();
		}

		wchar_t* pBuffer = ToWChars(str.ConstBuffer(), str.Count());

		WString wstr;
		wstr.MoveString(pBuffer, str.Count());
		return wstr;
	}
	WString ToWString(const char* pCStr, size_t count = npos)
	{
		if (count == npos)
		{
			count = FindCharsCount(pCStr);
		}

		if (count == 0)
		{
			return WString();
		}

		String str(pCStr, count);
		return ToWString(str);
	}

	// WString to String
	String ToString(const WString& wstr)
	{
		if (wstr.Count() == 0)
		{
			return String();
		}

		char* pBuffer = ToChars(wstr.ConstBuffer(), wstr.Count());

		String str;
		str.MoveString(pBuffer, wstr.Count());
		return str;
	}
	String ToString(const wchar_t* pWStr, size_t count = npos)
	{
		if (count == npos)
		{
			count = FindCharsCount(pWStr);
		}
		
		if (count == 0)
		{
			return String();
		}

		WString wstr(pWStr, count);
		return ToString(wstr);
	}
}