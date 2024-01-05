// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Debugging/Assert.h"

#include <string.h>
#include <stdio.h>
#include <type_traits>

export module jpt.StringUtils;

import jpt.TypeDefs;
import jpt.Concepts;
import jpt.Utilities;

export namespace jpt
{
	/** @return How many characters inside the input const char* string except the null terminator */
	size_t strlen(const char* string)
	{
		if (!string)
		{
			return 0;
		}

		size_t size = 0;
		while (string[size] != '\0')
		{
			++size;
		}

		return size;
	}

	/** @return How many characters inside the input const wide char* string except the null terminator */
	size_t wcslen(const wchar_t* string)
	{
		if (!string)
		{
			return 0;
		}

		size_t size = 0;
		while (string[size] != L'\0')
		{
			++size;
		}

		return size;
	}

	/** Convert from IntegerType to a char pointer holding the integer's value as string literal
		@param value:        The IntegerType value to convert to char*
		@param base:         The base of the value. Default to decimal as 10. Could be binary, oct, hex.
		@return A char pointer pointing to the memory where we store the converted number's string literal */
	template<Integral Type>
	char* IntegerToCStr(Type value, int32 base = 10)
	{
		// Prepare data
		bool isNegative = false;	// Whether this value is negative or not
		char* result = nullptr;		// Final result string to return
		size_t index = 0;			// The index I'm using for char array, will be used as length of this value and string as well

		// Process 0
		if (value == 0)
		{
			result = new char[2];
			result[0] = '0';
			result[1] = '\0';
			return result;
		}
		// Process negative if IntegerType can be signed
		if constexpr (std::is_signed_v<Type>)
		{
			if (value < 0)
			{
				isNegative = true;
				value = -value;
				++index;
			}
		}

		// Get Value's literal length
		Type valueCopy = value;
		while (valueCopy > 0)
		{
			valueCopy /= base;
			++index;
		}
		// Allocate Result string, + 1 for the end terminater
		result = new char[index + 1];

		// Append string terminator at the end
		result[index] = '\0';
		--index;

		// Process each digit
		while (value > 0)
		{
			const int32 digit = value % base;

			// For different base, process differently
			if (digit > 9)
			{
				result[index] = static_cast<char>(digit - 10) + 'A';
			}
			else
			{
				result[index] = static_cast<char>(digit + '0');
			}

			// Update index and cut last digit of value
			--index;
			value /= base;
		}

		// If value was negative, add '-' at the beginning
		if (isNegative)
		{
			result[index] = '-';
			--index;
		}

		// Return result
		return result;
	}

	template<Floating Type>
	char* FloatingToCStr(Type value)
	{
		char* buffer = new char[32];
		char format[5] = "%.3f";
		snprintf(buffer, 32, format, value);
		return buffer;
	}

	/** @return 0 if two strings are identical. kInvalidValue<int32> if not */
	int32 strncmp(const char* string1, const char* string2, size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (string1[i] != string2[i])
			{
				return kInvalidValue<int32>;
			}
		}
		return 0;
	}

	/** @return 0 if two wide strings are identical. kInvalidValue<int32> if not */
	int32 wcsncmp(const wchar_t* string1, const wchar_t* string2, size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (string1[i] != string2[i])
			{
				return kInvalidValue<int32>;
			}
		}
		return 0;
	}

	/** constexpr compile time hash functions, 32 and 64 bit
		@str: should be a null terminated string literal */
	constexpr uint32 StringHash32(const char* const str, const uint32 value = 0x811c9dc5)         noexcept { return (str[0] == '\0') ? value : StringHash32(&str[1], (value ^ uint32(str[0])) * 0x1000193); }
	constexpr uint64 StringHash64(const char* const str, const uint64 value = 0xcbf29ce484222325) noexcept { return (str[0] == '\0') ? value : StringHash64(&str[1], (value ^ uint64(str[0])) * 0x100000001b3); }

	/**	@return const CharType* string' string to check for size.'s size */
	template<StringLiteral CharType>
	size_t GetStrLength(const CharType* string)
	{
		if constexpr (jpt::IsSameType<CharType, char>::Value)
		{
			return jpt::strlen(string);
		}
		else if (jpt::IsSameType<CharType, wchar_t>::Value)
		{
			return jpt::wcslen(string);
		}
	}

	/**	Copies data from destination to source with the given size */
	template<StringLiteral CharType>
	void StrCpy(CharType* pDestination, size_t sizeInBytes, const CharType* pSource)
	{
		if constexpr (jpt::IsSameType<CharType, char>::Value)
		{
			strcpy_s(pDestination, sizeInBytes, pSource);
		}
		else if (jpt::IsSameType<CharType, wchar_t>::Value)
		{
			wcscpy_s(pDestination, sizeInBytes, pSource);
		}
	}

	/**	Copies data from destination to source with the given size and max count */
	template<StringLiteral CharType>
	void StrNCpy(CharType* pDestination, size_t sizeInBytes, const CharType* pSource, size_t maxCount)
	{
		if constexpr (jpt::IsSameType<CharType, char>::Value)
		{
			strncpy_s(pDestination, sizeInBytes, pSource, maxCount);
		}
		else if (jpt::IsSameType<CharType, wchar_t>::Value)
		{
			wcsncpy_s(pDestination, sizeInBytes, pSource, maxCount);
		}
	}

	/* @return		true if two C-Style strings within the given size are identical. false if not */
	template<StringLiteral CharType>
	bool StrNCmp(const CharType* pString1, const CharType* pString2, size_t size)
	{
		if (GetStrLength(pString1) != GetStrLength(pString2))
		{
			return false;
		}

		if constexpr (jpt::IsSameType<CharType, char>::Value)
		{
			return jpt::strncmp(pString1, pString2, size) == 0;
		}
		else if (jpt::IsSameType<CharType, wchar_t>::Value)
		{
			return jpt::wcsncmp(pString1, pString2, size) == 0;
		}
	}
	
	template<typename StringType>
	bool StringCmp(const StringType& string1, const StringType& string2, size_t size)
	{
		if (string1.size() != string2.size())
		{
			return false;
		}

		if constexpr (jpt::IsSameType<StringType::CharType, char>::Value)
		{
			return jpt::strncmp(string1.ConstBuffer(), string2.ConstBuffer(), size) == 0;
		}
		else if (jpt::IsSameType<StringType::CharType, wchar_t>::Value)
		{
			return jpt::wcsncmp(string1.ConstBuffer(), string2.ConstBuffer(), size) == 0;
		}
	}

	template<>
	struct hash<const char*>
	{
		size_t operator()(const char* key)
		{
			return jpt::StringHash64(key);
		}
	};
}