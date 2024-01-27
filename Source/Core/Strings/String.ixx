// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Macros.h"
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

import jpt_private.ContiguousIterator;

/** Resizing multiplier for capacity */
static constexpr size_t kLocCapacityMultiplier = 2;

export namespace jpt
{
	template<StringLiteral _TChar, class _AllocatorT = Allocator<_TChar>>
	class BasicString
	{
	public:
		using TChar      = _TChar;
		using AllocatorT = _AllocatorT;
		using Iterator   = jpt_private::ContiguousIterator<TChar>;

	private:
		TChar* m_pBuffer = nullptr;  /**< The pointer to the buffer representing this string's value */
		size_t m_size = 0;              /**< How many characters in this string currently */
		size_t m_capacity = 0;          /**< How many characters this string can hold before resizing */

	public:
		BasicString() = default;
		BasicString(const TChar* CString, size_t size);
		BasicString(const TChar* CString);
		BasicString(TChar c);
		BasicString(const BasicString<TChar>& otherString);
		BasicString(BasicString<TChar>&& otherString) noexcept;
		BasicString& operator=(const TChar* CString);
		BasicString& operator=(const BasicString<TChar>& otherString);
		BasicString& operator=(BasicString<TChar>&& otherString) noexcept;
		~BasicString();

		// Element Access
		const TChar* ConstBuffer() const { return m_pBuffer; }
		      TChar* Buffer()      const { return m_pBuffer; }
		      TChar& Front()       { return m_pBuffer[0]; }
		const TChar& Front() const { return m_pBuffer[0]; }
		      TChar& Back()        { return m_pBuffer[m_size - 1]; }
		const TChar& Back()  const { return m_pBuffer[m_size - 1]; }
		      TChar& operator[](size_t index)       { return m_pBuffer[index]; }
		const TChar& operator[](size_t index) const { return m_pBuffer[index]; }

		// Iterators
		      Iterator begin()        { return Iterator(m_pBuffer); }
		const Iterator begin()  const { return Iterator(m_pBuffer); }
		const Iterator cbegin() const { return Iterator(m_pBuffer); }

		      Iterator end()        { return Iterator(m_pBuffer + m_size); }
		const Iterator end()  const { return Iterator(m_pBuffer + m_size); }
		const Iterator cend() const { return Iterator(m_pBuffer + m_size); }

		// Capacity
		size_t Size()     const { return m_size; }
		size_t Capacity() const { return m_capacity; }
		bool   IsEmpty()  const { return m_size == 0; }

		/** Searching. Returns npos if not found */
		size_t Find(TChar TCharoFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		size_t Find(const TChar* stringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		size_t FindFirstOf(TChar TCharoFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		size_t FindFirstOf(const TChar* stringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		size_t FindLastOf(TChar TCharoFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		size_t FindLastOf(const TChar* stringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		bool Contains(TChar TCharoFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(TCharoFind, startIndex, endIndex, count) != npos; }
		bool Contains(const TChar* TCharoFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(TCharoFind, startIndex, endIndex, count) != npos; }
		
		/* Deallocate the memory that this string holds */
		void Clear();

		/* Replace the some content of this string with the new given one within optional range
			@param stringToFind:	The string we want to remove and replace
			@param stringToReplace:	The string to replace the original data
			@param startIndex:		[optional] The start index to start searching. Default to 0
			@param endIndex:		[optional] The end index to stop operation. Default to size() */
		BasicString& Replace(const TChar* stringToFind, const TChar* stringToReplace, size_t startIndex = 0, size_t endIndex = npos);

		/** @return		A sub string within the given range at index and length */
		BasicString SubStr(size_t index, size_t count = npos) const;

		/** Appends a new string to the end of buffer */
		void Append(const TChar* CString, size_t newStringSize);
		void Append(const TChar* CString) { Append(CString, GetCStrLength(CString)); }
		void Append(const BasicString<TChar>& otherString);
		void Append(TChar c);
		BasicString& operator+=(const TChar* CString) { Append(CString); return *this; }
		BasicString& operator+=(const BasicString<TChar>& otherString) { Append(otherString); return *this; }
		BasicString& operator+=(TChar c) { Append(c); return *this; }

		/** Pre allocate buffer with capacity's size. Preventing oftenly dynamic heap allocation */
		void Reserve(size_t capacity);

		template<size_t kSize>
		static BasicString Format(const TChar* format, ...);

		/* Copy the content of string. Will assign the current m_pBuffer with the new copied data in memory */
		void CopyString(const TChar* inCString, size_t size);
		void CopyString(const TChar* inCString) { CopyString(inCString, GetCStrLength(inCString)); }
		void CopyString(const BasicString<TChar>& otherString) { CopyString(otherString.ConstBuffer(), otherString.Size()); }

		/* Move the content of string. Will take ownership of the passed in string */
		void MoveString(TChar* inCString, size_t size);
		void MoveString(TChar* inCString) { MoveString(inCString, GetCStrLength(inCString)); }
		void MoveString(BasicString<TChar>&& otherString);

		/** @return An integer associated with this string
			@note   Will assert fail if contains non-numeric literals besides the negative sign at the front */
		template<Integral TInt = int32>
		TInt ToInt() const { return CStrToInteger(m_pBuffer, m_size); }

		/** @return A float associated with this string
			@note   Will assert fail if contains non-numeric literals besides the negative sign at the front or the percision dot
			@note	Will ignore the 'f' is there's any */
		template<Floating TFloat = float>
		TFloat ToFloat() const { return CStrToFloat(m_pBuffer, m_size); }

	private:
		/* Called when the current buffer is not big enough to hold a new string to append. Update the buffer to a larger sizeand increase capacity
		   @param inCapacity: The capacity for the new buffer. Typically current m_size * kCapacityMultiplier */
		void UpdateBuffer(size_t inCapacity);

		/** Inserts a C-String at the index by the given size */
		void InsertStringAt(const TChar* CString, size_t index, size_t size);
	};

	// Non member functions -------------------------------------------------------------------------------------------------------------------
	template<StringLiteral TChar, class AllocatorT>
	BasicString<TChar, AllocatorT> operator+(const BasicString<TChar, AllocatorT>& string, const TChar* CString)
	{
		BasicString<TChar> str = string;
		str.Append(CString);
		return str;
	}

	template<StringLiteral TChar, class AllocatorT>
	BasicString<TChar, AllocatorT> operator+(BasicString<TChar, AllocatorT>&& string, const TChar* CString)
	{
		BasicString<TChar> str = Move(string);
		str.Append(CString);
		return str;
	}

	template<StringLiteral TChar, class AllocatorT>
	BasicString<TChar, AllocatorT> operator+(const BasicString<TChar, AllocatorT>& string, const BasicString<TChar>& otherString)
	{
		BasicString<TChar> str = string;
		str.Append(otherString);
		return str;
	}

	template<StringLiteral TChar, class AllocatorT>
	BasicString<TChar, AllocatorT> operator+(BasicString<TChar, AllocatorT>&& string, const BasicString<TChar>& otherString)
	{
		BasicString<TChar> str = Move(string);
		str.Append(otherString);
		return str;
	}

	template<StringLiteral TChar>
	BasicString<TChar> operator+(const TChar* leftString, const BasicString<TChar>& rightString)
	{
		return jpt::BasicString<TChar>(leftString) += rightString;
	}

	template<StringLiteral TChar>
	BasicString<TChar> operator+(const TChar* leftString, BasicString<TChar>&& rightString)
	{
		return jpt::BasicString<TChar>(leftString) += Move(rightString);
	}

	template<StringLiteral TChar, class AllocatorT>
	bool operator==(const BasicString<TChar, AllocatorT>& string, const TChar* otherString)
	{
		return AreStringsSame(string.ConstBuffer(), otherString, string.Size());
	}

	template<StringLiteral TChar, class AllocatorT>
	bool operator==(const BasicString<TChar, AllocatorT>& string, const BasicString<TChar>& otherString)
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
	template<StringLiteral TChar, class AllocatorT>
	BasicString<TChar, AllocatorT>::BasicString(const TChar* CString, size_t size)
	{
		CopyString(CString, size);
	}

	template<StringLiteral TChar, class AllocatorT>
	BasicString<TChar, AllocatorT>::BasicString(const TChar* CString)
	{
		CopyString(CString);
	}

	template<StringLiteral _TChar, class _AllocatorT>
	BasicString<_TChar, _AllocatorT>::BasicString(TChar c)
	{
		TChar* cString = AllocatorT::AllocateArray(2);
		cString[0] = c;
		cString[1] = '\0';
		MoveString(cString, 1);
	}

	template<StringLiteral TChar, class AllocatorT>
	BasicString<TChar, AllocatorT>::BasicString(const BasicString<TChar>& otherString)
	{
		CopyString(otherString);
	}

	template<StringLiteral TChar, class AllocatorT>
	BasicString<TChar, AllocatorT>::BasicString(BasicString<TChar>&& otherString) noexcept
	{
		MoveString(Move(otherString));
	}

	template<StringLiteral TChar, class AllocatorT>
	BasicString<TChar, AllocatorT>& BasicString<TChar, AllocatorT>::operator=(const TChar* CString)
	{
		if (!AreStringsSame(m_pBuffer, CString, m_size, GetCStrLength(CString)))
		{
			Clear();
			CopyString(CString);
		}

		return *this;
	}

	template<StringLiteral TChar, class AllocatorT>
	BasicString<TChar, AllocatorT>& BasicString<TChar, AllocatorT>::operator=(const BasicString<TChar>& otherString)
	{
		if (this != &otherString)
		{
			Clear();
			CopyString(otherString);
		}

		return *this;
	}

	template<StringLiteral TChar, class AllocatorT>
	BasicString<TChar, AllocatorT>& BasicString<TChar, AllocatorT>::operator=(BasicString<TChar>&& otherString) noexcept
	{
		if (this != &otherString)
		{
			Clear();
			MoveString(Move(otherString));
		}

		return *this;
	}

	template<StringLiteral TChar, class AllocatorT>
	BasicString<TChar, AllocatorT>::~BasicString()
	{
		Clear();
	}

	template<StringLiteral TChar, class AllocatorT>
	size_t BasicString<TChar, AllocatorT>::Find(TChar TCharoFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
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

	template<StringLiteral TChar, class AllocatorT>
	size_t BasicString<TChar, AllocatorT>::Find(const TChar* stringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		const size_t stringToFindSize = GetCStrLength(stringToFind);
		ClampTo(endIndex, static_cast<size_t>(0), m_size);

		BasicString<TChar> current;
		for (size_t i = startIndex; i < endIndex; ++i)
		{
			if ((i + stringToFindSize) > endIndex)
			{
				return npos;
			}

			current = SubStr(i, stringToFindSize);
			if (current == stringToFind)
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

	template<StringLiteral TChar, class AllocatorT>
	size_t BasicString<TChar, AllocatorT>::FindFirstOf(TChar TCharoFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		return Find(TCharoFind, startIndex, endIndex);
	}

	template<StringLiteral TChar, class AllocatorT>
	size_t BasicString<TChar, AllocatorT>::FindFirstOf(const TChar* stringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		return Find(stringToFind, startIndex, endIndex);
	}

	template<StringLiteral TChar, class AllocatorT>
	size_t BasicString<TChar, AllocatorT>::FindLastOf(TChar TCharoFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
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

	template<StringLiteral TChar, class AllocatorT>
	size_t BasicString<TChar, AllocatorT>::FindLastOf(const TChar* stringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		const size_t stringToFindSize = GetCStrLength(stringToFind);
		ClampTo(endIndex, size_t(0), m_size);

		BasicString<TChar> current;
		for (int64 i = endIndex - 1; i >= static_cast<int64>(startIndex); --i)
		{
			if ((i - stringToFindSize) < startIndex)
			{
				return npos;
			}

			current = SubStr(i - stringToFindSize, stringToFindSize);
			if (current == stringToFind)
			{
				--count;
				if (count == 0)
				{
					return i - stringToFindSize;
				}
			}
		}

		return npos;
	}

	template<StringLiteral TChar, class AllocatorT>
	void BasicString<TChar, AllocatorT>::Clear()
	{
		JPT_SAFE_DELETE_ARRAY(m_pBuffer);
		m_size = 0;
		m_capacity = 0;
	}

	template<StringLiteral TChar, class AllocatorT>
	BasicString<TChar, AllocatorT>& BasicString<TChar, AllocatorT>::Replace(const TChar* stringToFind, const TChar* stringToReplace, size_t startIndex, size_t endIndex)
	{
		if (endIndex == npos)
		{
			endIndex = m_size;
		}

		const BasicString<TChar> replaced(stringToReplace);
		const size_t stringToFindSize = GetCStrLength(stringToFind);

		size_t foundPos = startIndex;
		while (true)
		{
			foundPos = Find(stringToFind, startIndex, endIndex);
			if (foundPos == npos)
			{
				break;
			}

			BasicString<TChar> pre = SubStr(0, foundPos);
			BasicString<TChar> suff = SubStr(foundPos + stringToFindSize);

			*this = Move(pre) + replaced + Move(suff);
			startIndex = foundPos + replaced.Size();	// In case 'stringToReplace' contains 'stringToFind', like replacing 'x' with 'yx'		
		}

		return *this;
	}

	template<StringLiteral TChar, class AllocatorT>
	BasicString<TChar, AllocatorT> BasicString<TChar, AllocatorT>::SubStr(size_t index, size_t count /*= npos*/) const
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

		TChar* subStrBuffer = AllocatorT::AllocateArray(count + sizeof(TChar));
		StrNCpy(subStrBuffer, count + sizeof(TChar), &m_pBuffer[index], count);

		jpt::BasicString<TChar> result;
		result.MoveString(subStrBuffer, count);
		return result;
	}

	template<StringLiteral TChar, class AllocatorT>
	void BasicString<TChar, AllocatorT>::Append(const TChar* CString, size_t newStringSize)
	{
		JPT_RETURN_IF(newStringSize == 0);
		InsertStringAt(CString, m_size, newStringSize);
	}

	template<StringLiteral TChar, class AllocatorT>
	void BasicString<TChar, AllocatorT>::Append(const BasicString<TChar>& otherString)
	{
		JPT_RETURN_IF(otherString.IsEmpty());
		InsertStringAt(otherString.ConstBuffer(), m_size, otherString.m_size);
	}

	template<StringLiteral _TChar, class _AllocatorT>
	void BasicString<_TChar, _AllocatorT>::Append(TChar c)
	{
		TChar* cString = AllocatorT::AllocateArray(2);
		cString[0] = c;
		cString[1] = '\0';
		InsertStringAt(cString, m_size, 1);
		AllocatorT::DeallocateArray(cString);
	}

	template<StringLiteral TChar, class AllocatorT>
	void BasicString<TChar, AllocatorT>::Reserve(size_t capacity)
	{
		if (capacity > m_capacity)
		{
			UpdateBuffer(capacity);
		}
	}

	template<StringLiteral _TChar, class _AllocatorT>
	template<size_t kSize>
	BasicString<_TChar, _AllocatorT> BasicString<_TChar, _AllocatorT>::Format(const TChar* format, ...)
	{
		TChar buffer[kSize];
		FORMAT_STRING(buffer, format, ...);
		return BasicString(buffer);
	}

	template<StringLiteral TChar, class AllocatorT>
	void BasicString<TChar, AllocatorT>::CopyString(const TChar* inCString, size_t size)
	{
		m_size = size;
		JPT_RETURN_IF(IsEmpty());

		UpdateBuffer(m_size);
		JPT_ASSERT(m_pBuffer, "m_pBuffer shouldn't be nullptr");
		StrCpy(m_pBuffer, m_size + sizeof(TChar), inCString);
	}

	template<StringLiteral TChar, class AllocatorT>
	void BasicString<TChar, AllocatorT>::MoveString(TChar* inCString, size_t size)
	{
		m_pBuffer  = inCString;
		m_size     = size;
		m_capacity = m_size;
	}

	template<StringLiteral TChar, class AllocatorT>
	void BasicString<TChar, AllocatorT>::MoveString(BasicString<TChar>&& otherString)
	{
		m_pBuffer  = otherString.m_pBuffer;
		m_size     = otherString.m_size;
		m_capacity = otherString.m_capacity;

		otherString.m_pBuffer  = nullptr;
		otherString.m_size     = 0;
		otherString.m_capacity = 0;
	}

	template<StringLiteral TChar, class AllocatorT>
	void BasicString<TChar, AllocatorT>::UpdateBuffer(size_t inCapacity)
	{
		TChar* pNewBuffer = AllocatorT::AllocateArray(inCapacity + sizeof(TChar));

		if (m_pBuffer)
		{
			StrCpy(pNewBuffer, m_size + sizeof(TChar), m_pBuffer);
			AllocatorT::DeallocateArray(m_pBuffer);
		}

		m_pBuffer = pNewBuffer;
		m_capacity = inCapacity;
	}

	template<StringLiteral TChar, class AllocatorT>
	void BasicString<TChar, AllocatorT>::InsertStringAt(const TChar* CString, size_t index, size_t size)
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