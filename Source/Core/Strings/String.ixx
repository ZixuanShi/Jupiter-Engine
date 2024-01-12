// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Macros.h"
#include "Core/Debugging/Assert.h"

export module jpt.String;

import jpt.Allocator;
import jpt.TypeDefs;
import jpt.Concepts;
import jpt.StringUtils;
import jpt.Math;

namespace jpt
{
	/** Resizing multiplier for capacity */
	static constexpr size_t kCapacityMultiplier = 2;

	template<StringLiteral _CharT, class _AllocatorT = Allocator<_CharT>>
	class BasicString
	{
	public:
		using CharT = _CharT;
		using AllocatorT = _AllocatorT;

		static constexpr size_t npos = kInvalidValue<size_t>;

	private:
		CharT* m_pBuffer = nullptr;  /**< The pointer to the buffer representing this string's value */
		size_t m_size = 0;              /**< How many characters in this string currently */
		size_t m_capacity = 0;          /**< How many characters this string can hold before resizing */

	public:
		BasicString() = default;
		BasicString(const CharT* CString, size_t size);
		BasicString(const CharT* CString);
		BasicString(CharT c);
		BasicString(const BasicString<CharT>& otherString);
		BasicString(BasicString<CharT>&& otherString) noexcept;
		BasicString& operator=(const CharT* CString);
		BasicString& operator=(const BasicString<CharT>& otherString);
		BasicString& operator=(BasicString<CharT>&& otherString) noexcept;
		~BasicString();

		// Element Access
		const CharT* ConstBuffer() const { return m_pBuffer; }
		CharT* Buffer() const { return m_pBuffer; }
		CharT& operator[](size_t index) { return m_pBuffer[index]; }
		const CharT& operator[](size_t index) const { return m_pBuffer[index]; }
		CharT& Back() { return m_pBuffer[m_size - 1]; }
		const CharT& Back() const { return m_pBuffer[m_size - 1]; }
		CharT& Front() { return m_pBuffer[0]; }
		const CharT& Front() const { return m_pBuffer[0]; }

		// Capacity
		size_t Size() const { return m_size; }
		size_t Capacity() const { return m_capacity; }
		bool IsEmpty() const { return m_size == 0; }

		/** Searching. Returns npos if not found */
		size_t Find(CharT charToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		size_t Find(const CharT* stringToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		size_t FindFirstOf(CharT charToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		size_t FindFirstOf(const CharT* stringToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		size_t FindLastOf(CharT charToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		size_t FindLastOf(const CharT* stringToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		
		/** Comparing */
		bool operator==(const CharT* otherString) const;
		bool operator==(const BasicString<CharT>& otherString) const;

		/* Deallocate the memory that this string holds */
		void Clear();

		/* Replace the some content of this string with the new given one within a range
			@param stringToFind:	The string we want to remove and replace
			@param stringToReplace:	The string to replace the original data
			@param startIndex:		The start index to start searching. Default to 0
			@param endIndex:		The end index to stop operation. Default to size() */
		BasicString& Replace(const CharT* stringToFind, const CharT* stringToReplace, size_t startIndex = 0, size_t endIndex = npos);

		/** @return		A sub string within the given range at index and length */
		BasicString SubStr(size_t index, size_t count = npos) const;

		/** Appends a new string to the end of buffer */
		void Append(const CharT* CString, size_t newStringSize);
		void Append(const CharT* CString) { Append(CString, GetCStrLength(CString)); }
		void Append(const BasicString<CharT>& otherString);
		void Append(CharT c);
		BasicString& operator+=(const CharT* CString) { Append(CString); return *this; }
		BasicString& operator+=(const BasicString<CharT>& otherString) { Append(otherString); return *this; }
		BasicString& operator+=(CharT c) { Append(c); return *this; }
		BasicString operator+(const CharT* CString) const;
		BasicString operator+(const BasicString<CharT>& otherString) const;

		/** Pre allocate buffer with capacity's size. Preventing oftenly dynamic heap allocation */
		void Reserve(size_t capacity);

		/* Copy the content of string. Will assign the current m_pBuffer with the new copied data in memory */
		void CopyString(const CharT* inCString, size_t size);
		void CopyString(const CharT* inCString) { CopyString(inCString, GetCStrLength(inCString)); }
		void CopyString(const BasicString<CharT>& otherString);

		/* Move the content of string. Will take ownership of the passed in string */
		void MoveString(CharT* inCString, size_t size);
		void MoveString(CharT* inCString) { MoveString(inCString, GetCStrLength(inCString)); }
		void MoveString(BasicString<CharT>&& otherString);

		/** @return An integer associated with this string
			@note   Will assert fail if contains non-numeric literals besides the negative sign at the front */
		template<Integral IntT = int32>
		IntT ToInt() const { return CStrToInteger(m_pBuffer, m_size); }

		/** @return A float associated with this string
			@note   Will assert fail if contains non-numeric literals besides the negative sign at the front or the percision dot
			@note	Will ignore the 'f' is there's any */
		template<Floating FloatT = float>
		FloatT ToFloat() const { return CStrToFloat(m_pBuffer, m_size); }

	private:
		/* Called when the current buffer is not big enough to hold a new string to append. Update the buffer to a larger sizeand increase capacity
		   @param inCapacity: The capacity for the new buffer. Typically current m_size * kCapacityMultiplier */
		void UpdateBuffer(size_t inCapacity);

		/** Inserts a C-String at the index by the given size */
		void InsertStringAt(const CharT* CString, size_t index, size_t size);
	};

	template<StringLiteral CharT, class AllocatorT>
	BasicString<CharT, AllocatorT>::BasicString(const CharT* CString, size_t size)
	{
		CopyString(CString, size);
	}

	template<StringLiteral CharT, class AllocatorT>
	BasicString<CharT, AllocatorT>::BasicString(const CharT* CString)
	{
		CopyString(CString);
	}

	template<StringLiteral _CharT, class _AllocatorT>
	BasicString<_CharT, _AllocatorT>::BasicString(CharT c)
	{
		CharT* cString = AllocatorT::AllocateArray(2);
		cString[0] = c;
		cString[1] = '\0';
		MoveString(cString, 1);
	}

	template<StringLiteral CharT, class AllocatorT>
	BasicString<CharT, AllocatorT>::BasicString(const BasicString<CharT>& otherString)
	{
		CopyString(otherString);
	}

	template<StringLiteral CharT, class AllocatorT>
	BasicString<CharT, AllocatorT>::BasicString(BasicString<CharT>&& otherString) noexcept
	{
		MoveString(jpt::Move(otherString));
	}

	template<StringLiteral CharT, class AllocatorT>
	BasicString<CharT, AllocatorT>& BasicString<CharT, AllocatorT>::operator=(const CharT* CString)
	{
		if (!AreStringsSame(m_pBuffer, CString, m_size))
		{
			Clear();
			CopyString(CString);
		}

		return *this;
	}

	template<StringLiteral CharT, class AllocatorT>
	BasicString<CharT, AllocatorT>& BasicString<CharT, AllocatorT>::operator=(const BasicString<CharT>& otherString)
	{
		if (this != &otherString)
		{
			Clear();
			CopyString(otherString);
		}

		return *this;
	}

	template<StringLiteral CharT, class AllocatorT>
	BasicString<CharT, AllocatorT>& BasicString<CharT, AllocatorT>::operator=(BasicString<CharT>&& otherString) noexcept
	{
		if (this != &otherString)
		{
			Clear();
			MoveString(jpt::Move(otherString));
		}

		return *this;
	}

	template<StringLiteral CharT, class AllocatorT>
	BasicString<CharT, AllocatorT>::~BasicString()
	{
		Clear();
	}

	template<StringLiteral CharT, class AllocatorT>
	size_t BasicString<CharT, AllocatorT>::Find(CharT charToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/) const
	{
		ClampTo(endIndex, size_t(0), m_size);

		for (size_t i = startIndex; i < endIndex; ++i)
		{
			if ((i + 1) > endIndex)
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

	template<StringLiteral CharT, class AllocatorT>
	size_t BasicString<CharT, AllocatorT>::Find(const CharT* stringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		const size_t stringToFindSize = GetCStrLength(stringToFind);
		ClampTo(endIndex, size_t(0), m_size);

		BasicString<CharT> current;
		for (size_t i = startIndex; i < endIndex; ++i)
		{
			if ((i + stringToFindSize) > endIndex)
			{
				return npos;
			}

			current = SubStr(i, stringToFindSize);
			if (current == stringToFind)
			{
				return i;
			}
		}

		return npos;
	}

	template<StringLiteral CharT, class AllocatorT>
	size_t BasicString<CharT, AllocatorT>::FindFirstOf(CharT charToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		return Find(charToFind, startIndex, endIndex);
	}

	template<StringLiteral CharT, class AllocatorT>
	size_t BasicString<CharT, AllocatorT>::FindFirstOf(const CharT* stringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		return Find(stringToFind, startIndex, endIndex);
	}

	template<StringLiteral CharT, class AllocatorT>
	size_t BasicString<CharT, AllocatorT>::FindLastOf(CharT charToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		ClampTo(endIndex, size_t(0), m_size);

		for (int64 i = endIndex - 1; i >= static_cast<int64>(startIndex); --i)
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

	template<StringLiteral CharT, class AllocatorT>
	size_t BasicString<CharT, AllocatorT>::FindLastOf(const CharT* stringToFind, size_t startIndex, size_t endIndex) const
	{
		const size_t stringToFindSize = GetCStrLength(stringToFind);
		ClampTo(endIndex, size_t(0), m_size);

		BasicString<CharT> current;
		for (int64 i = endIndex - 1; i >= static_cast<int64>(startIndex); --i)
		{
			if ((i - stringToFindSize) < startIndex)
			{
				return npos;
			}

			current = SubStr(i - stringToFindSize, stringToFindSize);
			if (current == stringToFind)
			{
				return i - stringToFindSize;
			}
		}

		return npos;
	}

	template<StringLiteral CharT, class AllocatorT>
	bool BasicString<CharT, AllocatorT>::operator==(const CharT* otherString) const
	{
		return AreStringsSame(m_pBuffer, otherString, m_size);
	}

	template<StringLiteral CharT, class AllocatorT>
	bool BasicString<CharT, AllocatorT>::operator==(const BasicString<CharT>& otherString) const
	{
		return AreStringsSame(*this, otherString, m_size);
	}

	template<StringLiteral CharT, class AllocatorT>
	void BasicString<CharT, AllocatorT>::Clear()
	{
		JPT_SAFE_DELETE_ARRAY(m_pBuffer);
		m_size = 0;
		m_capacity = 0;
	}

	template<StringLiteral CharT, class AllocatorT>
	BasicString<CharT, AllocatorT>& BasicString<CharT, AllocatorT>::Replace(const CharT* stringToFind, const CharT* stringToReplace, size_t startIndex, size_t endIndex)
	{
		if (endIndex == npos)
		{
			endIndex = m_size;
		}

		const BasicString<CharT> replaced(stringToReplace);
		const size_t stringToFindSize = GetCStrLength(stringToFind);

		size_t foundPos = startIndex;
		while (true)
		{
			foundPos = Find(stringToFind, startIndex, endIndex);
			if (foundPos == npos)
			{
				break;
			}

			const BasicString<CharT> pre = SubStr(0, foundPos);
			const BasicString<CharT> suff = SubStr(foundPos + stringToFindSize);

			*this = pre + replaced + suff;
			startIndex = foundPos + replaced.Size();	// In case 'stringToReplace' contains 'stringToFind', like replacing 'x' with 'yx'		
		}

		return *this;
	}

	template<StringLiteral CharT, class AllocatorT>
	BasicString<CharT, AllocatorT> BasicString<CharT, AllocatorT>::SubStr(size_t index, size_t count /*= npos*/) const
	{
		if (count == npos)
		{
			count = m_size - index;
		}

		JPT_ASSERT((index + count) <= m_size, "SubStr cannot exceeds string's bound");
		
		if (count == 0)
		{
			return jpt::BasicString<CharT>();
		}

		CharT* subStrBuffer = AllocatorT::AllocateArray(count + sizeof(CharT));
		StrNCpy(subStrBuffer, count + sizeof(CharT), &m_pBuffer[index], count);

		jpt::BasicString<CharT> result;
		result.MoveString(subStrBuffer, count);
		return result;
	}

	template<StringLiteral CharT, class AllocatorT>
	void BasicString<CharT, AllocatorT>::Append(const CharT* CString, size_t newStringSize)
	{
		JPT_RETURN_IF(newStringSize == 0);
		InsertStringAt(CString, m_size, newStringSize);
	}

	template<StringLiteral CharT, class AllocatorT>
	void BasicString<CharT, AllocatorT>::Append(const BasicString<CharT>& otherString)
	{
		JPT_RETURN_IF(otherString.IsEmpty());
		InsertStringAt(otherString.ConstBuffer(), m_size, otherString.m_size);
	}

	template<StringLiteral _CharT, class _AllocatorT>
	void BasicString<_CharT, _AllocatorT>::Append(CharT c)
	{
		CharT* cString = AllocatorT::AllocateArray(2);
		cString[0] = c;
		cString[1] = '\0';
		InsertStringAt(cString, m_size, 1);
		AllocatorT::DeallocateArray(cString);
	}

	template<StringLiteral CharT, class AllocatorT>
	BasicString<CharT, AllocatorT> BasicString<CharT, AllocatorT>::operator+(const CharT* CString) const
	{
		BasicString<CharT> str = *this;
		str.Append(CString);
		return str;
	}

	template<StringLiteral CharT, class AllocatorT>
	BasicString<CharT, AllocatorT> BasicString<CharT, AllocatorT>::operator+(const BasicString<CharT>& otherString) const
	{
		BasicString<CharT> str = *this;
		str.Append(otherString);
		return str;
	}

	template<StringLiteral CharT, class AllocatorT>
	void BasicString<CharT, AllocatorT>::Reserve(size_t capacity)
	{
		if (capacity > m_capacity)
		{
			UpdateBuffer(capacity);
		}
	}

	template<StringLiteral CharT, class AllocatorT>
	void BasicString<CharT, AllocatorT>::CopyString(const CharT* inCString, size_t size)
	{
		m_size = size;
		JPT_RETURN_IF(IsEmpty());

		UpdateBuffer(m_size);
		JPT_ASSERT(m_pBuffer, "m_pBuffer shouldn't be nullptr");
		StrCpy(m_pBuffer, m_size + sizeof(CharT), inCString);
	}

	template<StringLiteral CharT, class AllocatorT>
	void BasicString<CharT, AllocatorT>::CopyString(const BasicString<CharT>& otherString)
	{
		m_size = otherString.m_size;
		JPT_RETURN_IF(IsEmpty());

		UpdateBuffer(m_size);
		JPT_ASSERT(m_pBuffer, "m_pBuffer shouldn't be nullptr");
		StrCpy(m_pBuffer, m_size + sizeof(CharT), otherString.ConstBuffer());
	}

	template<StringLiteral CharT, class AllocatorT>
	void BasicString<CharT, AllocatorT>::MoveString(CharT* inCString, size_t size)
	{
		m_pBuffer  = inCString;
		m_size     = size;
		m_capacity = m_size;
	}

	template<StringLiteral CharT, class AllocatorT>
	void BasicString<CharT, AllocatorT>::MoveString(BasicString<CharT>&& otherString)
	{
		m_pBuffer  = otherString.m_pBuffer;
		m_size     = otherString.m_size;
		m_capacity = otherString.m_capacity;

		otherString.m_pBuffer  = nullptr;
		otherString.m_size     = 0;
		otherString.m_capacity = 0;
	}

	template<StringLiteral CharT, class AllocatorT>
	void BasicString<CharT, AllocatorT>::UpdateBuffer(size_t inCapacity)
	{
		CharT* pNewBuffer = AllocatorT::AllocateArray(inCapacity + sizeof(CharT));

		if (m_pBuffer)
		{
			StrCpy(pNewBuffer, m_size + sizeof(CharT), m_pBuffer);
			AllocatorT::DeallocateArray(m_pBuffer);
		}

		m_pBuffer = pNewBuffer;
		m_capacity = inCapacity;
	}

	template<StringLiteral CharT, class AllocatorT>
	void BasicString<CharT, AllocatorT>::InsertStringAt(const CharT* CString, size_t index, size_t size)
	{
		const size_t newSize = m_size + size;
		if (newSize > m_capacity)
		{
			UpdateBuffer(newSize);
		}

		StrCpy(m_pBuffer + index, size + sizeof(CharT), CString);

		m_size = newSize;
	}
}

export namespace jpt
{
	using String = BasicString<char>;
	using WString = BasicString<wchar_t>;	// Wide string

	template<typename T>
	concept BasicStringType = IsSameType<T, String> || IsSameType<T, WString>;

	template<typename T>
	concept CanBeStringed = Integral<T> || Floating<T> || IsSameType<T, bool> || IsSameType<T, char>;

	template<typename T>
	concept DisabledBuiltInToString = Primitive<T> || !EnabledToString<T> && CanBeStringed<T>;

	template<typename CharT>
	BasicString<CharT> operator+(const CharT* leftString, const BasicString<CharT>& rightString)
	{
		return jpt::BasicString<CharT>(leftString) += rightString;
	}

	// Any non-primitive object that has ToString() implemented
	template<EnabledToString T>
	String ToString(const T& object)
	{
		return object.ToString();
	}

	// int, uint
	template<BasicStringType StringT = jpt::String, Integral IntT>
	StringT ToString(IntT integer)
	{
		using CharT = StringT::CharT;
		CharT* integerCString = IntegerToCStr<CharT>(integer);
		StringT integerString;
		integerString.MoveString(Move(integerCString));
		return integerString;
	}

	// float, double
	template<BasicStringType StringT = jpt::String, Floating FloatT>
	StringT ToString(FloatT value)
	{
		using CharT = StringT::CharT;
		CharT* floatCString = FloatToCStr<CharT>(value);
		StringT floatString;
		floatString.MoveString(Move(floatCString));
		return floatString;
	}

	// bool
	template<BasicStringType StringT = jpt::String>
	StringT ToString(bool value)
	{
		using CharT = StringT::CharT;

		StringT boolStr;
		if (value)
		{
			boolStr = JPT_GET_PROPER_STRING(CharT, true);
		}
		else
		{
			boolStr = JPT_GET_PROPER_STRING(CharT, false);
		}
		return boolStr;
	}

	// char
	String ToString(char c)
	{
		return String(&c);
	}
	WString ToString(wchar_t c)
	{
		return WString(&c);
	}
}