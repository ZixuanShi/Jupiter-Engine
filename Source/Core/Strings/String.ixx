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
//import jpt.DynamicArray;

import jpt_private.ContiguousIterator;

/** Resizing multiplier for capacity */
static constexpr size_t kLocCapacityMultiplier = 2;

export namespace jpt
{
	template<StringLiteral _TChar, class _TAllocator = Allocator<_TChar>>
	class BasicString
	{
	public:
		using TChar      = _TChar;
		using TAllocator = _TAllocator;
		using Iterator   = jpt_private::ContiguousIterator<TChar>;

	private:
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
		constexpr       Iterator begin()        { return Iterator(m_pBuffer); }
		constexpr const Iterator begin()  const { return Iterator(m_pBuffer); }
		constexpr const Iterator cbegin() const { return Iterator(m_pBuffer); }
		constexpr       Iterator end()          { return Iterator(m_pBuffer + m_size); }
		constexpr const Iterator end()    const { return Iterator(m_pBuffer + m_size); }
		constexpr const Iterator cend()   const { return Iterator(m_pBuffer + m_size); }

		// Capacity
		constexpr size_t Size()     const { return m_size;      }
		constexpr size_t Capacity() const { return m_capacity;  }
		constexpr bool   IsEmpty()  const { return m_size == 0; }

		/** Searching. Returns npos if not found */
		constexpr size_t Find(      TChar  TCharoFind,   size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t Find(const TChar* TStringoFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindFirstOf(      TChar TCharoFind,    size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindFirstOf(const TChar* TStringoFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindLastOf(      TChar TCharoFind,    size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
		constexpr size_t FindLastOf(const TChar* TStringoFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
		constexpr bool   Contains(      TChar  TCharoFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(TCharoFind, startIndex, endIndex, count) != npos; }
		constexpr bool   Contains(const TChar* TCharoFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(TCharoFind, startIndex, endIndex, count) != npos; }
		
		/* Deallocate the memory that this string holds */
		constexpr void Clear();

		/* Replace the some content of this string with the new given one within optional range
			@param TStringoFind:	The string we want to remove and replace
			@param TStringoReplace:	The string to replace the original data
			@param startIndex:		[optional] The start index to start searching. Default to 0
			@param endIndex:		[optional] The end index to stop operation. Default to size() */
		constexpr BasicString& Replace(const TChar* TStringoFind, const TChar* TStringoReplace, size_t startIndex = 0, size_t endIndex = npos);

		/** Splits this string to substrings by a keyword, stored by a dynamic array */
		//constexpr DynamicArray<BasicString> Split(const TChar* pKeyword) const;

		/** @return		A sub string within the given range at index and length */
		constexpr BasicString SubStr(size_t index, size_t count = npos) const;

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
		constexpr void UpdateBuffer(size_t inCapacity);

		/** Inserts a C-String at the index by the given size */
		constexpr void InsertStringAt(const TChar* CString, size_t index, size_t size);
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
	constexpr BasicString<TChar> operator+(const TChar* leftString, const BasicString<TChar>& rightString)
	{
		return jpt::BasicString<TChar>(leftString) += rightString;
	}

	template<StringLiteral TChar>
	constexpr BasicString<TChar> operator+(const TChar* leftString, BasicString<TChar>&& rightString)
	{
		return jpt::BasicString<TChar>(leftString) += Move(rightString);
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr bool operator==(const BasicString<TChar, TAllocator>& string, const TChar* otherString)
	{
		return AreStringsSame(string.ConstBuffer(), otherString, string.Size());
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr bool operator==(const BasicString<TChar, TAllocator>& string, const BasicString<TChar>& otherString)
	{
		if (string.Size() != otherString.Size())
		{
			return false;
		}

		for (size_t i = 0; i < string.Size(); ++i)
		{
			if (string[i] != otherString[i])
			{
				return false;
			}
		}

		return true;
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
		TChar* cString = TAllocator::AllocateArray(2);
		cString[0] = c;
		cString[1] = '\0';
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
			Clear();
			CopyString(CString);
		}

		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	BasicString<TChar, TAllocator>& BasicString<TChar, TAllocator>::operator=(const BasicString<TChar>& otherString)
	{
		if (this != &otherString)
		{
			Clear();
			CopyString(otherString);
		}

		return *this;
	}

	template<StringLiteral TChar, class TAllocator>
	BasicString<TChar, TAllocator>& BasicString<TChar, TAllocator>::operator=(BasicString<TChar>&& otherString) noexcept
	{
		if (this != &otherString)
		{
			Clear();
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
	constexpr size_t BasicString<TChar, TAllocator>::Find(const TChar* TStringoFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		const size_t TStringoFindSize = GetCStrLength(TStringoFind);
		ClampTo(endIndex, static_cast<size_t>(0), m_size);

		BasicString<TChar> current;
		for (size_t i = startIndex; i < endIndex; ++i)
		{
			if ((i + TStringoFindSize) > endIndex)
			{
				return npos;
			}

			current = SubStr(i, TStringoFindSize);
			if (current == TStringoFind)
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
	constexpr size_t BasicString<TChar, TAllocator>::FindFirstOf(const TChar* TStringoFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		return Find(TStringoFind, startIndex, endIndex);
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
	constexpr size_t BasicString<TChar, TAllocator>::FindLastOf(const TChar* TStringoFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		const size_t TStringoFindSize = GetCStrLength(TStringoFind);
		ClampTo(endIndex, size_t(0), m_size);

		BasicString<TChar> current;
		for (int64 i = endIndex - 1; i >= static_cast<int64>(startIndex); --i)
		{
			if ((i - TStringoFindSize) < startIndex)
			{
				return npos;
			}

			current = SubStr(i - TStringoFindSize, TStringoFindSize);
			if (current == TStringoFind)
			{
				--count;
				if (count == 0)
				{
					return i - TStringoFindSize;
				}
			}
		}

		return npos;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void BasicString<TChar, TAllocator>::Clear()
	{
		JPT_SAFE_DELETE_ARRAY(m_pBuffer);
		m_size = 0;
		m_capacity = 0;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr BasicString<TChar, TAllocator>& BasicString<TChar, TAllocator>::Replace(const TChar* TStringoFind, const TChar* TStringoReplace, size_t startIndex, size_t endIndex)
	{
		if (endIndex == npos)
		{
			endIndex = m_size;
		}

		const BasicString<TChar> replaced(TStringoReplace);
		const size_t TStringoFindSize = GetCStrLength(TStringoFind);

		size_t foundPos = startIndex;
		while (true)
		{
			foundPos = Find(TStringoFind, startIndex, endIndex);
			if (foundPos == npos)
			{
				break;
			}

			BasicString<TChar> pre = SubStr(0, foundPos);
			BasicString<TChar> suff = SubStr(foundPos + TStringoFindSize);

			*this = Move(pre) + replaced + Move(suff);
			startIndex = foundPos + replaced.Size();	// In case 'TStringoReplace' contains 'TStringoFind', like replacing 'x' with 'yx'		
		}

		return *this;
	}

	//template<StringLiteral _TChar, class _TAllocator>
	//constexpr DynamicArray<BasicString<_TChar, _TAllocator>> BasicString<_TChar, _TAllocator>::Split(const TChar* pKeyword) const
	//{
	//	return DynamicArray<BasicString>();
	//}

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

	template<StringLiteral TChar, class TAllocator>
	constexpr void BasicString<TChar, TAllocator>::Append(const TChar* CString, size_t newStringSize)
	{
		JPT_RETURN_IF(newStringSize == 0);
		InsertStringAt(CString, m_size, newStringSize);
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void BasicString<_TChar, _TAllocator>::Append(const TChar* CString)
	{
		Append(CString, GetCStrLength(CString));
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void BasicString<TChar, TAllocator>::Append(const BasicString<TChar>& otherString)
	{
		JPT_RETURN_IF(otherString.IsEmpty());
		InsertStringAt(otherString.ConstBuffer(), m_size, otherString.m_size);
	}

	template<StringLiteral _TChar, class _TAllocator>
	constexpr void BasicString<_TChar, _TAllocator>::Append(TChar c)
	{
		TChar* cString = TAllocator::AllocateArray(2);
		cString[0] = c;
		cString[1] = '\0';
		InsertStringAt(cString, m_size, 1);
		TAllocator::DeallocateArray(cString);
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
			UpdateBuffer(capacity);
		}
	}

	template<StringLiteral _TChar, class _TAllocator>
	template<size_t kSize>
	constexpr BasicString<_TChar, _TAllocator> BasicString<_TChar, _TAllocator>::Format(const TChar* format, ...)
	{
		TChar buffer[kSize];
		FORMAT_STRING(buffer, format, ...);
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
		m_size = size;
		JPT_RETURN_IF(IsEmpty());

		UpdateBuffer(m_size);
		JPT_ASSERT(m_pBuffer, "m_pBuffer shouldn't be nullptr");
		StrCpy(m_pBuffer, m_size + sizeof(TChar), inCString);
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
		m_pBuffer  = otherString.m_pBuffer;
		m_size     = otherString.m_size;
		m_capacity = otherString.m_capacity;

		otherString.m_pBuffer  = nullptr;
		otherString.m_size     = 0;
		otherString.m_capacity = 0;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void BasicString<TChar, TAllocator>::UpdateBuffer(size_t inCapacity)
	{
		TChar* pNewBuffer = TAllocator::AllocateArray(inCapacity + sizeof(TChar));

		if (m_pBuffer)
		{
			StrCpy(pNewBuffer, m_size + sizeof(TChar), m_pBuffer);
			TAllocator::DeallocateArray(m_pBuffer);
		}

		m_pBuffer = pNewBuffer;
		m_capacity = inCapacity;
	}

	template<StringLiteral TChar, class TAllocator>
	constexpr void BasicString<TChar, TAllocator>::InsertStringAt(const TChar* CString, size_t index, size_t size)
	{
		const size_t newSize = m_size + size;
		if (newSize > m_capacity)
		{
			UpdateBuffer(newSize);
		}

		StrCpy(m_pBuffer + index, size + sizeof(TChar), CString);

		m_size = newSize;
	}

	using String = BasicString<char>;
	using WString = BasicString<wchar_t>;	// Wide string

	template<typename T>
	concept BasicStringType = IsSameType<T, String> || IsSameType<T, WString>;
}