// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

#include <stdio.h>
#include <vadefs.h>
#include <stdarg.h>

export module jpt.String;

import jpt.Allocator;
import jpt.Concepts;
import jpt.Constants;
import jpt.Utilities;
import jpt.Math;
import jpt.StringUtils;
import jpt.TypeDefs;
import jpt.DynamicArray;


// SSO
// - Refactor every StrCpy or StrNCpy to correspond small buffer when size is less than kSmallDataSize
// - Reallocate, Allocate, Insert, SubStr, Copy, Move should also take care of small buffer when applicable

/** Resizing multiplier for capacity */
static constexpr size_t kLocCapacityMultiplier = 2;

export namespace jpt
{
	template<StringLiteral _TChar, class _TAllocator = Allocator<_TChar>>
	class BasicString
	{
	public:
		using TChar         = _TChar;
		using TAllocator    = _TAllocator;
		using Iterator      = TChar*;
		using ConstIterator = const TChar*;

	private:
		TChar m_smallBuffer[kSmallDataSize] = { 0 };	/**< Small buffer to store small data */
		TChar* m_pBuffer = nullptr;  /**< The pointer to the buffer representing this string's value */
		size_t m_size = 0;           /**< How many characters in this string currently */
		size_t m_capacity = 0;       /**< How many characters this string can hold before resizing */

	public:
		constexpr BasicString() = default;
		constexpr BasicString(const TChar* CString, size_t size);
		constexpr BasicString(const TChar* CString);
		constexpr BasicString(TChar c);
		constexpr BasicString(const BasicString<TChar>& otherString);
		constexpr BasicString(BasicString<TChar>&& otherString) noexcept;
		BasicString& operator=(const TChar* CString);
		BasicString& operator=(const BasicString<TChar>& otherString);
		BasicString& operator=(BasicString<TChar>&& otherString) noexcept;
		constexpr ~BasicString();

		// Element Access
		constexpr const TChar* ConstBuffer() const { return m_pBuffer; }
		constexpr       TChar* Buffer()      const { return m_pBuffer; }
		constexpr       TChar& Front()             { return m_pBuffer[0]; }
		constexpr const TChar& Front()       const { return m_pBuffer[0]; }
		constexpr       TChar& Back()              { return m_pBuffer[m_size - 1]; }
		constexpr const TChar& Back()        const { return m_pBuffer[m_size - 1]; }
		constexpr       TChar& operator[](size_t index)       { return m_pBuffer[index]; }
		constexpr const TChar& operator[](size_t index) const { return m_pBuffer[index]; }

		// Iterators
		constexpr Iterator begin() { return Iterator(m_pBuffer); }
		constexpr Iterator end()   { return Iterator(m_pBuffer + m_size); }
		constexpr ConstIterator begin()  const { return ConstIterator(m_pBuffer); }
		constexpr ConstIterator cbegin() const { return ConstIterator(m_pBuffer); }
		constexpr ConstIterator end()    const { return ConstIterator(m_pBuffer + m_size); }
		constexpr ConstIterator cend()   const { return ConstIterator(m_pBuffer + m_size); }

		// Capacity
		constexpr size_t Size()     const { return m_size;      }
		constexpr size_t Capacity() const { return m_capacity;  }
		constexpr bool   IsEmpty()  const { return m_size == 0; }

		/** Searching. Returns npos if not found */
		constexpr size_t Find(      TChar  TCharoFind,   size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t Find(const TChar* StringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindFirstOf(      TChar TCharoFind,    size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindFirstOf(const TChar* StringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindLastOf(      TChar TCharoFind,    size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
		constexpr size_t FindLastOf(const TChar* StringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
		constexpr bool   Contains(      TChar  TCharoFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(TCharoFind, startIndex, endIndex, count) != npos; }
		constexpr bool   Contains(const TChar* TCharoFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(TCharoFind, startIndex, endIndex, count) != npos; }
		
		/* Deallocate the memory that this string holds */
		constexpr void Clear();

		/* Replace the some content of this string with the new given one within optional range
			@param StringToFind:	The string we want to remove and replace
			@param StringToReplace:	The string to replace the original data
			@param startIndex:		[optional] The start index to start searching. Default to 0
			@param endIndex:		[optional] The end index to stop operation. Default to size() */
		constexpr BasicString& Replace(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex = 0, size_t endIndex = npos);
		constexpr BasicString GetReplaced(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex = 0, size_t endIndex = npos) const;

		/** Splits this string to substrings by a keyword */
		constexpr DynamicArray<BasicString> Split(const TChar* pKeyword) const;
		constexpr DynamicArray<BasicString> Split(TChar keyword) const;

		/** @return		A sub string within the given range at index and length */
		constexpr BasicString SubStr(size_t index, size_t count = npos) const;

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
		constexpr void Append(const BasicString<TChar>& otherString);
		constexpr void Append(TChar c);
		constexpr BasicString& operator+=(const TChar* CString);
		constexpr BasicString& operator+=(const BasicString<TChar>& otherString);
		constexpr BasicString& operator+=(TChar c);

		/** Pre allocate buffer with capacity's size. Preventing oftenly dynamic heap allocation */
		constexpr void Reserve(size_t capacity);

		/** Formats data to a string with provided format then return it
			@example String::Format<32>("%d/%d/%d. %d:%d:%d", month, day, year, hour, minute, second); */
		template<size_t kSize>
		static constexpr BasicString Format(const TChar* format, ...);

		/* Copy the content of string. Will assign the current m_pBuffer with the new copied data in memory */
		constexpr void CopyString(const TChar* inCString, size_t size);
		constexpr void CopyString(const TChar* inCString);
		constexpr void CopyString(const BasicString<TChar>& otherString);

		/* Move the content of string. Will take ownership of the passed in string */
		constexpr void MoveString(TChar* inCString, size_t size);
		constexpr void MoveString(TChar* inCString);
		constexpr void MoveString(BasicString<TChar>&& otherString);

		/** @return An integer associated with this string
			@note   Will assert fail if contains non-numeric literals besides the negative sign at the front */
		template<Integral TInt = int32>
		constexpr TInt ToInt() const;

		/** @return A float associated with this string
			@note   Will assert fail if contains non-numeric literals besides the negative sign at the front or the percision dot
			@note	Will ignore the 'f' is there's any */
		template<Floating TFloat = float>
		constexpr TFloat ToFloat() const;

	private:
		/* Called when the current buffer is not big enough to hold a new string to append. Update the buffer to a larger sizeand increase capacity
		   @param inCapacity: The capacity for the new buffer. Typically current m_size * kCapacityMultiplier */
		constexpr void ReAllocateBuffer(size_t inCapacity);

		constexpr void DeallocateBuffer();

		/** Implementation of appending a C-String at the index by the given size */
		constexpr void AppendImpl(const TChar* CString, size_t size);
	};

	// Non member functions -------------------------------------------------------------------------------------------------------------------
	template<StringLiteral TChar, class TAllocator>
	constexpr BasicString<TChar, TAllocator> operator+(const BasicString<TChar, TAllocator>& string, const TChar* CString)
	{
		BasicString<TChar> str = string;
		str.Append(CString);
		return str;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr BasicString<TChar, TAllocator> operator+(BasicString<TChar, TAllocator>&& string, const TChar* CString)
	{
		BasicString<TChar> str = Move(string);
		str.Append(CString);
		return str;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr BasicString<TChar, TAllocator> operator+(const BasicString<TChar, TAllocator>& string, const BasicString<TChar>& otherString)
	{
		BasicString<TChar> str = string;
		str.Append(otherString);
		return str;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr BasicString<TChar, TAllocator> operator+(BasicString<TChar, TAllocator>&& string, const BasicString<TChar>& otherString)
	{
		BasicString<TChar> str = Move(string);
		str.Append(otherString);
		return str;
	}

	template<StringLiteral TChar>
	constexpr BasicString<TChar> operator+(const TChar* CString, const BasicString<TChar>& string)
	{
		return jpt::BasicString<TChar>(CString) += string;
	}

	template<StringLiteral TChar>
	constexpr BasicString<TChar> operator+(const TChar* CString, BasicString<TChar>&& rightString)
	{
		return jpt::BasicString<TChar>(CString) += Move(rightString);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr bool operator==(const BasicString<TChar, TAllocator>& string, const TChar* CString)
	{
		return AreStringsSame(string.ConstBuffer(), CString, string.Size(), GetCStrLength(CString));
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr bool operator==(const BasicString<TChar, TAllocator>& lhs, const BasicString<TChar>& rhs)
	{
		return AreStringsSame(lhs.ConstBuffer(), rhs.ConstBuffer(), lhs.Size(), rhs.Size());
	}

	// Member Functions Definitions ---------------------------------------------------------------------------------------
	template<StringLiteral TChar, class TAllocator>
	constexpr BasicString<TChar, TAllocator>::BasicString(const TChar* CString, size_t size)
	{
		CopyString(CString, size);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr BasicString<TChar, TAllocator>::BasicString(const TChar* CString)
	{
		CopyString(CString);
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr BasicString<_TChar, _TAllocator>::BasicString(TChar c)
	{
		TChar cString[2] = { c, '\0' };
		MoveString(cString, 1);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr BasicString<TChar, TAllocator>::BasicString(const BasicString<TChar>& otherString)
	{
		CopyString(otherString);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr BasicString<TChar, TAllocator>::BasicString(BasicString<TChar>&& otherString) noexcept
	{
		MoveString(Move(otherString));
	}

	template<StringLiteral TChar, class TAllocator>
	BasicString<TChar, TAllocator>& BasicString<TChar, TAllocator>::operator=(const TChar* CString)
	{
		if (!AreStringsSame(m_pBuffer, CString, m_size, GetCStrLength(CString)))
		{
			CopyString(CString);
		}

		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	BasicString<TChar, TAllocator>& BasicString<TChar, TAllocator>::operator=(const BasicString<TChar>& otherString)
	{
		if (this != &otherString)
		{
			CopyString(otherString);
		}

		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	BasicString<TChar, TAllocator>& BasicString<TChar, TAllocator>::operator=(BasicString<TChar>&& otherString) noexcept
	{
		if (this != &otherString)
		{
			MoveString(Move(otherString));
		}

		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr BasicString<TChar, TAllocator>::~BasicString()
	{
		Clear();
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t BasicString<TChar, TAllocator>::Find(TChar TCharoFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
	{
		ClampTo(endIndex, size_t(0), m_size);

		for (size_t i = startIndex; i < endIndex; ++i)
		{
			if ((i + 1) > endIndex)
			{
				return npos;
			}

			if (m_pBuffer[i] == TCharoFind)
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
	constexpr size_t BasicString<TChar, TAllocator>::Find(const TChar* StringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		const size_t StringToFindSize = GetCStrLength(StringToFind);
		ClampTo(endIndex, static_cast<size_t>(0), m_size);

		BasicString<TChar> current;
		for (size_t i = startIndex; i < endIndex; ++i)
		{
			if ((i + StringToFindSize) > endIndex)
			{
				return npos;
			}

			current = SubStr(i, StringToFindSize);
			if (current == StringToFind)
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
	constexpr size_t BasicString<TChar, TAllocator>::FindFirstOf(TChar TCharoFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		return Find(TCharoFind, startIndex, endIndex);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t BasicString<TChar, TAllocator>::FindFirstOf(const TChar* StringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		return Find(StringToFind, startIndex, endIndex);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr size_t BasicString<TChar, TAllocator>::FindLastOf(TChar TCharoFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		ClampTo(endIndex, size_t(0), m_size);

		for (int64 i = endIndex - 1; i >= static_cast<int64>(startIndex); --i)
		{
			if (i < static_cast<int64>(startIndex))
			{
				return npos;
			}

			if (m_pBuffer[i] == TCharoFind)
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
	constexpr size_t BasicString<TChar, TAllocator>::FindLastOf(const TChar* StringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		const size_t StringToFindSize = GetCStrLength(StringToFind);
		ClampTo(endIndex, size_t(0), m_size);

		BasicString<TChar> current;
		for (int64 i = endIndex - 1; i >= static_cast<int64>(startIndex); --i)
		{
			if ((i - StringToFindSize) < startIndex)
			{
				return npos;
			}

			current = SubStr(i - StringToFindSize, StringToFindSize);
			if (current == StringToFind)
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
	constexpr void BasicString<TChar, TAllocator>::Clear()
	{
		DeallocateBuffer();
		m_pBuffer = nullptr;
		m_size = 0;
		m_capacity = 0;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr BasicString<TChar, TAllocator>& BasicString<TChar, TAllocator>::Replace(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex, size_t endIndex)
	{
		if (endIndex == npos)
		{
			endIndex = m_size;
		}

		const BasicString<TChar> replaced(StringToReplace);
		const size_t StringToFindSize = GetCStrLength(StringToFind);

		size_t foundPos = startIndex;
		while (true)
		{
			foundPos = Find(StringToFind, startIndex, endIndex);
			if (foundPos == npos)
			{
				break;
			}

			BasicString<TChar> pre = SubStr(0, foundPos);
			BasicString<TChar> suff = SubStr(foundPos + StringToFindSize);

			*this = Move(pre) + replaced + Move(suff);
			startIndex = foundPos + replaced.Size();	// In case 'StringToReplace' contains 'StringToFind', like replacing 'x' with 'yx'		
		}

		return *this;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr BasicString<_TChar, _TAllocator> BasicString<_TChar, _TAllocator>::GetReplaced(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex, size_t endIndex) const
	{
		BasicString copy = *this;
		copy.Replace(StringToFind, StringToReplace, startIndex, endIndex);
		return copy;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr DynamicArray<BasicString<_TChar, _TAllocator>> BasicString<_TChar, _TAllocator>::Split(const TChar* pKeyword) const
	{
		DynamicArray<BasicString> substrs;
		BasicString current;
		BasicString copy = *this;
		const size_t pKeywordSize = GetCStrLength(pKeyword);

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

	template<StringLiteral _TChar, class _TAllocator>
	constexpr DynamicArray<BasicString<_TChar, _TAllocator>> BasicString<_TChar, _TAllocator>::Split(TChar keyword) const
	{
		DynamicArray<BasicString> substrs;
		BasicString current;
		BasicString copy = *this;

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
	constexpr BasicString<TChar, TAllocator> BasicString<TChar, TAllocator>::SubStr(size_t index, size_t count /*= npos*/) const
	{
		if (count == npos)
		{
			count = m_size - index;
		}

		JPT_ASSERT((index + count) <= m_size, "SubStr cannot exceeds string's bound");
		
		if (count == 0)
		{
			return BasicString<TChar>();
		}

		TChar* subStrBuffer = TAllocator::AllocateArray(count + sizeof(TChar));
		StrNCpy(subStrBuffer, count + sizeof(TChar), &m_pBuffer[index], count);

		jpt::BasicString<TChar> result;
		result.MoveString(subStrBuffer, count);
		return result;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void BasicString<_TChar, _TAllocator>::TrimLeft(size_t index /* = npos*/)
	{
		// If index == npos, trim all the white spaces from the left
		if (index == npos)
		{
			size_t i = 0;
			while (i < m_size && m_pBuffer[i] == ' ')
			{
				++i;
			}

			*this = SubStr(i);
			return;
		}

		// Trim from the left to the index
		JPT_ASSERT(index <= m_size, "Index out of bound");
		*this = SubStr(index);
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void BasicString<_TChar, _TAllocator>::TrimRight(size_t index /* = npos*/)
	{
		// If index == npos, trim all the white spaces from the right
		if (index == npos)
		{
			size_t i = m_size - 1;
			while (i >= 0 && m_pBuffer[i] == ' ')
			{
				--i;
			}

			*this = SubStr(0, i + 1);
			return;
		}

		// Trim from right to the index
		JPT_ASSERT(index <= m_size, "Index out of bound");
		*this = SubStr(0, index);
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void BasicString<_TChar, _TAllocator>::Insert(TChar c, size_t index)
	{
		TChar cString[2] = { c, '\0' };
		Insert(cString, index, 1);
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void BasicString<_TChar, _TAllocator>::Insert(const TChar* CString, size_t index)
	{
		Insert(CString, index, GetCStrLength(CString));
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void BasicString<_TChar, _TAllocator>::Insert(const TChar* CString, size_t index, size_t size)
	{
		JPT_ASSERT(index <= m_size, "Index out of bound");
		JPT_EXIT_IF(size == 0);

		BasicString pre = SubStr(0, index);
		BasicString mid(CString, size);
		BasicString suff = SubStr(index);

		*this = Move(pre) + Move(mid) + Move(suff);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void BasicString<TChar, TAllocator>::Append(const TChar* CString, size_t newStringSize)
	{
		JPT_EXIT_IF(newStringSize == 0);
		AppendImpl(CString, newStringSize);
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void BasicString<_TChar, _TAllocator>::Append(const TChar* CString)
	{
		Append(CString, GetCStrLength(CString));
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void BasicString<TChar, TAllocator>::Append(const BasicString<TChar>& otherString)
	{
		JPT_EXIT_IF(otherString.IsEmpty());
		AppendImpl(otherString.ConstBuffer(), otherString.m_size);
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void BasicString<_TChar, _TAllocator>::Append(TChar c)
	{
		TChar cString[2] = { c, '\0' };
		AppendImpl(cString, 1);
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr BasicString<_TChar, _TAllocator>& BasicString<_TChar, _TAllocator>::operator+=(const TChar* CString)
	{
		Append(CString);
		return *this;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr BasicString<_TChar, _TAllocator>& BasicString<_TChar, _TAllocator>::operator+=(const BasicString<TChar>& otherString)
	{
		Append(otherString);
		return *this;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr BasicString<_TChar, _TAllocator>& BasicString<_TChar, _TAllocator>::operator+=(TChar c)
	{
		Append(c);
		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void BasicString<TChar, TAllocator>::Reserve(size_t capacity)
	{
		if (capacity > m_capacity)
		{
			ReAllocateBuffer(capacity);
		}
	}

	template<StringLiteral _TChar, class _TAllocator>
	template<size_t kSize>
	constexpr BasicString<_TChar, _TAllocator> BasicString<_TChar, _TAllocator>::Format(const TChar* format, ...)
	{
		TChar buffer[kSize];
		JPT_FORMAT_STRING(buffer, format, ...);
		return BasicString(buffer);
	}

	template<StringLiteral _TChar, class _TAllocator>
	template<Integral TInt>
	constexpr TInt BasicString<_TChar, _TAllocator>::ToInt() const
	{
		return CStrToInteger(m_pBuffer, m_size);
	}

	template<StringLiteral _TChar, class _TAllocator>
	template<Floating TFloat>
	constexpr TFloat BasicString<_TChar, _TAllocator>::ToFloat() const
	{
		return CStrToFloat(m_pBuffer, m_size);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void BasicString<TChar, TAllocator>::CopyString(const TChar* inCString, size_t size)
	{
		if (size == 0)
		{
			Clear();
			return;
		}

		if (size == m_size)
		{
			StrCpy(m_pBuffer, size + sizeof(TChar), inCString);
		}
		else
		{
			DeallocateBuffer();
			m_pBuffer = TAllocator::AllocateArray(size + sizeof(TChar));
			StrCpy(m_pBuffer, size + sizeof(TChar), inCString);
		}

		m_size     = size;
		m_capacity = m_size;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void BasicString<_TChar, _TAllocator>::CopyString(const TChar* inCString)
	{
		CopyString(inCString, GetCStrLength(inCString));
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void BasicString<_TChar, _TAllocator>::CopyString(const BasicString<TChar>& otherString)
	{
		CopyString(otherString.ConstBuffer(), otherString.Size());
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void BasicString<TChar, TAllocator>::MoveString(TChar* inCString, size_t size)
	{
		DeallocateBuffer();

		m_pBuffer  = inCString;
		m_size     = size;
		m_capacity = m_size;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void BasicString<_TChar, _TAllocator>::MoveString(TChar* inCString)
	{
		MoveString(inCString, GetCStrLength(inCString));
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void BasicString<TChar, TAllocator>::MoveString(BasicString<TChar>&& otherString)
	{
		DeallocateBuffer();

		m_pBuffer  = otherString.m_pBuffer;
		m_size     = otherString.m_size;
		m_capacity = otherString.m_capacity;

		otherString.m_pBuffer  = nullptr;
		otherString.m_size     = 0;
		otherString.m_capacity = 0;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void BasicString<TChar, TAllocator>::ReAllocateBuffer(size_t inCapacity)
	{
		TChar* pNewBuffer = TAllocator::AllocateArray(inCapacity + sizeof(TChar));

		// Copy the old buffer to the new one
		if (m_pBuffer)
		{
			StrCpy(pNewBuffer, m_size + sizeof(TChar), m_pBuffer);
			DeallocateBuffer();
		}

		m_pBuffer = pNewBuffer;
		m_capacity = inCapacity;
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void BasicString<_TChar, _TAllocator>::DeallocateBuffer()
	{
		if (m_pBuffer && 
			m_pBuffer != m_smallBuffer)
		{
			TAllocator::DeallocateArray(m_pBuffer);
		}
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void BasicString<TChar, TAllocator>::AppendImpl(const TChar* CString, size_t size)
	{
		JPT_EXIT_IF(size == 0);

		const size_t newSize = m_size + size;
		Reserve(newSize);

		StrCpy(m_pBuffer + m_size, size + sizeof(TChar), CString);

		m_size = newSize;
	}

	using String = BasicString<char>;
	using WString = BasicString<wchar_t>;	// Wide string

	template<typename T>
	concept BasicStringType = IsSameType<T, String> || IsSameType<T, WString>;

	template<>
	struct Hash<String>
	{
		size_t operator()(const String& key)
		{
			return jpt::StringHash64(key.ConstBuffer());
		}
	};
}