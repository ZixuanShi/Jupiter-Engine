// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Core/Minimal/Macros.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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
	/**	@return How many characters inside of the given CString. Excluding '\0' */
	template<StringLiteral TChar>
	constexpr size_t FindCharsCount(const TChar* string)
	{
		if (!string)
		{
			return 0;
		}

		size_t count = 0;
		while (string[count] != '\0')
		{
			++count;
		}

		return count;
	}

#pragma region Converters

	/** Convert from IntegerType to a char pointer holding the integer's value as string literal
		@param value:        The IntegerType value to convert to char*
		@param base:         The base of the value. Default to decimal as 10. Could be binary, oct, hex.
		@return A char pointer pointing to the memory where we store the converted number's string literal */
	template<StringLiteral TChar = char, Integral TInt = int32>
	constexpr TChar* IntegerToCStr(TInt value, EIntBase base = EIntBase::Decimal)
	{
		// Prepare data
		bool isNegative = false;	// Whether this value is negative or not
		TChar* result = nullptr;	// Final result string to return
		size_t index = 0;			// The index I'm using for char array, will be used as length of this value and string as well
		const TInt numBase = static_cast<TInt>(base);

		// Process 0
		if (value == 0)
		{
			result = Allocator<TChar>::AllocateArray(2);
			result[0] = '0';
			result[1] = '\0';
			return result;
		}
		// Process negative if IntegerType can be signed
		if constexpr (std::is_signed_v<TInt>)
		{
			if (value < 0)
			{
				isNegative = true;
				value = -value;
				++index;
			}
		}

		// Get Value's literal length
		TInt valueCopy = value;
		while (valueCopy > 0)
		{
			valueCopy /= numBase;
			++index;
		}
		// Hex prefix "0x"
		if (numBase == static_cast<TInt>(0x10u))
		{
			index += 2;
		}

		// Allocate Result string, + 1 for the end terminater
		result = Allocator<TChar>::AllocateArray(index + 1);

		// Append string terminator at the end
		result[index] = '\0';
		--index;

		// Process each digit
		while (value > 0)
		{
			const TInt digit = value % numBase;

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
			value /= numBase;
		}

		// Hex
		if (base == EIntBase::Hex)
		{
			result[index--] = 'x';
			result[index] = '0';
		}

		// If value was negative, add '-' at the beginning
		if constexpr (std::is_signed_v<TInt>)
		{
			if (isNegative)
			{
				result[index] = '-';
				--index;
			}
		}

		// Return result
		return result;
	}

	/** @return Integral number converted from pBuffer
		@param pBuffer		Source buffer containing integer data 
		@param count		Desired count to parse from start of pBuffer
		@param base			Decimal, Hex, etc. If it's Hex, Ignore 0x prefix */
	template<StringLiteral TChar = char, Integral TInt = int32>
	constexpr TInt CStrToInteger(const TChar* pBuffer, size_t count, EIntBase base = EIntBase::Decimal)
	{
		TInt result = 0;
		bool isNegative = false;
		size_t start = 0;
		const TInt numBase = static_cast<TInt>(base);

		// Negative
		if constexpr (std::is_signed_v<TInt>)
		{
			if (pBuffer[0] == '-')
			{
				isNegative = true;
				start = 1;
			}
		}

		// Hex prefix
		if (base == EIntBase::Hex)
		{
			start = 2;
		}

		for (size_t i = start; i < count; ++i)
		{
			TChar c = pBuffer[i];

			if (c >= 'a' && c <= 'f')
			{
				c -= static_cast<TChar>(32);
			}

			JPT_ASSERT((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'), "Invalid character for converting to hex");

			TInt number = static_cast<TInt>(0);
			if (c > static_cast<TChar>('9'))
			{
				number = c + static_cast<TChar>(10) - static_cast<TChar>('A');
			}
			else
			{
				number = c - static_cast<TChar>('0');
			}

			if (base == EIntBase::Hex)
			{
				result <<= 4;
				result |= number;
			}
			else if (base == EIntBase::Decimal)
			{
				result *= numBase;
				result += number;
			}
			else
			{
				JPT_ASSERT(false, "Unsupported base for converting to integer");
			}
		}

		if constexpr (std::is_signed_v<TInt>)
		{
			if (isNegative)
			{
				result *= -1;
			}
		}

		return result;
	}
	template<StringLiteral TChar = char, Integral TInt = int32>
	constexpr TInt CStrToInteger(const TChar* pBuffer)
	{
		return CStrToInteger<TChar, TInt>(pBuffer, FindCharsCount(pBuffer));
	}

	template<StringLiteral TChar = char, Floating TFloat = float>
	constexpr TChar* FloatToCStr(TFloat value)
	{
		static constexpr size_t kMaxSize = 32;

		TChar* buffer = new TChar[kMaxSize];
		const TChar* format = JPT_GET_PROPER_STRING(TChar, %.3f);	// controls how many precision digits to keep

		if constexpr (AreSameType<TChar, char>)
		{
			snprintf(buffer, kMaxSize, format, value);
		}
		else if (AreSameType<TChar, wchar_t>)
		{
			swprintf(buffer, kMaxSize, format, value);
		}

		return buffer;
	}

	/** @note	Will ignore the 'f' is there's any */
	template<StringLiteral TChar = char, Floating TFloat = float>
	constexpr TFloat CStrToFloat(const TChar* pBuffer, size_t count)
	{
		// Parse two integral parts of the precision dot, then combine them

		TFloat integer = 0;		// Left of precision
		TFloat floatingNum = 0;	// Right of precision
		size_t precisionIndex = kInvalidValue<size_t>;
		bool isNegative = false;

		for (size_t i = 0; i < count; ++i)
		{
			const TChar c = pBuffer[i];

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
			const int32 number = c - static_cast<TChar>('0');

			if (precisionIndex == kInvalidValue<size_t>)
			{
				integer *= static_cast<TFloat>(10);
				integer += number;
			}
			else
			{
				floatingNum *= static_cast<TFloat>(10);
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
		TFloat result = integer + floatingNum;
		if (isNegative)
		{
			result *= static_cast<TFloat>(-1);
		}

		return result;
	}
	template<StringLiteral TChar = char, Floating TFloat = float>
	constexpr TFloat CStrToFloat(const TChar* pBuffer)
	{
		return CStrToFloat(pBuffer, FindCharsCount(pBuffer));
	}

	/** @return		Lowercase or Uppercase for the given character */
	template<StringLiteral TChar = char>
	constexpr TChar GetLower(TChar c)
	{
		if (c >= 'A' && c <= 'Z')
		{
			return c + 'a' - 'A';
		}

		return c;
	}

	template<StringLiteral TChar = char>
	constexpr TChar GetUpper(TChar c)
	{
		if (c >= 'a' && c <= 'z')
		{
			return c + 'A' - 'a';
		}

		return c;
	}

	/** @return Heap allocated Buffer with content of input string. Caller needs to take ownership of buffer. i.e. Memory Managing, deleting */
	wchar_t* ToWChars(const char* pCStr, size_t count)
	{
		++count;	// For null terminator

		wchar_t* pBuffer = new wchar_t[count];
		mbstowcs_s(nullptr, pBuffer, count, pCStr, count);

		return pBuffer;
	}
	wchar_t* ToWChars(const char* pCStr)
	{
		return ToWChars(pCStr, FindCharsCount(pCStr));
	}
	char* ToChars(const wchar_t* pWStr, size_t count)
	{
		++count;	// For null terminator

		char* pBuffer = new char[count];
		wcstombs_s(nullptr, pBuffer, count, pWStr, count);

		return pBuffer;
	}
	char* ToChars(const wchar_t* pWStr)
	{
		return ToChars(pWStr, FindCharsCount(pWStr));
	}

#pragma endregion

#pragma region Operations

	/**	Copies data from destination to source with the given size */
	template<StringLiteral TChar>
	constexpr void StrCpy(TChar* pDestination, size_t sizeInBytes, const TChar* pSource)
	{
		if constexpr (AreSameType<TChar, char>)
		{
			strcpy_s(pDestination, sizeInBytes, pSource);
		}
		else if (AreSameType<TChar, wchar_t>)
		{
			wcscpy_s(pDestination, sizeInBytes, pSource);
		}
	}

	/**	Copies data from destination to source with the given size and max count */
	template<StringLiteral TChar>
	constexpr void StrNCpy(TChar* pDestination, size_t sizeInBytes, const TChar* pSource, size_t maxCount)
	{
		if constexpr (AreSameType<TChar, char>)
		{
			strncpy_s(pDestination, sizeInBytes, pSource, maxCount);
		}
		else if (AreSameType<TChar, wchar_t>)
		{
			wcsncpy_s(pDestination, sizeInBytes, pSource, maxCount);
		}
	}

#pragma endregion

#pragma region Comparisons

	/** Compares two C-Style strings. 
		Optional one size parameter indicates how many characters should it compare from begin
		Optional two size parameters indicates the first and second string's size in order
		@return		
		- npos if two C-Style strings within the given size are identical. 
		- 0 maybe strings' sizes are not equal
		- 0 to size for the index of of the first different char found in two strings */
	template<StringLiteral TChar>
	constexpr size_t StrCmp(const TChar* pString1, const TChar* pString2, size_t string1Count, size_t string2Count)
	{
		if (string1Count != string2Count)
		{
			return 0;
		}

		for (size_t i = 0; i < string1Count; ++i)
		{
			if (pString1[i] != pString2[i])
			{
				return i;
			}
		}

		return npos;
	}
	template<StringLiteral TChar>
	constexpr size_t StrCmp(const TChar* pString1, const TChar* pString2, size_t count)
	{
		return StrCmp(pString1, pString2, count, count);
	}
	template<StringLiteral TChar>
	constexpr size_t StrCmp(const TChar* pString1, const TChar* pString2)
	{
		return StrCmp(pString1, pString2, FindCharsCount(pString1), FindCharsCount(pString2));
	}
	template<StringLiteral TChar>
	constexpr bool AreStringsSame(const TChar* pString1, const TChar* pString2, size_t string1Count, size_t string2Count)
	{
		return StrCmp(pString1, pString2, string1Count, string2Count) == npos;
	}
	template<StringLiteral TChar>
	constexpr bool AreStringsSame(const TChar* pString1, const TChar* pString2, size_t count)
	{
		return StrCmp(pString1, pString2, count) == npos;
	}
	template<StringLiteral TChar>
	constexpr bool AreStringsSame(const TChar* pString1, const TChar* pString2)
	{
		return StrCmp(pString1, pString2) == npos;
	}

#pragma endregion

#pragma region Validation

	constexpr bool IsAlpha(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	constexpr bool IsDigit(char c)
	{
		return (c >= '0' && c <= '9');
	}

	constexpr bool IsAlphaNumeric(char c)
	{
		return IsAlpha(c) || IsDigit(c);
	}

	constexpr bool IsSafeSpecialChar(char c)
	{
		return (c == '_' || c == ' ');
	}

	constexpr bool IsSensitiveSpecialChar(char c)
	{
		return (c == '-' || c == '.' || c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' || c == '*' || c == '(' || c == ')' || c == '+' || c == '=' || c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == ';' || c == '"' || c == '\'' || c == '<' || c == '>' || c == ',' || c == '.' || c == '?' || c == '/' || c == '\\' || c == '|' || c == '`' || c == '~');
	}

	constexpr bool IsEmpty(char c)
	{
		return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\0';
	}

	/** @return		true if input CString Has acceptable text data
		@param pString				The string to perform check on every char 
		@param treatSpecialAsValid	[Optional] false if treat special characters as failure of validness. default to true */
	constexpr bool IsValidDataCStr(const char* pString, bool treatSpecialAsValid = true)
	{
		// Check if the pointer is valid and not empty
		if (pString == nullptr)
		{
			return false;
		}

		// Check each char validness
		size_t i = 0;
		while (true)
		{
			const char c = pString[i];
			if (c == '\0')
			{
				break;
			}

			++i;

			if (treatSpecialAsValid && IsSensitiveSpecialChar(c))
			{
				continue;
			}

			if (!IsDigit(c) && !IsAlpha(c) && !IsSafeSpecialChar(c))
			{
				return false;
			}
		}

		return true;
	}

#pragma endregion
}