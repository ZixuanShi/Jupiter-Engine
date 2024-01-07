// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

#include <string.h>
#include <stdio.h>
#include <type_traits>

export module jpt.StringUtils;

import jpt.TypeDefs;
import jpt.Concepts;
import jpt.Utilities;
import jpt.Allocator;
import jpt.Math;

export namespace jpt
{
	/**	@return const CharType* string' string to check for size.'s size */
	template<StringLiteral CharType>
	size_t GetCStrLength(const CharType* string)
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

	/** Convert from IntegerType to a char pointer holding the integer's value as string literal
		@param value:        The IntegerType value to convert to char*
		@param base:         The base of the value. Default to decimal as 10. Could be binary, oct, hex.
		@return A char pointer pointing to the memory where we store the converted number's string literal */
	template<StringLiteral CharType = char, Integral IntegerType = int32>
	CharType* IntegerToCStr(IntegerType value, int32 base = 10)
	{
		// Prepare data
		bool isNegative = false;	// Whether this value is negative or not
		CharType* result = nullptr;	// Final result string to return
		size_t index = 0;			// The index I'm using for char array, will be used as length of this value and string as well

		// Process 0
		if (value == 0)
		{
			result = Allocator<CharType>::AllocateArray(2);
			result[0] = '0';
			result[1] = '\0';
			return result;
		}
		// Process negative if IntegerType can be signed
		if constexpr (std::is_signed_v<IntegerType>)
		{
			if (value < 0)
			{
				isNegative = true;
				value = -value;
				++index;
			}
		}

		// Get Value's literal length
		IntegerType valueCopy = value;
		while (valueCopy > 0)
		{
			valueCopy /= base;
			++index;
		}
		// Allocate Result string, + 1 for the end terminater
		result = Allocator<CharType>::AllocateArray(index + 1);

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

	/** @return Integral number converted from pBuffer */
	template<StringLiteral CharType = char, Integral NumType = int32>
	NumType CStrToInteger(const CharType* pBuffer, size_t size = kInvalidValue<size_t>)
	{
		size = (size == kInvalidValue<size_t>) ? GetCStrLength(pBuffer) : size;
		NumType result = 0;
		bool isNegative = false;

		for (size_t i = 0; i < size; ++i)
		{
			const CharType c = pBuffer[i];

			// Negative
			if (c == '-')
			{
				isNegative = true;
				continue;
			}

			// Parse number
			JPT_ASSERT(c >= '0' && c <= '9', "Invalid character for converting to number");
			const int32 number = c - static_cast<CharType>('0');
			result *= 10;
			result += number;
		}

		if (isNegative)
		{
			result *= -1;
		}

		return result;
	}

	template<StringLiteral CharType = char, Floating Type = float>
	CharType* FloatingToCStr(Type value)
	{
		CharType* buffer = new CharType[32];
		const CharType* format = JPT_GET_PROPER_STRING(CharType, %.3f);

		if constexpr (IsSameType<CharType, char>::Value)
		{
			snprintf(buffer, 32, format, value);
		}
		else if (IsSameType<CharType, wchar_t>::Value)
		{
			swprintf(buffer, 32, format, value);
		}

		return buffer;
	}

	/** @note	Will ignore the 'f' is there's any */
	template<StringLiteral CharType = char, Floating NumType = float>
	NumType CStrToFloat(const CharType* pBuffer, size_t size = kInvalidValue<size_t>)
	{
		// Parse two integral parts of the precision dot, then combine them

		size = (size == kInvalidValue<size_t>) ? GetCStrLength(pBuffer) : size;
		NumType integer = 0;		// Left of precision
		NumType floatingNum = 0;	// Right of precision
		size_t precisionIndex = kInvalidValue<size_t>;
		bool isNegative = false;

		for (size_t i = 0; i < size; ++i)
		{
			const CharType c = pBuffer[i];

			// Negative
			if (c == '-')
			{
				isNegative = true;
				continue;
			}

			// Precision dot
			if (c == '.')
			{
				precisionIndex = i;
				continue;
			}

			// Num
			JPT_ASSERT(c >= '0' && c <= '9', "Invalid character for converting to number");
			const int32 number = c - static_cast<CharType>('0');

			if (precisionIndex == kInvalidValue<size_t>)
			{
				integer *= static_cast<NumType>(10);
				integer += number;
			}
			else
			{
				floatingNum *= static_cast<NumType>(10);
				floatingNum += number;
			}
		}

		// Make floating number part eligible
		uint32 floatCopy = static_cast<int32>(floatingNum);
		uint32 divider = 1;
		while (floatCopy > 0)
		{
			floatCopy /= 10;
			divider *= 10;
		}
		floatingNum /= divider;

		// Combine two parts and process negative if it is
		NumType result = integer + floatingNum;
		if (isNegative)
		{
			result *= static_cast<NumType>(-1);
		}

		return result;
	}

	/** constexpr compile time hash functions, 32 and 64 bit
		@str: should be a null terminated string literal */
	constexpr uint32 StringHash32(const char* const str, const uint32 value = 0x811c9dc5)         noexcept { return (str[0] == '\0') ? value : StringHash32(&str[1], (value ^ uint32(str[0])) * 0x1000193); }
	constexpr uint64 StringHash64(const char* const str, const uint64 value = 0xcbf29ce484222325) noexcept { return (str[0] == '\0') ? value : StringHash64(&str[1], (value ^ uint64(str[0])) * 0x100000001b3); }

	/**	Copies data from destination to source with the given size */
	template<StringLiteral CharType>
	void StrCpy(CharType* pDestination, size_t sizeInBytes, const CharType* pSource)
	{
		if constexpr (IsSameType<CharType, char>::Value)
		{
			strcpy_s(pDestination, sizeInBytes, pSource);
		}
		else if (IsSameType<CharType, wchar_t>::Value)
		{
			wcscpy_s(pDestination, sizeInBytes, pSource);
		}
	}

	/**	Copies data from destination to source with the given size and max count */
	template<StringLiteral CharType>
	void StrNCpy(CharType* pDestination, size_t sizeInBytes, const CharType* pSource, size_t maxCount)
	{
		if constexpr (IsSameType<CharType, char>::Value)
		{
			strncpy_s(pDestination, sizeInBytes, pSource, maxCount);
		}
		else if (IsSameType<CharType, wchar_t>::Value)
		{
			wcsncpy_s(pDestination, sizeInBytes, pSource, maxCount);
		}
	}

	/* @return		true if two C-Style strings within the given size are identical. false if not */
	template<StringLiteral CharType>
	bool AreStringsSame(const CharType* pString1, const CharType* pString2, size_t size = kInvalidValue<size_t>)
	{
		const size_t string1Length = GetCStrLength(pString1);
		const size_t string2Length = GetCStrLength(pString2);

		if (string1Length != string2Length)
		{
			return false;
		}

		size = (size == kInvalidValue<size_t>) ? Min(string1Length, string2Length) : size;

		for (size_t i = 0; i < size; ++i)
		{
			if (pString1[i] != pString2[i])
			{
				return false;
			}
		}

		return true;
	}

	template<typename StringType>
	bool AreStringsSame(const StringType& string1, const StringType& string2, size_t size = kInvalidValue<size_t>)
	{
		if (string1.Size() != string2.Size())
		{
			return false;
		}

		size = (size == kInvalidValue<size_t>) ? Min(string1.Size(), string2.Size()) : size;

		for (size_t i = 0; i < size; ++i)
		{
			if (string1[i] != string2[i])
			{
				return false;
			}
		}

		return true;
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