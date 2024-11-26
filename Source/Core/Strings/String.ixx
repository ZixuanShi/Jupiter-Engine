// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Strings/StringMacros.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"

#include <stdio.h>
#include <vadefs.h>
#include <stdarg.h>
#include <fstream>


export module jpt.String;

import jpt.Allocator;
import jpt.Concepts;
import jpt.Constants;
import jpt.DynamicArray;
import jpt.Hash;
import jpt.Math;
import jpt.String.Helpers;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Serializer;

export namespace jpt
{
	template<StringLiteral _TChar, class _TAllocator = Allocator<_TChar>>
	class String_Base
	{
	public:
		using TChar         = _TChar;
		using TData         = _TChar;
		using TAllocator    = _TAllocator;
		using Iterator      = TChar*;
		using ConstIterator = const TChar*;

	private:
		TChar m_smallBuffer[kSmallDataSize] = { 0 };	/**< Small buffer to store small data */
		TChar* m_pBuffer = nullptr;  /**< The pointer to the buffer representing this string's value */
		size_t m_count = 0;          /**< How many characters in this string currently */
		size_t m_capacity = 0;       /**< How many characters this string can hold before resizing. Excluded the null terminator */

	public:
		constexpr String_Base() = default;
		constexpr String_Base(const TChar* CString, size_t size);
		constexpr String_Base(const TChar* CString);
		constexpr String_Base(TChar c);
		constexpr String_Base(const String_Base<TChar>& otherString);
		constexpr String_Base(String_Base<TChar>&& otherString) noexcept;

		String_Base& operator=(const TChar* CString);
		String_Base& operator=(const String_Base<TChar>& otherString);
		String_Base& operator=(String_Base<TChar>&& otherString) noexcept;
		constexpr ~String_Base();

		// Element Access
		constexpr const TChar* ConstBuffer() const { return m_pBuffer; }
		constexpr       TChar* Buffer()      const { return m_pBuffer; }
		constexpr       TChar& Front()             { return m_pBuffer[0]; }
		constexpr const TChar& Front()       const { return m_pBuffer[0]; }
		constexpr       TChar& Back()              { return m_pBuffer[m_count - 1]; }
		constexpr const TChar& Back()        const { return m_pBuffer[m_count - 1]; }
		constexpr       TChar& operator[](size_t index)       { return m_pBuffer[index]; }
		constexpr const TChar& operator[](size_t index) const { return m_pBuffer[index]; }

		// Iterators
		constexpr Iterator begin() { return Iterator(m_pBuffer); }
		constexpr Iterator end()   { return Iterator(m_pBuffer + m_count); }
		constexpr ConstIterator begin()  const { return ConstIterator(m_pBuffer); }
		constexpr ConstIterator cbegin() const { return ConstIterator(m_pBuffer); }
		constexpr ConstIterator end()    const { return ConstIterator(m_pBuffer + m_count); }
		constexpr ConstIterator cend()   const { return ConstIterator(m_pBuffer + m_count); }

		// Capacity
		constexpr size_t Size()         const; /**< How many size in bytes this string in memory */
		constexpr size_t Capacity()     const;
		constexpr bool   IsEmpty()      const;
		constexpr size_t Count()        const; /**< How many characters in this string */
		constexpr size_t Count(TChar c, size_t startIndex = 0, size_t endIndex = npos) const; /**< How many times the character c appears in this string */
		constexpr size_t Count(const TChar* pString, size_t startIndex = 0, size_t endIndex = npos) const;

		/** Searching. Returns npos if not found */
		constexpr size_t Find(      TChar  charToFind,    size_t startIndex = 0, size_t endIndex = npos) const;
		constexpr size_t Find(const TChar* pStringToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		constexpr size_t FindFirstOf(      TChar charToFind,     size_t startIndex = 0, size_t endIndex = npos) const;
		constexpr size_t FindFirstOf(const TChar* pStringToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		constexpr size_t FindLastOf(      TChar charToFind,     size_t startIndex = 0, size_t endIndex = npos)  const;
		constexpr size_t FindLastOf(const TChar* pStringToFind, size_t startIndex = 0, size_t endIndex = npos)  const;
		constexpr bool   Has(      TChar  charToFind,    size_t startIndex = 0, size_t endIndex = npos) const { return Find(charToFind, startIndex, endIndex)    != npos; }
		constexpr bool   Has(const TChar* pStringToFind, size_t startIndex = 0, size_t endIndex = npos) const { return Find(pStringToFind, startIndex, endIndex) != npos; }
		constexpr bool BeginsWith(const TChar* pStringToFind) const;
		constexpr bool EndsWith(const TChar* pStringToFind) const;

		/* Deallocate the memory that this string holds */
		constexpr void Clear();

		/* Replace the some content of this string with the new given one within optional range
			@param StringToFind:	The string we want to remove and replace
			@param StringToReplace:	The string to replace the original data
			@param startIndex:		[optional] The start index to start searching. Default to 0
			@param endIndex:		[optional] The end index to stop operation. Default to Count() */
		constexpr String_Base& Replace(const TChar* pStringToFind, const TChar* pStringToReplace, size_t startIndex = 0, size_t endIndex = npos);

		/** Splits this string to substrings by a keyword */
		constexpr DynamicArray<String_Base> Split(const TChar* pKeyword) const;
		constexpr DynamicArray<String_Base> Split(TChar keyword) const;

		/** @return		A sub string within the given range at index and length */
		constexpr String_Base SubStr(size_t index, size_t count = npos) const;
		constexpr void PopBack(size_t count = 1);

		/** Trim string from the left or right at given index
			@param index:	[optional] The index to trim to. Default to npos if just trim out white spaces */
		constexpr void TrimLeft(size_t index = npos);
		constexpr void TrimRight(size_t index = npos);

		/** Insert a string at the index
			@param CString:		Content of new string to insert
			@param index:		Where to insert in this string
			@param size:		[optional] Length of the string to insert */
		constexpr void Insert(TChar c, size_t index);
		constexpr void Insert(const TChar* CString, size_t index);
		constexpr void Insert(const TChar* CString, size_t index, size_t size);

		/** Appends a new string to the end of buffer */
		constexpr void Append(const TChar* CString, size_t newStringSize);
		constexpr void Append(const TChar* CString);
		constexpr void Append(const String_Base<TChar>& otherString);
		constexpr void Append(const DynamicArray<String_Base<TChar>>& strings, const TChar* separator = nullptr);
		constexpr void Append(TChar c);
		constexpr String_Base& operator+=(const TChar* CString);
		constexpr String_Base& operator+=(const String_Base<TChar>& otherString);
		constexpr String_Base& operator+=(TChar c);

		/** Pre allocate buffer with capacity's size. Preventing oftenly dynamic heap allocation */
		constexpr void Reserve(size_t capacity);
		constexpr void Resize(size_t newSize);

		/** Formats data to a string with provided format then return it
			@example String::Format<32>("%d/%d/%d. %d:%d:%d", month, day, year, hour, minute, second); */
		template<size_t kCount>
		static constexpr String_Base Format(const TChar* format, ...);

		/* Copy the content of string. Will assign the current m_pBuffer with the new copied data in memory */
		constexpr void CopyString(const TChar* inCString, size_t size);
		constexpr void CopyString(const TChar* inCString);
		constexpr void CopyString(const String_Base<TChar>& otherString);

		/* Move the content of string. Will take ownership of the passed in string */
		constexpr void MoveString(TChar* inCString, size_t size);
		constexpr void MoveString(TChar* inCString);
		constexpr void MoveString(String_Base<TChar>&& otherString);

		/** @return A hash value of this string */
		constexpr uint64 Hash() const;

		void Serialize(Serializer& serializer) const;
		void Deserialize(Serializer& serializer);

	private:
		constexpr void DeallocateBuffer();

		/** Implementation of appending a C-String at the index by the given size */
		constexpr void AppendImpl(const TChar* CString, size_t size);
	};

	// Non member functions -------------------------------------------------------------------------------------------------------------------
	
	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator> operator+(const String_Base<TChar, TAllocator>& string, const TChar* CString)
	{
		String_Base<TChar> str = string;
		str.Append(CString);
		return str;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator> operator+(String_Base<TChar, TAllocator>&& string, const TChar* CString)
	{
		String_Base<TChar> str = Move(string);
		str.Append(CString);
		return str;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator> operator+(const String_Base<TChar, TAllocator>& string, const String_Base<TChar>& otherString)
	{
		String_Base<TChar> str = string;
		str.Append(otherString);
		return str;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator> operator+(String_Base<TChar, TAllocator>&& string, const String_Base<TChar>& otherString)
	{
		String_Base<TChar> str = Move(string);
		str.Append(otherString);
		return str;
	}

	template<StringLiteral TChar>
	constexpr String_Base<TChar> operator+(const TChar* CString, const String_Base<TChar>& string)
	{
		return jpt::String_Base<TChar>(CString) += string;
	}

	template<StringLiteral TChar>
	constexpr String_Base<TChar> operator+(const TChar* CString, String_Base<TChar>&& rightString)
	{
		return jpt::String_Base<TChar>(CString) += Move(rightString);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr bool operator==(const String_Base<TChar, TAllocator>& string, const TChar* CString)
	{
		return AreStringsSame(string.ConstBuffer(), CString, string.Count(), FindCharsCount(CString));
	}

	/** Comparing a String class with raw CStr but different char type */
	template<StringLiteral TChar1, class TAllocator, StringLiteral TChar2 = TChar1>
	constexpr bool operator==(const String_Base<TChar1, TAllocator>& string, const TChar2* CString)
	{
		const size_t cStrCount = FindCharsCount(CString);
		bool result = false;

		// String to const wchar_t*
		if constexpr (AreSameType<TChar1, char> && AreSameType<TChar2, wchar_t>)
		{
			const char* pCStr = ToChars(CString, cStrCount);
			result = AreStringsSame(string.ConstBuffer(), pCStr, string.Count(), cStrCount);
			JPT_DELETE_ARRAY(pCStr);
		}
		// WString to const char*
		else if constexpr (AreSameType<TChar1, wchar_t> && AreSameType<TChar2, char>)
		{
			const wchar_t* pWCStr = ToWChars(CString, cStrCount);
			result = AreStringsSame(string.ConstBuffer(), pWCStr, string.Count(), cStrCount);
			JPT_DELETE_ARRAY(pWCStr);
		}
		else
		{
			JPT_ASSERT(false, "Unsupported type");
		}

		return result;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr bool operator==(const String_Base<TChar, TAllocator>& lhs, const String_Base<TChar>& rhs)
	{
		return AreStringsSame(lhs.ConstBuffer(), rhs.ConstBuffer(), lhs.Count(), rhs.Count());
	}

	// Member Functions Definitions ---------------------------------------------------------------------------------------
	
	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator>::String_Base(const TChar* CString, size_t size)
	{
		CopyString(CString, size);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator>::String_Base(const TChar* CString)
	{
		CopyString(CString);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator>::String_Base(TChar c)
	{
		TChar* pBuffer = TAllocator::AllocateArray(2);
		pBuffer[0] = c;
		pBuffer[1] = '\0';
		MoveString(pBuffer, 1);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator>::String_Base(const String_Base<TChar>& otherString)
	{
		CopyString(otherString);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator>::String_Base(String_Base<TChar>&& otherString) noexcept
	{
		MoveString(Move(otherString));
	}

	template<StringLiteral TChar, class TAllocator>
	String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::operator=(const TChar* CString)
	{
		CopyString(CString);
		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::operator=(const String_Base<TChar>& otherString)
	{
		if (this != &otherString)
		{
			CopyString(otherString);
		}

		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::operator=(String_Base<TChar>&& otherString) noexcept
	{
		if (this != &otherString)
		{
			MoveString(Move(otherString));
		}

		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator>::~String_Base()
	{
		Clear();
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr size_t String_Base<_TChar, _TAllocator>::Count() const
	{
		return m_count;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr size_t String_Base<_TChar, _TAllocator>::Count(TChar c, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/) const
	{
		ClampTo(endIndex, size_t(0), m_count);
		size_t count = 0;

		for (size_t i = startIndex; i < endIndex; ++i)
		{
			const TChar ch = m_pBuffer[i];
			if (ch == c)
			{
				++count;
			}
		}

		return count;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr size_t String_Base<_TChar, _TAllocator>::Count(const TChar* pString, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/) const
	{
		ClampTo(endIndex, size_t(0), m_count);
		size_t count = 0;
		const size_t stringToFindSize = FindCharsCount(pString);

		for (size_t i = startIndex; i < endIndex;)
		{
			if ((i + stringToFindSize) > endIndex)
			{
				break;
			}

			if (AreStringsSame(m_pBuffer + i, pString, stringToFindSize))
			{
				++count;
				i += stringToFindSize;
			}
			else
			{
				++i;
			}
		}

		return count;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr size_t String_Base<_TChar, _TAllocator>::Size() const
	{
		return m_count * sizeof(TChar);
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr size_t String_Base<_TChar, _TAllocator>::Capacity() const
	{
		return m_capacity;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr bool String_Base<_TChar, _TAllocator>::IsEmpty() const
	{
		return m_count == 0;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t String_Base<TChar, TAllocator>::Find(TChar charToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/) const
	{
		ClampTo(endIndex, size_t(0), m_count);

		for (size_t i = startIndex; i < endIndex; ++i)
		{
			if (m_pBuffer[i] == charToFind)
			{
				return i;
			}
		}

		return npos;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t String_Base<TChar, TAllocator>::Find(const TChar* pStringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		const size_t stringToFindSize = FindCharsCount(pStringToFind);
		ClampTo(endIndex, static_cast<size_t>(0), m_count);

		for (size_t i = startIndex; i < endIndex; ++i)
		{
			if ((i + stringToFindSize) > endIndex)
			{
				return npos;
			}

			if (AreStringsSame(m_pBuffer + i, pStringToFind, stringToFindSize))
			{
				return i;
			}
		}

		return npos;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t String_Base<TChar, TAllocator>::FindFirstOf(TChar charToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		return Find(charToFind, startIndex, endIndex);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t String_Base<TChar, TAllocator>::FindFirstOf(const TChar* pStringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		return Find(pStringToFind, startIndex, endIndex);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t String_Base<TChar, TAllocator>::FindLastOf(TChar charToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		ClampTo(endIndex, size_t(0), m_count);

		for (int64 i = endIndex; i >= static_cast<int64>(startIndex); --i)
		{
			if (i < static_cast<int64>(startIndex))
			{
				return npos;
			}

			if (m_pBuffer[i] == charToFind)
			{
				return i;
			}
		}

		return npos;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t String_Base<TChar, TAllocator>::FindLastOf(const TChar* pStringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		const size_t StringToFindSize = FindCharsCount(pStringToFind);
		ClampTo(endIndex, size_t(0), m_count);

		for (int64 i = endIndex; i >= static_cast<int64>(startIndex); --i)
		{
			if ((i - StringToFindSize) < startIndex)
			{
				return npos;
			}

			if (AreStringsSame(m_pBuffer + i - StringToFindSize, pStringToFind, StringToFindSize))
			{
				return i - StringToFindSize;
			}
		}

		return npos;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr bool String_Base<_TChar, _TAllocator>::BeginsWith(const TChar* pStringToFind) const
	{
		return Find(pStringToFind) == 0;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr bool String_Base<_TChar, _TAllocator>::EndsWith(const TChar* pStringToFind) const
	{
		const size_t stringToFindSize = FindCharsCount(pStringToFind);
		const size_t startIndex = m_count - stringToFindSize;
		const size_t foundLastPos = FindLastOf(pStringToFind);
		return foundLastPos == startIndex;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::Clear()
	{
		DeallocateBuffer();
		m_pBuffer = nullptr;
		m_count = 0;
		m_capacity = 0;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::Replace(const TChar* pStringToFind, const TChar* pStringToReplace, size_t startIndex, size_t endIndex)
	{
		if (endIndex == npos)
		{
			endIndex = m_count;
		}

		const size_t stringToReplaceSize = FindCharsCount(pStringToReplace);
		const size_t stringToFindSize = FindCharsCount(pStringToFind);

		// If the replaced string is larger than the string to find, we need to reserve more space
		if (stringToReplaceSize > stringToFindSize)
		{
			const size_t count = Count(pStringToFind, startIndex, endIndex);
			Reserve(m_count + (stringToReplaceSize - stringToFindSize) * count);
		}

		while (true)
		{
			const size_t foundPos = Find(pStringToFind, startIndex, endIndex);
			if (foundPos == npos)
			{
				break;
			}

			// Move the suffix to the new position
			void* pDestinationToMove  = m_pBuffer + foundPos + stringToReplaceSize;
			const void* pSourceToMove = m_pBuffer + foundPos + stringToFindSize;
			const size_t sizeToMove   = (m_count - foundPos - stringToFindSize + 1) * sizeof(TChar);
			memmove(pDestinationToMove, pSourceToMove, sizeToMove);

			// Insert the new string
			void* pDestinationToInsert = m_pBuffer + foundPos;
			const size_t sizeToInsert  = stringToReplaceSize * sizeof(TChar);
			memcpy(pDestinationToInsert, pStringToReplace, sizeToInsert);

			// Update count and index for the next search
			startIndex = foundPos + stringToReplaceSize;	// In case 'StringToReplace' Has 'StringToFind', like replacing 'x' with 'yx'		
			endIndex += stringToReplaceSize - stringToFindSize;
			m_count  += stringToReplaceSize - stringToFindSize;
		}

		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr DynamicArray<String_Base<TChar, TAllocator>> String_Base<TChar, TAllocator>::Split(const TChar* pKeyword) const
	{
		DynamicArray<String_Base> substrs;
		String_Base current;
		String_Base copy = *this;
		const size_t pKeywordSize = FindCharsCount(pKeyword);

		while (true)
		{
			const size_t keywordIndex = copy.Find(pKeyword);
			if (keywordIndex == npos)
			{
				substrs.EmplaceBack(copy);
				break;
			}

			current = copy.SubStr(0, keywordIndex);
			substrs.EmplaceBack(current);
			copy = copy.SubStr(keywordIndex + pKeywordSize);
		}

		return substrs;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr DynamicArray<String_Base<TChar, TAllocator>> String_Base<TChar, TAllocator>::Split(TChar keyword) const
	{
		DynamicArray<String_Base> substrs;
		String_Base current;
		String_Base copy = *this;

		while (true)
		{
			const size_t keywordIndex = copy.Find(keyword);
			if (keywordIndex == npos)
			{
				substrs.EmplaceBack(copy);
				break;
			}

			current = copy.SubStr(0, keywordIndex);
			substrs.EmplaceBack(current);
			copy = copy.SubStr(keywordIndex + 1);
		}

		return substrs;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator> String_Base<TChar, TAllocator>::SubStr(size_t index, size_t count /*= npos*/) const
	{
		if (count == npos)
		{
			count = m_count - index;
		}

		JPT_ASSERT((index + count) <= m_count, "SubStr cannot exceeds string's bound");
		
		if (count == 0)
		{
			return String_Base<TChar>();
		}

		String_Base<TChar> result;

		if (count < kSmallDataSize)
		{
			StrNCpy(result.m_smallBuffer, kSmallDataSize, &m_pBuffer[index], count);
			result.m_smallBuffer[count] = '\0';
			result.m_pBuffer = result.m_smallBuffer;
			result.m_count = count;
			result.m_capacity = kSmallDataSize - 1;
		}
		else
		{
			TChar* pBuffer = TAllocator::AllocateArray(count + 1);
			StrNCpy(pBuffer, count + 1, &m_pBuffer[index], count);

			result.MoveString(pBuffer, count);
		}

		return result;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void String_Base<_TChar, _TAllocator>::PopBack(size_t count /*= 1*/)
	{
		Resize(m_count - count);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::TrimLeft(size_t index /* = npos*/)
	{
		// If index == npos, trim all the white spaces from the left
		if (index == npos)
		{
			size_t i = 0;
			while (i < m_count && m_pBuffer[i] == ' ')
			{
				++i;
			}

			*this = SubStr(i);
			return;
		}

		// Trim from the left to the index
		JPT_ASSERT(index <= m_count, "Index out of bound");
		*this = SubStr(index);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::TrimRight(size_t index /* = npos*/)
	{
		// If index == npos, trim all the white spaces from the right
		if (index == npos)
		{
			size_t i = m_count - 1;
			while (i >= 0 && m_pBuffer[i] == ' ')
			{
				--i;
			}

			*this = SubStr(0, i + 1);
			return;
		}

		// Trim from right to the index
		JPT_ASSERT(index <= m_count, "Index out of bound");
		*this = SubStr(0, index);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::Insert(TChar c, size_t index)
	{
		TChar cString[2] = { c, '\0' };
		Insert(cString, index, 1);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::Insert(const TChar* CString, size_t index)
	{
		Insert(CString, index, FindCharsCount(CString));
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::Insert(const TChar* CString, size_t index, size_t size)
	{
		JPT_ASSERT(index <= m_count, "Index out of bound");
		JPT_EXIT_IF(size == 0);

		Reserve(m_count + size);

		// Move the suffix to the new position
		void* pDestinationToMove = m_pBuffer + index + size;
		const void* pSourceToMove = m_pBuffer + index;
		const size_t sizeToMove = (m_count - index + 1) * sizeof(TChar);
		memmove(pDestinationToMove, pSourceToMove, sizeToMove);

		// Insert the new string
		void* pDestinationToInsert = m_pBuffer + index;
		const size_t sizeToInsert = size * sizeof(TChar);
		memcpy(pDestinationToInsert, CString, sizeToInsert);

		m_count += size;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::Append(const TChar* CString, size_t newStringSize)
	{
		JPT_EXIT_IF(newStringSize == 0);
		AppendImpl(CString, newStringSize);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::Append(const TChar* CString)
	{
		Append(CString, FindCharsCount(CString));
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::Append(const String_Base<TChar>& otherString)
	{
		JPT_EXIT_IF(otherString.IsEmpty());
		AppendImpl(otherString.ConstBuffer(), otherString.m_count);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::Append(const DynamicArray<String_Base<TChar>>& strings, const TChar* separator)
	{
		size_t newCount = m_count;
		const size_t separatorCount = FindCharsCount(separator);

		for (size_t i = 0; i < strings.Count(); ++i)
		{
			newCount += strings[i].Count();
			if (i != strings.Count() - 1 && separator)
			{
				newCount += separatorCount;
			}
		}

		Reserve(newCount);

		for (size_t i = 0; i < strings.Count(); ++i)
		{
			Append(strings[i]);
			if (i != strings.Count() - 1 && separator)
			{
				Append(separator, separatorCount);
			}
		}
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::Append(TChar c)
	{
		TChar cString[2] = { c, '\0' };
		AppendImpl(cString, 1);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::operator+=(const TChar* CString)
	{
		Append(CString);
		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::operator+=(const String_Base<TChar>& otherString)
	{
		Append(otherString);
		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::operator+=(TChar c)
	{
		Append(c);
		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::Reserve(size_t capacity)
	{
		if (capacity <= m_capacity)
		{
			return;	
		}

		if (capacity < kSmallDataSize)
		{
			DeallocateBuffer();
			m_pBuffer = m_smallBuffer;
			m_capacity = kSmallDataSize - 1;
		}
		else
		{
			TChar* pNewBuffer = TAllocator::AllocateArray(capacity + 1);

			// Copy the old buffer to the new one
			if (m_pBuffer)
			{
				StrCpy(pNewBuffer, m_count + 1, m_pBuffer);
				DeallocateBuffer();
			}

			m_pBuffer = pNewBuffer;
			m_capacity = capacity;
		}
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void String_Base<_TChar, _TAllocator>::Resize(size_t newSize)
	{
		if (newSize == 0)
		{
			Clear();
			return;
		}

		if (newSize < m_count)
		{
			m_count = newSize;
		}
		else
		{
			Reserve(newSize);
			m_count = newSize;
		}

		m_pBuffer[m_count] = '\0';
	}

	template<StringLiteral TChar, class TAllocator>
	template<size_t kCount>
	constexpr String_Base<TChar, TAllocator> String_Base<TChar, TAllocator>::Format(const TChar* format, ...)
	{
		TChar buffer[kCount];

		if constexpr (AreSameType<TChar, char>)
		{
			JPT_FORMAT_STRING(buffer, format, ...);
		}
		else if constexpr (AreSameType<TChar, wchar_t>)
		{
			JPT_FORMAT_WSTRING(buffer, format, ...);
		}

		return String_Base(buffer);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::CopyString(const TChar* inCString, size_t size)
	{
		if (AreStringsSame(m_pBuffer, inCString, m_count, size))
		{
			return;
		}

		if (size == m_count)
		{
			StrNCpy(m_pBuffer, size + 1, inCString, size);
			return;
		}

		DeallocateBuffer();

		if (size == 0)
		{
			m_pBuffer = nullptr;
		}
		else
		{
			if (size < kSmallDataSize)
			{
				StrNCpy(m_smallBuffer, size + 1, inCString, size);
				m_pBuffer = m_smallBuffer;
				m_capacity = kSmallDataSize - 1;
			}
			else 
			{
				m_pBuffer = TAllocator::AllocateArray(size + 1);
				StrNCpy(m_pBuffer, size + 1, inCString, size);
				m_capacity = size;
			}
		}

		m_count = size;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::CopyString(const TChar* inCString)
	{
		CopyString(inCString, FindCharsCount(inCString));
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::CopyString(const String_Base<TChar>& otherString)
	{
		CopyString(otherString.ConstBuffer(), otherString.Count());
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::MoveString(TChar* inCString, size_t size)
	{
		DeallocateBuffer();

		if (size == 0)
		{
			m_pBuffer = nullptr;
		}
		else if (size < kSmallDataSize)
		{
			StrNCpy(m_smallBuffer, size + 1, inCString, size);
			m_pBuffer = m_smallBuffer;
			TAllocator::DeallocateArray(inCString);
			m_capacity = kSmallDataSize - 1;
		}
		else
		{
			m_pBuffer = inCString;
			m_capacity = size;
		}

		m_count     = size;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::MoveString(TChar* inCString)
	{
		MoveString(inCString, FindCharsCount(inCString));
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::MoveString(String_Base<TChar>&& otherString)
	{
		DeallocateBuffer();

		if (otherString.IsEmpty())
		{
			m_pBuffer = nullptr;
		}
		else if (otherString.Count() < kSmallDataSize)
		{
			StrCpy(m_smallBuffer, otherString.m_count + 1, otherString.m_pBuffer);
			m_pBuffer = m_smallBuffer;
			otherString.DeallocateBuffer();
			m_capacity = kSmallDataSize - 1;
		}
		else
		{
			m_pBuffer = otherString.m_pBuffer;
			m_capacity = otherString.m_capacity;
		}

		m_count    = otherString.m_count;

		otherString.m_pBuffer  = nullptr;
		otherString.m_count     = 0;
		otherString.m_capacity = 0;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr uint64 String_Base<TChar, TAllocator>::Hash() const
	{
		return StringHash64(m_pBuffer);
	}

	template<StringLiteral TChar, class TAllocator>
	void String_Base<TChar, TAllocator>::Serialize(Serializer& serializer) const
	{
		serializer.Write(m_count);
		serializer.Write(m_capacity);
		serializer.Write(reinterpret_cast<const char*>(m_smallBuffer), kSmallDataSize * sizeof(TChar));
		serializer.Write(reinterpret_cast<const char*>(m_pBuffer), m_count * sizeof(TChar));
	}

	template<StringLiteral TChar, class TAllocator>
	void String_Base<TChar, TAllocator>::Deserialize(Serializer& serializer)
	{
		size_t count = 0;
		serializer.Read(count);

		size_t capacity = 0;
		serializer.Read(capacity);

		Resize(count);
		serializer.Read(reinterpret_cast<char*>(m_smallBuffer), kSmallDataSize * sizeof(TChar));
		serializer.Read(reinterpret_cast<char*>(m_pBuffer), m_count * sizeof(TChar));
		m_count = count;
		m_capacity = capacity;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::DeallocateBuffer()
	{
		if (m_pBuffer && 
			m_pBuffer != m_smallBuffer)
		{
			TAllocator::DeallocateArray(m_pBuffer);
		}
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::AppendImpl(const TChar* CString, size_t size)
	{
		JPT_EXIT_IF(size == 0);

		const size_t newSize = m_count + size;

		Reserve(newSize);
		StrCpy(m_pBuffer + m_count, size + 1, CString);

		m_count = newSize;
	}

	using String = String_Base<char>;
	using WString = String_Base<wchar_t>;	// Wide string

	template<typename T>
	concept StringType = AreSameType<T, String> || AreSameType<T, WString>;
}