// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Core/Minimals/Macros.h"

#include <string.h>
#include <stdio.h>
#include <type_traits>

export module jpt.StringUtils;

import jpt.TypeDefs;
import jpt.Constants;
import jpt.Concepts;
import jpt.Utilities;
import jpt.Allocator;
import jpt.Math;

export namespace jpt
{
	/**	@return const CharT* string' string to check for size.'s size. Excluding '\0' */
	template<StringLiteral CharT>
	size_t GetCStrLength(const CharT* string)
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
	template<StringLiteral CharT = char, Integral IntT = int32>
	CharT* IntegerToCStr(IntT value, IntT base = 10)
	{
		// Prepare data
		bool isNegative = false;	// Whether this value is negative or not
		CharT* result = nullptr;	// Final result string to return
		size_t index = 0;			// The index I'm using for char array, will be used as length of this value and string as well

		// Process 0
		if (value == 0)
		{
			result = Allocator<CharT>::AllocateArray(2);
			result[0] = '0';
			result[1] = '\0';
			return result;
		}
		// Process negative if IntegerType can be signed
		if constexpr (std::is_signed_v<IntT>)
		{
			if (value < 0)
			{
				isNegative = true;
				value = -value;
				++index;
			}
		}

		// Get Value's literal length
		IntT valueCopy = value;
		while (valueCopy > 0)
		{
			valueCopy /= base;
			++index;
		}
		// Allocate Result string, + 1 for the end terminater
		result = Allocator<CharT>::AllocateArray(index + 1);

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
	template<StringLiteral CharT = char, Integral IntT = int32>
	IntT CStrToInteger(const CharT* pBuffer, size_t size = kInvalidValue<size_t>)
	{
		size = (size == kInvalidValue<size_t>) ? GetCStrLength(pBuffer) : size;
		IntT result = 0;
		bool isNegative = false;

		for (size_t i = 0; i < size; ++i)
		{
			const CharT c = pBuffer[i];

			// Negative
			if (c == '-')
			{
				isNegative = true;
				continue;
			}

			// Parse number
			JPT_ASSERT(c >= '0' && c <= '9', "Invalid character for converting to number");
			const int32 number = c - static_cast<CharT>('0');
			result *= 10;
			result += number;
		}

		if (isNegative)
		{
			result *= -1;
		}

		return result;
	}

	template<StringLiteral CharT = char, Floating FloatT = float>
	CharT* FloatToCStr(FloatT value)
	{
		CharT* buffer = new CharT[32];
		const CharT* format = JPT_GET_PROPER_STRING(CharT, %.3f);

		if constexpr (IsSameType<CharT, char>)
		{
			snprintf(buffer, 32, format, value);
		}
		else if (IsSameType<CharT, wchar_t>)
		{
			swprintf(buffer, 32, format, value);
		}

		return buffer;
	}

	/** @note	Will ignore the 'f' is there's any */
	template<StringLiteral CharT = char, Floating FloatT = float>
	FloatT CStrToFloat(const CharT* pBuffer, size_t size = kInvalidValue<size_t>)
	{
		// Parse two integral parts of the precision dot, then combine them

		size = (size == kInvalidValue<size_t>) ? GetCStrLength(pBuffer) : size;
		FloatT integer = 0;		// Left of precision
		FloatT floatingNum = 0;	// Right of precision
		size_t precisionIndex = kInvalidValue<size_t>;
		bool isNegative = false;

		for (size_t i = 0; i < size; ++i)
		{
			const CharT c = pBuffer[i];

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
			const int32 number = c - static_cast<CharT>('0');

			if (precisionIndex == kInvalidValue<size_t>)
			{
				integer *= static_cast<FloatT>(10);
				integer += number;
			}
			else
			{
				floatingNum *= static_cast<FloatT>(10);
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
		FloatT result = integer + floatingNum;
		if (isNegative)
		{
			result *= static_cast<FloatT>(-1);
		}

		return result;
	}

	/** constexpr compile time hash functions, 32 and 64 bit
		@str: should be a null terminated string literal */
	constexpr uint32 StringHash32(const char* const str, const uint32 value = 0x811c9dc5)         noexcept { return (str[0] == '\0') ? value : StringHash32(&str[1], (value ^ uint32(str[0])) * 0x1000193); }
	constexpr uint64 StringHash64(const char* const str, const uint64 value = 0xcbf29ce484222325) noexcept { return (str[0] == '\0') ? value : StringHash64(&str[1], (value ^ uint64(str[0])) * 0x100000001b3); }

	/**	Copies data from destination to source with the given size */
	template<StringLiteral CharT>
	void StrCpy(CharT* pDestination, size_t sizeInBytes, const CharT* pSource)
	{
		if constexpr (IsSameType<CharT, char>)
		{
			strcpy_s(pDestination, sizeInBytes, pSource);
		}
		else if (IsSameType<CharT, wchar_t>)
		{
			wcscpy_s(pDestination, sizeInBytes, pSource);
		}
	}

	/**	Copies data from destination to source with the given size and max count */
	template<StringLiteral CharT>
	void StrNCpy(CharT* pDestination, size_t sizeInBytes, const CharT* pSource, size_t maxCount)
	{
		if constexpr (IsSameType<CharT, char>)
		{
			strncpy_s(pDestination, sizeInBytes, pSource, maxCount);
		}
		else if (IsSameType<CharT, wchar_t>)
		{
			wcsncpy_s(pDestination, sizeInBytes, pSource, maxCount);
		}
	}

	/* @return		
		- npos if two C-Style strings within the given size are identical. 
		- 0 maybe strings' sizes are not equal
		- 0 to size for the index of of the first different char found in two strings */
	template<StringLiteral CharT>
	size_t StrCmp(const CharT* pString1, const CharT* pString2, size_t string1Size, size_t string2Size)
	{
		if (string1Size != string2Size)
		{
			return 0;
		}

		for (size_t i = 0; i < string1Size; ++i)
		{
			if (pString1[i] != pString2[i])
			{
				return i;
			}
		}

		return npos;
	}
	template<StringLiteral CharT>
	size_t StrCmp(const CharT* pString1, const CharT* pString2, size_t size)
	{
		return StrCmp(pString1, pString2, size, size);
	}
	template<StringLiteral CharT>
	size_t StrCmp(const CharT* pString1, const CharT* pString2)
	{
		return StrCmp(pString1, pString2, GetCStrLength(pString1), GetCStrLength(pString2));
	}
	template<StringLiteral CharT>
	bool AreStringsSame(const CharT* pString1, const CharT* pString2, size_t string1Size, size_t string2Size)
	{
		return StrCmp(pString1, pString2, string1Size, string2Size) == npos;
	}
	template<StringLiteral CharT>
	bool AreStringsSame(const CharT* pString1, const CharT* pString2, size_t size)
	{
		return StrCmp(pString1, pString2, size) == npos;
	}
	template<StringLiteral CharT>
	bool AreStringsSame(const CharT* pString1, const CharT* pString2)
	{
		return StrCmp(pString1, pString2) == npos;
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