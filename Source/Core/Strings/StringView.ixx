// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.StringView;

import jpt.Allocator;
import jpt.Concepts;
import jpt.Constants;
import jpt.Math;
import jpt.String;
import jpt.StringUtils;
import jpt.TypeDefs;

export namespace jpt
{
	/** Describes an object that can refer to a constant contiguous sequence 
		of CharT with the first element of the sequence at position zero. */
	template<StringLiteral _CharT>
	class BasicStringView
	{
	public:
		using CharT = _CharT;

	private:
		const CharT* m_pBuffer = nullptr;
		size_t m_size = 0;

	public:
		// Constructors
		constexpr BasicStringView() = default;
		constexpr BasicStringView(const CharT* CString, size_t size);
		constexpr BasicStringView(const CharT* CString);
		BasicStringView(const BasicStringView& other);
		BasicStringView(const BasicString<CharT>& string);

		// operator=
		BasicStringView& operator=(const CharT* CString);
		BasicStringView& operator=(const BasicStringView& other);
		BasicStringView& operator=(const BasicString<CharT>& string);

		// Element Access
		const CharT* Buffer() const { return m_pBuffer; }
		const CharT& operator[](size_t index) const { return m_pBuffer[index]; }
		const CharT& At(size_t index) const { return m_pBuffer[index]; }
		const CharT& Front() const { return m_pBuffer[0]; }
		const CharT& Back() const { return m_pBuffer[m_size - 1]; }

		// Capacity
		bool IsEmpty() const { return m_size == 0; }
		size_t Size() const { return m_size; }

		// Operations

		/** @return		A sub string within the given range at index and length */
		BasicStringView SubStr(size_t index, size_t count = npos) const;
	};

	// Non member functions -------------------------------------------------------------------------------------------------------------------
	template<StringLiteral CharT>
	bool operator==(const BasicStringView<CharT>& a, const BasicStringView<CharT>& b)
	{
		return AreStringsSame(a.Buffer(), b.Buffer(), a.Size(), b.Size());
	}
	template<StringLiteral CharT>
	bool operator==(const BasicStringView<CharT>& a, const CharT* b)
	{
		return AreStringsSame(a.Buffer(), b, a.Size(), GetCStrLength(b));
	}
	template<StringLiteral CharT>
	bool operator==(const BasicStringView<CharT>& a, const BasicString<CharT>& b)
	{
		return AreStringsSame(a.Buffer(), b.ConstBuffer(), a.Size(), b.Size());
	}

	// Member Functions Definitions --------------------------------------------------------------------------------------------------------
	template<StringLiteral CharT>
	constexpr BasicStringView<CharT>::BasicStringView(const CharT* CString, size_t size)
		: m_pBuffer(CString)
		, m_size(size)
	{
	}

	template<StringLiteral CharT>
	constexpr BasicStringView<CharT>::BasicStringView(const CharT* CString)
		: BasicStringView(CString, GetCStrLength(CString))
	{
	}

	template<StringLiteral _CharT>
	BasicStringView<_CharT>::BasicStringView(const BasicStringView& other)
		: m_pBuffer(other.m_pBuffer)
		, m_size(other.m_size)
	{
	}

	template<StringLiteral CharT>
	BasicStringView<CharT>::BasicStringView(const BasicString<CharT>& string)
		: m_pBuffer(string.ConstBuffer())
		, m_size(string.Size())
	{
	}

	template<StringLiteral CharT>
	BasicStringView<CharT>& BasicStringView<CharT>::operator=(const CharT* CString)
	{
		if (m_pBuffer != CString)
		{
			m_pBuffer = CString;
			m_size = GetCStrLength(CString);
		}

		return *this;
	}

	template<StringLiteral CharT>
	BasicStringView<CharT>& BasicStringView<CharT>::operator=(const BasicStringView& other)
	{
		if (this != &other)
		{
			m_pBuffer = other.m_pBuffer;
			m_size = other.m_size;
		}

		return *this;
	}

	template<StringLiteral CharT>
	BasicStringView<CharT>& BasicStringView<CharT>::operator=(const BasicString<CharT>& string)
	{
		if (m_pBuffer != string.ConstBuffer())
		{
			m_pBuffer = string.ConstBuffer();
			m_size = string.Size();
		}

		return *this;
	}

	template<StringLiteral CharT>
	BasicStringView<CharT> BasicStringView<CharT>::SubStr(size_t index, size_t count) const
	{
		if (count == npos)
		{
			count = m_size - index;
		}

		JPT_ASSERT((index + count) <= m_size, "SubStr cannot exceeds string's bound");

		return BasicStringView(m_pBuffer + index, count);
	}

	using StringView = BasicStringView<char>;
	using WStringView = BasicStringView<wchar_t>;
}