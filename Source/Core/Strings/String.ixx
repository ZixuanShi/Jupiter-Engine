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

	template<StringLiteral _CharType, class _AllocatorType = Allocator<_CharType>>
	class BasicString
	{
	public:
		using CharType = _CharType;
		using AllocatorType = _AllocatorType;

		static constexpr size_t npos = kInvalidValue<size_t>;

	private:
		CharType* m_pBuffer = nullptr;  /**< The pointer to the buffer representing this string's value */
		size_t m_size = 0;              /**< How many characters in this string currently */
		size_t m_capacity = 0;          /**< How many characters this string can hold before resizing */

	public:
		BasicString() = default;
		BasicString(const CharType* CString, size_t size);
		BasicString(const CharType* CString);
		BasicString(const BasicString<CharType>& otherString);
		BasicString(BasicString<CharType>&& otherString) noexcept;
		BasicString& operator=(const CharType* CString);
		BasicString& operator=(const BasicString<CharType>& otherString);
		BasicString& operator=(BasicString<CharType>&& otherString) noexcept;
		~BasicString();

		// Element Access
		const CharType* ConstBuffer() const { return m_pBuffer; }
		CharType* Buffer() const { return m_pBuffer; }
		CharType& operator[](size_t index) { return m_pBuffer[index]; }
		const CharType& operator[](size_t index) const { return m_pBuffer[index]; }
		CharType& Back() { return m_pBuffer[m_size - 1]; }
		const CharType& Back() const { return m_pBuffer[m_size - 1]; }
		CharType& Front() { return m_pBuffer[0]; }
		const CharType& Front() const { return m_pBuffer[0]; }

		// Capacity
		size_t Size() const { return m_size; }
		size_t Capacity() const { return m_capacity; }
		bool IsEmpty() const { return m_size == 0; }

		/** Searching. Returns npos if not found */
		size_t Find(CharType charToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		size_t Find(const CharType* stringToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		size_t FindFirstOf(CharType charToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		size_t FindFirstOf(const CharType* stringToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		size_t FindLastOf(CharType charToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		size_t FindLastOf(const CharType* stringToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		
		/** Comparing */
		bool operator==(const CharType* otherString) const;
		bool operator==(const BasicString<CharType>& otherString) const;

		/* Deallocate the memory that this string holds */
		void Clear();

		/* Replace the some content of this string with the new given one within a range
			@param stringToFind:	The string we want to remove and replace
			@param stringToReplace:	The string to replace the original data
			@param startIndex:		The start index to start searching. Default to 0
			@param endIndex:		The end index to stop operation. Default to size() */
		BasicString& Replace(const CharType* stringToFind, const CharType* stringToReplace, size_t startIndex = 0, size_t endIndex = npos);

		/** @return		A sub string within the given range at index and length */
		BasicString SubStr(size_t index, size_t count = npos) const;

		/** Appends a new string to the end of buffer */
		void Append(const CharType* CString, size_t newStringSize);
		void Append(const CharType* CString) { Append(CString, GetCStrLength(CString)); }
		void Append(const BasicString<CharType>& otherString);
		BasicString& operator+=(const CharType* CString) { Append(CString); return *this; }
		BasicString& operator+=(const BasicString<CharType>& otherString) { Append(otherString); return *this; }
		BasicString operator+(const CharType* CString) const;
		BasicString operator+(const BasicString<CharType>& otherString) const;

		/** Pre allocate buffer with capacity's size. Preventing oftenly dynamic heap allocation */
		void Reserve(size_t capacity);

		/* Copy the content of string. Will assign the current m_pBuffer with the new copied data in memory */
		void CopyString(const CharType* inCString, size_t size);
		void CopyString(const CharType* inCString) { CopyString(inCString, GetCStrLength(inCString)); }
		void CopyString(const BasicString<CharType>& otherString);

		/* Move the content of string. Will take ownership of the passed in string */
		void MoveString(CharType* inCString, size_t size);
		void MoveString(CharType* inCString) { MoveString(inCString, GetCStrLength(inCString)); }
		void MoveString(BasicString<CharType>&& otherString);

		/** @return An integer associated with this string
			@note   Will assert fail if contains non-numeric literals besides the negative sign at the front */
		template<Integral NumType = int32>
		NumType ToInt() const { return CStrToInteger(m_pBuffer, m_size); }

		/** @return A float associated with this string
			@note   Will assert fail if contains non-numeric literals besides the negative sign at the front or the percision dot
			@note	Will ignore the 'f' is there's any */
		template<Floating NumType = float>
		NumType ToFloat() const { return CStrToFloat(m_pBuffer, m_size); }

	private:
		/* Called when the current buffer is not big enough to hold a new string to append. Update the buffer to a larger sizeand increase capacity
		   @param inCapacity: The capacity for the new buffer. Typically current m_size * kCapacityMultiplier */
		void UpdateBuffer(size_t inCapacity);

		void InsertStringAt(size_t index, size_t size, const CharType* CString);
	};

	template<StringLiteral _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType>::BasicString(const CharType* CString, size_t size)
	{
		CopyString(CString, size);
	}

	template<StringLiteral _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType>::BasicString(const CharType* CString)
	{
		CopyString(CString);
	}

	template<StringLiteral _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType>::BasicString(const BasicString<CharType>& otherString)
	{
		CopyString(otherString);
	}

	template<StringLiteral _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType>::BasicString(BasicString<CharType>&& otherString) noexcept
	{
		MoveString(jpt::move(otherString));
	}

	template<StringLiteral _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType>& BasicString<_CharType, _AllocatorType>::operator=(const CharType* CString)
	{
		if (!AreStringsSame(m_pBuffer, CString, m_size))
		{
			Clear();
			CopyString(CString);
		}

		return *this;
	}

	template<StringLiteral _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType>& BasicString<_CharType, _AllocatorType>::operator=(const BasicString<CharType>& otherString)
	{
		if (this != &otherString)
		{
			Clear();
			CopyString(otherString);
		}

		return *this;
	}

	template<StringLiteral _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType>& BasicString<_CharType, _AllocatorType>::operator=(BasicString<CharType>&& otherString) noexcept
	{
		if (this != &otherString)
		{
			Clear();
			MoveString(jpt::move(otherString));
		}

		return *this;
	}

	template<StringLiteral _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType>::~BasicString()
	{
		Clear();
	}

	template<StringLiteral _CharType, class _AllocatorType>
	size_t BasicString<_CharType, _AllocatorType>::Find(CharType charToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/) const
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

	template<StringLiteral _CharType, class _AllocatorType>
	size_t BasicString<_CharType, _AllocatorType>::Find(const CharType* stringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		const size_t stringToFindSize = GetCStrLength(stringToFind);
		ClampTo(endIndex, size_t(0), m_size);

		BasicString<CharType> current;
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

	template<StringLiteral _CharType, class _AllocatorType>
	size_t BasicString<_CharType, _AllocatorType>::FindFirstOf(CharType charToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		return Find(charToFind, startIndex, endIndex);
	}

	template<StringLiteral _CharType, class _AllocatorType>
	size_t BasicString<_CharType, _AllocatorType>::FindFirstOf(const CharType* stringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		return Find(stringToFind, startIndex, endIndex);
	}

	template<StringLiteral _CharType, class _AllocatorType>
	size_t BasicString<_CharType, _AllocatorType>::FindLastOf(CharType charToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
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

	template<StringLiteral _CharType, class _AllocatorType>
	size_t BasicString<_CharType, _AllocatorType>::FindLastOf(const CharType* stringToFind, size_t startIndex, size_t endIndex) const
	{
		const size_t stringToFindSize = GetCStrLength(stringToFind);
		ClampTo(endIndex, size_t(0), m_size);

		BasicString<CharType> current;
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

	template<StringLiteral _CharType, class _AllocatorType>
	bool BasicString<_CharType, _AllocatorType>::operator==(const CharType* otherString) const
	{
		return AreStringsSame(m_pBuffer, otherString, m_size);
	}

	template<StringLiteral _CharType, class _AllocatorType>
	bool BasicString<_CharType, _AllocatorType>::operator==(const BasicString<CharType>& otherString) const
	{
		return AreStringsSame(*this, otherString, m_size);
	}

	template<StringLiteral _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::Clear()
	{
		JPT_SAFE_DELETE_ARRAY(m_pBuffer);
		m_size = 0;
		m_capacity = 0;
	}

	template<StringLiteral _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType>& BasicString<_CharType, _AllocatorType>::Replace(const CharType* stringToFind, const CharType* stringToReplace, size_t startIndex, size_t endIndex)
	{
		if (endIndex == npos)
		{
			endIndex = m_size;
		}

		const BasicString<CharType> replaced(stringToReplace);
		const size_t stringToFindSize = GetCStrLength(stringToFind);

		size_t foundPos = startIndex;
		while (true)
		{
			foundPos = Find(stringToFind, startIndex, endIndex);
			if (foundPos == npos)
			{
				break;
			}

			const BasicString<CharType> pre = SubStr(0, foundPos);
			const BasicString<CharType> suff = SubStr(foundPos + stringToFindSize);

			*this = pre + replaced + suff;
			startIndex = foundPos + replaced.Size();	// In case 'stringToReplace' contains 'stringToFind', like replacing 'x' with 'yx'		
		}

		return *this;
	}

	template<StringLiteral _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType> BasicString<_CharType, _AllocatorType>::SubStr(size_t index, size_t count /*= npos*/) const
	{
		if (count == npos)
		{
			count = m_size - index;
		}

		JPT_ASSERT((index + count) <= m_size, "SubStr cannot exceeds string's bound");
		
		if (count == 0)
		{
			return jpt::BasicString<CharType>();
		}

		CharType* subStrBuffer = AllocatorType::AllocateArray(count + sizeof(CharType));
		StrNCpy(subStrBuffer, count + sizeof(CharType), &m_pBuffer[index], count);

		jpt::BasicString<CharType> result;
		result.MoveString(subStrBuffer, count);
		return result;
	}

	template<StringLiteral _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::Append(const CharType* CString, size_t newStringSize)
	{
		JPT_RETURN_IF(newStringSize == 0);
		InsertStringAt(m_size, newStringSize, CString);
	}

	template<StringLiteral _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::Append(const BasicString<CharType>& otherString)
	{
		JPT_RETURN_IF(otherString.IsEmpty());
		InsertStringAt(m_size, otherString.m_size, otherString.ConstBuffer());
	}

	template<StringLiteral _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType> BasicString<_CharType, _AllocatorType>::operator+(const CharType* CString) const
	{
		BasicString<CharType> str = *this;
		str.Append(CString);
		return str;
	}

	template<StringLiteral _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType> BasicString<_CharType, _AllocatorType>::operator+(const BasicString<CharType>& otherString) const
	{
		BasicString<CharType> str = *this;
		str.Append(otherString);
		return str;
	}

	template<StringLiteral _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::Reserve(size_t capacity)
	{
		if (capacity > m_capacity)
		{
			UpdateBuffer(capacity);
		}
	}

	template<StringLiteral _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::CopyString(const CharType* inCString, size_t size)
	{
		m_size = size;
		JPT_RETURN_IF(IsEmpty());

		UpdateBuffer(m_size);
		JPT_ASSERT(m_pBuffer, "m_pBuffer shouldn't be nullptr");
		StrCpy(m_pBuffer, m_size + sizeof(CharType), inCString);
	}

	template<StringLiteral _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::CopyString(const BasicString<CharType>& otherString)
	{
		m_size = otherString.m_size;
		JPT_RETURN_IF(IsEmpty());

		UpdateBuffer(m_size);
		JPT_ASSERT(m_pBuffer, "m_pBuffer shouldn't be nullptr");
		StrCpy(m_pBuffer, m_size + sizeof(CharType), otherString.ConstBuffer());
	}

	template<StringLiteral _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::MoveString(CharType* inCString, size_t size)
	{
		m_pBuffer  = inCString;
		m_size     = size;
		m_capacity = m_size;
	}

	template<StringLiteral _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::MoveString(BasicString<CharType>&& otherString)
	{
		m_pBuffer  = otherString.m_pBuffer;
		m_size     = otherString.m_size;
		m_capacity = otherString.m_capacity;

		otherString.m_pBuffer  = nullptr;
		otherString.m_size     = 0;
		otherString.m_capacity = 0;
	}

	template<StringLiteral _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::UpdateBuffer(size_t inCapacity)
	{
		CharType* pNewBuffer = AllocatorType::AllocateArray(inCapacity + sizeof(CharType));

		if (m_pBuffer)
		{
			StrCpy(pNewBuffer, m_size + sizeof(CharType), m_pBuffer);
			AllocatorType::DeallocateArray(m_pBuffer);
		}

		m_pBuffer = pNewBuffer;
		m_capacity = inCapacity;
	}

	template<StringLiteral _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::InsertStringAt(size_t index, size_t size, const CharType* CString)
	{
		const size_t newSize = m_size + size;
		if (newSize > m_capacity)
		{
			UpdateBuffer(newSize);
		}

		StrCpy(m_pBuffer + index, size + sizeof(CharType), CString);

		m_size = newSize;
	}
}

export namespace jpt
{
	using String  = BasicString<char>;
	using WString = BasicString<wchar_t>;	// Wide string

	template<typename CharType>
	BasicString<CharType> operator+(const CharType* leftString, const BasicString<CharType>& rightString)
	{
		return jpt::BasicString<CharType>(leftString) += rightString;
	}

	template<EnabledToString Type>
	String ToString(const Type& object)
	{
		return object.ToString();
	}

	template<class StringType = jpt::String, Integral Type>
	StringType ToString(Type integer)
	{
		using CharType = StringType::CharType;
		CharType* integerCString = IntegerToCStr<CharType>(integer);
		StringType integerString;
		integerString.MoveString(move(integerCString));
		return integerString;
	}

	template<class StringType = jpt::String, Floating Type>
	StringType ToString(Type value)
	{
		using CharType = StringType::CharType;
		CharType* floatCString = FloatingToCStr<CharType>(value);
		StringType floatString;
		floatString.MoveString(move(floatCString));
		return floatString;
	}
}