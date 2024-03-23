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
		of TChar with the first element of the sequence at position zero. */
	template<StringLiteral _TChar>
	class BasicStringView
	{
	public:
		using TChar = _TChar;

	private:
		const TChar* m_pBuffer = nullptr;
		size_t m_size = 0;

	public:
		// Constructors
		constexpr BasicStringView() = default;
		constexpr BasicStringView(const TChar* CString, size_t size);
		constexpr BasicStringView(const TChar* CString);
		constexpr BasicStringView(const BasicStringView& other);
		constexpr BasicStringView(const StringBase<TChar>& string);

		// operator=
		BasicStringView& operator=(const TChar* CString);
		BasicStringView& operator=(const BasicStringView& other);
		BasicStringView& operator=(const StringBase<TChar>& string);

		// Element Access
		constexpr const TChar* Buffer()         const { return m_pBuffer;             } 
		constexpr const TChar& At(size_t index) const { return m_pBuffer[index];      }
		constexpr const TChar& Front()          const { return m_pBuffer[0];          }
		constexpr const TChar& Back()           const { return m_pBuffer[m_size - 1]; }
		constexpr const TChar& operator[](size_t index) const { return m_pBuffer[index]; }

		// Capacity
		constexpr bool IsEmpty() const { return m_size == 0; }
		constexpr size_t Size()  const { return m_size;      }

		// Operations

		/** @return		A sub string within the given range at index and length */
		constexpr BasicStringView SubStr(size_t index, size_t count = npos) const;

		/** @return     true if the string view starts with the given prefix */
		constexpr bool StartsWith(const TChar* CString, size_t count) const;
		constexpr bool StartsWith(const TChar* CString) const { return StartsWith(CString, GetCStrLength(CString)); }

		/** @return     true if the string view ends with the given suffix */
		constexpr bool EndsWith(const TChar* CString, size_t count) const;
		constexpr bool EndsWith(const TChar* CString) const { return EndsWith(CString, GetCStrLength(CString)); }

		constexpr size_t Find(TChar TCharoFind, size_t startIndex = 0, size_t endIndex = npos) const;
		constexpr size_t Find(const TChar* pStringToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		constexpr bool Contains(TChar TCharoFind, size_t startIndex = 0, size_t endIndex = npos) const { return Find(TCharoFind, startIndex, endIndex) != npos; }
		constexpr bool Contains(const TChar* pStringToFind, size_t startIndex = 0, size_t endIndex = npos) const { return Find(pStringToFind, startIndex, endIndex) != npos; }
	};

	// Non member functions -------------------------------------------------------------------------------------------------------------------
	template<StringLiteral TChar>
	constexpr bool operator==(const BasicStringView<TChar>& a, const BasicStringView<TChar>& b)
	{
		return AreStringsSame(a.Buffer(), b.Buffer(), a.Size(), b.Size());
	}
	template<StringLiteral TChar>
	constexpr bool operator==(const BasicStringView<TChar>& a, const TChar* b)
	{
		return AreStringsSame(a.Buffer(), b, a.Size(), GetCStrLength(b));
	}
	template<StringLiteral TChar>
	constexpr bool operator==(const BasicStringView<TChar>& a, const StringBase<TChar>& b)
	{
		return AreStringsSame(a.Buffer(), b.ConstBuffer(), a.Size(), b.Size());
	}

	// Member Functions Definitions --------------------------------------------------------------------------------------------------------
	template<StringLiteral TChar>
	constexpr BasicStringView<TChar>::BasicStringView(const TChar* CString, size_t size)
		: m_pBuffer(CString)
		, m_size(size)
	{
	}

	template<StringLiteral TChar>
	constexpr BasicStringView<TChar>::BasicStringView(const TChar* CString)
		: BasicStringView(CString, GetCStrLength(CString))
	{
	}

	template<StringLiteral _TChar>
	constexpr BasicStringView<_TChar>::BasicStringView(const BasicStringView& other)
		: m_pBuffer(other.m_pBuffer)
		, m_size(other.m_size)
	{
	}

	template<StringLiteral TChar>
	constexpr BasicStringView<TChar>::BasicStringView(const StringBase<TChar>& string)
		: m_pBuffer(string.ConstBuffer())
		, m_size(string.Size())
	{
	}

	template<StringLiteral TChar>
	BasicStringView<TChar>& BasicStringView<TChar>::operator=(const TChar* CString)
	{
		if (m_pBuffer != CString)
		{
			m_pBuffer = CString;
			m_size = GetCStrLength(CString);
		}

		return *this;
	}

	template<StringLiteral TChar>
	BasicStringView<TChar>& BasicStringView<TChar>::operator=(const BasicStringView& other)
	{
		if (this != &other)
		{
			m_pBuffer = other.m_pBuffer;
			m_size = other.m_size;
		}

		return *this;
	}

	template<StringLiteral TChar>
	BasicStringView<TChar>& BasicStringView<TChar>::operator=(const StringBase<TChar>& string)
	{
		if (m_pBuffer != string.ConstBuffer())
		{
			m_pBuffer = string.ConstBuffer();
			m_size = string.Size();
		}

		return *this;
	}

	template<StringLiteral TChar>
	constexpr BasicStringView<TChar> BasicStringView<TChar>::SubStr(size_t index, size_t count) const
	{
		if (count == npos)
		{
			count = m_size - index;
		}

		JPT_ASSERT((index + count) <= m_size, "SubStr cannot exceeds string's bound");

		return BasicStringView(m_pBuffer + index, count);
	}

	template<StringLiteral _TChar>
	constexpr bool BasicStringView<_TChar>::StartsWith(const TChar* CString, size_t count) const
	{
		return AreStringsSame(m_pBuffer, CString, count);
	}

	template<StringLiteral _TChar>
	constexpr bool BasicStringView<_TChar>::EndsWith(const TChar* CString, size_t count) const
	{
		return AreStringsSame(m_pBuffer + m_size - count, CString, count);
	}

	template<StringLiteral _TChar>
	constexpr size_t BasicStringView<_TChar>::Find(TChar TCharoFind, size_t startIndex, size_t endIndex) const
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
				return i;
			}
		}

		return npos;
	}

	template<StringLiteral _TChar>
	constexpr size_t BasicStringView<_TChar>::Find(const TChar* pStringToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/) const
	{
		const size_t StringToFindSize = GetCStrLength(pStringToFind);
		ClampTo(endIndex, static_cast<size_t>(0), m_size);

		BasicStringView current;
		for (size_t i = startIndex; i < endIndex; ++i)
		{
			if ((i + StringToFindSize) > endIndex)
			{
				return npos;
			}

			current = SubStr(i, StringToFindSize);
			if (current == pStringToFind)
			{
				return i;
			}
		}

		return npos;
	}

	using StringView = BasicStringView<char>;
	using WStringView = BasicStringView<wchar_t>;
}