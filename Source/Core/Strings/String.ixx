// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
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
import jpt.StringUtils;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Serializer;

/** Resizing multiplier for capacity */
static constexpr size_t kLocCapacityMultiplier = 2;

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
		size_t m_count = 0;           /**< How many characters in this string currently */
		size_t m_capacity = 0;       /**< How many characters this string can hold before resizing */

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
		constexpr size_t Count()    const { return m_count;      }
		constexpr size_t Size()     const { return m_count * sizeof(TChar); }	/**< How many size in bytes this string in memory */
		constexpr size_t Capacity() const { return m_capacity;   }
		constexpr bool   IsEmpty()  const { return m_count == 0; }

		/** Searching. Returns npos if not found */
		constexpr size_t Find(      TChar  charToFind,    size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t Find(const TChar* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindFirstOf(      TChar charToFind,     size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindFirstOf(const TChar* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindLastOf(      TChar charToFind,     size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
		constexpr size_t FindLastOf(const TChar* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
		constexpr bool   Has(      TChar  charToFind,    size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(charToFind, startIndex, endIndex, count)    != npos; }
		constexpr bool   Has(const TChar* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(pStringToFind, startIndex, endIndex, count) != npos; }
		
		/* Deallocate the memory that this string holds */
		constexpr void Clear();

		/* Replace the some content of this string with the new given one within optional range
			@param StringToFind:	The string we want to remove and replace
			@param StringToReplace:	The string to replace the original data
			@param startIndex:		[optional] The start index to start searching. Default to 0
			@param endIndex:		[optional] The end index to stop operation. Default to Count() */
		constexpr String_Base& Replace(const TChar* pStringToFind, const TChar* pStringToReplace, size_t startIndex = 0, size_t endIndex = npos);
		constexpr String_Base GetReplaced(const TChar* pStringToFind, const TChar* pStringToReplace, size_t startIndex = 0, size_t endIndex = npos) const;
		constexpr String_Base& Replace(const DynamicArray<String_Base>& stringsToFind, const TChar* pStringToReplace, size_t startIndex = 0, size_t endIndex = npos);
		constexpr String_Base GetReplaced(const DynamicArray<String_Base>& stringsToFind, const TChar* pStringToReplace, size_t startIndex = 0, size_t endIndex = npos) const;

		/** Splits this string to substrings by a keyword */
		constexpr DynamicArray<String_Base> Split(const TChar* pKeyword) const;
		constexpr DynamicArray<String_Base> Split(TChar keyword) const;

		/** @return		A sub string within the given range at index and length */
		constexpr String_Base SubStr(size_t index, size_t count = npos) const;

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

		/** @return An integer associated with this string
			@note   Will assert fail if Has non-numeric literals besides the negative sign at the front */
		template<Integral TInt = int32>
		constexpr TInt ToInt() const;

		/** @return A float associated with this string
			@note   Will assert fail if Has non-numeric literals besides the negative sign at the front or the percision dot
			@note	Will ignore the 'f' is there's any */
		template<Floating TFloat = float>
		constexpr TFloat ToFloat() const;

		/** Converts all the characters to lowercase/uppercase */
		constexpr void MakeLower();
		constexpr void MakeUpper();
		static constexpr String_Base GetLower(const String_Base& string);
		static constexpr String_Base GetUpper(const String_Base& string);

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

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t String_Base<TChar, TAllocator>::Find(TChar charToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
	{
		ClampTo(endIndex, size_t(0), m_count);

		for (size_t i = startIndex; i < endIndex; ++i)
		{
			if ((i + 1) > endIndex)
			{
				return npos;
			}

			if (m_pBuffer[i] == charToFind)
			{
				--count;
				if (count == 0)
				{
					return i;
				}
			}
		}

		return npos;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t String_Base<TChar, TAllocator>::Find(const TChar* pStringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		const size_t StringToFindSize = FindCharsCount(pStringToFind);
		ClampTo(endIndex, static_cast<size_t>(0), m_count);

		String_Base<TChar> current;
		for (size_t i = startIndex; i < endIndex; ++i)
		{
			if ((i + StringToFindSize) > endIndex)
			{
				return npos;
			}

			current = SubStr(i, StringToFindSize);
			if (AreStringsSame(current.ConstBuffer(), pStringToFind, current.Count(), StringToFindSize))
			{
				--count;
				if (count == 0)
				{
					return i;
				}
			}
		}

		return npos;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t String_Base<TChar, TAllocator>::FindFirstOf(TChar charToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		return Find(charToFind, startIndex, endIndex, count);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t String_Base<TChar, TAllocator>::FindFirstOf(const TChar* pStringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		return Find(pStringToFind, startIndex, endIndex, count);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t String_Base<TChar, TAllocator>::FindLastOf(TChar charToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		ClampTo(endIndex, size_t(0), m_count);

		for (int64 i = endIndex - 1; i >= static_cast<int64>(startIndex); --i)
		{
			if (i < static_cast<int64>(startIndex))
			{
				return npos;
			}

			if (m_pBuffer[i] == charToFind)
			{
				--count;
				if (count == 0)
				{
					return i;
				}
			}
		}

		return npos;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t String_Base<TChar, TAllocator>::FindLastOf(const TChar* pStringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		const size_t StringToFindSize = FindCharsCount(pStringToFind);
		ClampTo(endIndex, size_t(0), m_count);

		String_Base<TChar> current;
		for (int64 i = endIndex - 1; i >= static_cast<int64>(startIndex); --i)
		{
			if ((i - StringToFindSize) < startIndex)
			{
				return npos;
			}

			current = SubStr(i - StringToFindSize, StringToFindSize);
			if (AreStringsSame(current.ConstBuffer(), pStringToFind, current.Count(), StringToFindSize))
			{
				--count;
				if (count == 0)
				{
					return i - StringToFindSize;
				}
			}
		}

		return npos;
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

		const String_Base<TChar> replaced(pStringToReplace);
		const size_t stringToFindSize = FindCharsCount(pStringToFind);

		size_t foundPos = startIndex;
		while (true)
		{
			foundPos = Find(pStringToFind, startIndex, endIndex);
			if (foundPos == npos)
			{
				break;
			}

			String_Base<TChar> pre = SubStr(0, foundPos);
			String_Base<TChar> suff = SubStr(foundPos + stringToFindSize);

			*this = Move(pre) + replaced + Move(suff);

			startIndex = foundPos + replaced.Count();	// In case 'StringToReplace' Has 'StringToFind', like replacing 'x' with 'yx'		
			endIndex += replaced.Count() - stringToFindSize;
		}

		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator> String_Base<TChar, TAllocator>::GetReplaced(const TChar* pStringToFind, const TChar* pStringToReplace, size_t startIndex, size_t endIndex) const
	{
		String_Base copy = *this;
		copy.Replace(pStringToFind, pStringToReplace, startIndex, endIndex);
		return copy;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::Replace(const DynamicArray<String_Base>& stringsToFind, const TChar* pStringToReplace, size_t startIndex, size_t endIndex)
	{
		for (const String_Base& stringToFind : stringsToFind)
		{
			Replace(stringToFind.ConstBuffer(), pStringToReplace, startIndex, endIndex);
		}

		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator> String_Base<TChar, TAllocator>::GetReplaced(const DynamicArray<String_Base>& stringsToFind, const TChar* pStringToReplace, size_t startIndex, size_t endIndex) const
	{
		String_Base copy = *this;
		copy.Replace(stringsToFind, pStringToReplace, startIndex, endIndex);
		return copy;
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
			TChar subStrBuffer[kSmallDataSize] = { 0 };
			StrNCpy(subStrBuffer, kSmallDataSize, &m_pBuffer[index], count);

			result.CopyString(subStrBuffer, count);
		}
		else
		{
			TChar* pBuffer = TAllocator::AllocateArray(count + sizeof(TChar));
			StrNCpy(pBuffer, count + sizeof(TChar), &m_pBuffer[index], count);

			result.MoveString(pBuffer, count);
		}

		return result;
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

		String_Base suff = SubStr(index);

		TrimRight(index);

		Append(CString, size);
		Append(suff);
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
		capacity += sizeof(TChar); // Null terminator

		if (capacity >= kSmallDataSize &&
			capacity > m_capacity)
		{
			TChar* pNewBuffer = TAllocator::AllocateArray(capacity);

			// Copy the old buffer to the new one
			if (m_pBuffer)
			{
				StrCpy(pNewBuffer, m_count + sizeof(TChar), m_pBuffer);
				DeallocateBuffer();
			}

			m_pBuffer = pNewBuffer;
			m_capacity = capacity;
		}

		if (capacity < kSmallDataSize)
		{
			DeallocateBuffer();
			m_pBuffer = m_smallBuffer;
			m_capacity = kSmallDataSize;
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
	template<Integral TInt>
	constexpr TInt String_Base<TChar, TAllocator>::ToInt() const
	{
		return CStrToInteger<TChar, TInt>(m_pBuffer, m_count);
	}

	template<StringLiteral TChar, class TAllocator>
	template<Floating TFloat>
	constexpr TFloat String_Base<TChar, TAllocator>::ToFloat() const
	{
		return CStrToFloat<TChar, TFloat>(m_pBuffer, m_count);
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
			StrNCpy(m_pBuffer, size + sizeof(TChar), inCString, size);
			m_capacity = m_count;
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
				StrNCpy(m_smallBuffer, size + sizeof(TChar), inCString, size);
				m_pBuffer = m_smallBuffer;
			}
			else 
			{
				m_pBuffer = TAllocator::AllocateArray(size + sizeof(TChar));
				StrNCpy(m_pBuffer, size + sizeof(TChar), inCString, size);
			}
		}

		m_count    = size;
		m_capacity = m_count;
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
			StrNCpy(m_smallBuffer, size + sizeof(TChar), inCString, size);
			m_pBuffer = m_smallBuffer;
			TAllocator::DeallocateArray(inCString);
		}
		else
		{
			m_pBuffer = inCString;
		}

		m_count     = size;
		m_capacity = m_count;
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
			StrCpy(m_smallBuffer, otherString.m_count + sizeof(TChar), otherString.m_pBuffer);
			m_pBuffer = m_smallBuffer;
			otherString.DeallocateBuffer();
		}
		else
		{
			m_pBuffer = otherString.m_pBuffer;
		}

		m_count    = otherString.m_count;
		m_capacity = otherString.m_capacity;

		otherString.m_pBuffer  = nullptr;
		otherString.m_count     = 0;
		otherString.m_capacity = 0;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::MakeLower()
	{
		for (size_t i = 0; i < m_count; ++i)
		{
			m_pBuffer[i] = jpt::GetLower(m_pBuffer[i]);
		}
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void String_Base<TChar, TAllocator>::MakeUpper()
	{
		for (size_t i = 0; i < m_count; ++i)
		{
			m_pBuffer[i] = jpt::GetUpper(m_pBuffer[i]);
		}
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator> String_Base<TChar, TAllocator>::GetLower(const String_Base& string)
	{
		String_Base str = string;
		str.MakeLower();
		return str;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr String_Base<TChar, TAllocator> String_Base<TChar, TAllocator>::GetUpper(const String_Base& string)
	{
		String_Base str = string;
		str.MakeUpper();
		return str;
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
		StrCpy(m_pBuffer + m_count, size + sizeof(TChar), CString);

		m_count = newSize;
	}

	using String = String_Base<char>;
	using WString = String_Base<wchar_t>;	// Wide string

	template<typename T>
	concept StringType = AreSameType<T, String> || AreSameType<T, WString>;
}