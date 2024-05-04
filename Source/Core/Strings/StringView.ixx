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
	class StringViewBase
	{
	public:
		using TChar = _TChar;

	private:
		const TChar* m_pBuffer = nullptr;
		size_t m_count = 0;

	public:
		// Constructors
		constexpr StringViewBase() = default;
		constexpr StringViewBase(const TChar* CString, size_t size);
		constexpr StringViewBase(const TChar* CString);
		constexpr StringViewBase(const StringViewBase& other);
		constexpr StringViewBase(const StringBase<TChar>& string);

		// operator=
		StringViewBase& operator=(const TChar* CString);
		StringViewBase& operator=(const StringViewBase& other);
		StringViewBase& operator=(const StringBase<TChar>& string);

		// Element Access
		constexpr const TChar* ConstBuffer()    const { return m_pBuffer;             } 
		constexpr const TChar& At(size_t index) const { return m_pBuffer[index];      }
		constexpr const TChar& Front()          const { return m_pBuffer[0];          }
		constexpr const TChar& Back()           const { return m_pBuffer[m_count - 1]; }
		constexpr const TChar& operator[](size_t index) const { return m_pBuffer[index]; }

		// Capacity
		constexpr size_t Count() const { return m_count; }
		constexpr bool IsEmpty() const { return m_count == 0; }

		/** @return		A sub string within the given range at index and length */
		constexpr StringViewBase SubStr(size_t index, size_t count = npos) const;

		/** @return     true if the string view starts with the given prefix */
		constexpr bool StartsWith(const TChar* CString, size_t count) const;
		constexpr bool StartsWith(const TChar* CString) const { return StartsWith(CString, FindCharsCount(CString)); }

		/** @return     true if the string view ends with the given suffix */
		constexpr bool EndsWith(const TChar* CString, size_t count) const;
		constexpr bool EndsWith(const TChar* CString) const { return EndsWith(CString, FindCharsCount(CString)); }

		constexpr size_t Find(      TChar  charToFind,    size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t Find(const TChar* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindLastOf(      TChar charToFind,     size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
		constexpr size_t FindLastOf(const TChar* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
		constexpr bool   Contains(      TChar  charToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(charToFind, startIndex, endIndex, count) != npos; }
		constexpr bool   Contains(const TChar* charToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(charToFind, startIndex, endIndex, count) != npos; }
	};

	// Non member functions -------------------------------------------------------------------------------------------------------------------
	template<StringLiteral TChar>
	constexpr bool operator==(const StringViewBase<TChar>& a, const StringViewBase<TChar>& b)
	{
		return AreStringsSame(a.ConstBuffer(), b.ConstBuffer(), a.Count(), b.Count());
	}
	template<StringLiteral TChar>
	constexpr bool operator==(const StringViewBase<TChar>& a, const TChar* b)
	{
		return AreStringsSame(a.ConstBuffer(), b, a.Count(), FindCharsCount(b));
	}
	template<StringLiteral TChar>
	constexpr bool operator==(const StringViewBase<TChar>& a, const StringBase<TChar>& b)
	{
		return AreStringsSame(a.ConstBuffer(), b.ConstBuffer(), a.Count(), b.Count());
	}

	// Member Functions Definitions --------------------------------------------------------------------------------------------------------
	template<StringLiteral TChar>
	constexpr StringViewBase<TChar>::StringViewBase(const TChar* CString, size_t size)
		: m_pBuffer(CString)
		, m_count(size)
	{
	}

	template<StringLiteral TChar>
	constexpr StringViewBase<TChar>::StringViewBase(const TChar* CString)
		: StringViewBase(CString, FindCharsCount(CString))
	{
	}

	template<StringLiteral _TChar>
	constexpr StringViewBase<_TChar>::StringViewBase(const StringViewBase& other)
		: m_pBuffer(other.m_pBuffer)
		, m_count(other.m_count)
	{
	}

	template<StringLiteral TChar>
	constexpr StringViewBase<TChar>::StringViewBase(const StringBase<TChar>& string)
		: m_pBuffer(string.ConstBuffer())
		, m_count(string.Count())
	{
	}

	template<StringLiteral TChar>
	StringViewBase<TChar>& StringViewBase<TChar>::operator=(const TChar* CString)
	{
		if (m_pBuffer != CString)
		{
			m_pBuffer = CString;
			m_count = FindCharsCount(CString);
		}

		return *this;
	}

	template<StringLiteral TChar>
	StringViewBase<TChar>& StringViewBase<TChar>::operator=(const StringViewBase& other)
	{
		if (this != &other)
		{
			m_pBuffer = other.m_pBuffer;
			m_count = other.m_count;
		}

		return *this;
	}

	template<StringLiteral TChar>
	StringViewBase<TChar>& StringViewBase<TChar>::operator=(const StringBase<TChar>& string)
	{
		if (m_pBuffer != string.ConstBuffer())
		{
			m_pBuffer = string.ConstBuffer();
			m_count = string.Count();
		}

		return *this;
	}

	template<StringLiteral TChar>
	constexpr StringViewBase<TChar> StringViewBase<TChar>::SubStr(size_t index, size_t count) const
	{
		if (count == npos)
		{
			count = m_count - index;
		}

		JPT_ASSERT((index + count) <= m_count, "SubStr cannot exceeds string's bound");

		return StringViewBase(m_pBuffer + index, count);
	}

	template<StringLiteral _TChar>
	constexpr bool StringViewBase<_TChar>::StartsWith(const TChar* CString, size_t count) const
	{
		return AreStringsSame(m_pBuffer, CString, count);
	}

	template<StringLiteral _TChar>
	constexpr bool StringViewBase<_TChar>::EndsWith(const TChar* CString, size_t count) const
	{
		return AreStringsSame(m_pBuffer + m_count - count, CString, count);
	}

	template<StringLiteral _TChar>
	constexpr size_t StringViewBase<_TChar>::Find(TChar charToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
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

	template<StringLiteral _TChar>
	constexpr size_t StringViewBase<_TChar>::Find(const TChar* pStringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
	{
		const size_t StringToFindSize = FindCharsCount(pStringToFind);
		ClampTo(endIndex, static_cast<size_t>(0), m_count);

		StringViewBase current;
		for (size_t i = startIndex; i < endIndex; ++i)
		{
			if ((i + StringToFindSize) > endIndex)
			{
				return npos;
			}

			current = SubStr(i, StringToFindSize);
			if (current == pStringToFind)
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

	template<StringLiteral _TChar>
	constexpr size_t StringViewBase<_TChar>::FindLastOf(TChar charToFind, size_t startIndex, size_t endIndex, size_t count) const
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

	template<StringLiteral _TChar>
	constexpr size_t StringViewBase<_TChar>::FindLastOf(const TChar* pStringToFind, size_t startIndex, size_t endIndex, size_t count) const
	{
		const size_t StringToFindSize = FindCharsCount(pStringToFind);
		ClampTo(endIndex, size_t(0), m_count);

		StringBase<TChar> current;
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

	using StringView = StringViewBase<char>;
	using WStringView = StringViewBase<wchar_t>;
}