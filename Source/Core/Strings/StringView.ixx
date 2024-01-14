// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.StringView;

import jpt.Allocator;
import jpt.TypeDefs;
import jpt.Concepts;
import jpt.StringUtils;

export namespace jpt
{
	template<typename T>
	class BasicString;

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
		constexpr BasicStringView() = default;
		constexpr BasicStringView(const CharT* CString, size_t size);
		constexpr BasicStringView(const CharT* CString);
		BasicStringView(const BasicStringView& other);
		BasicStringView(const BasicString<CharT>& string);
	};

	template<StringLiteral _CharT>
	constexpr BasicStringView<_CharT>::BasicStringView(const CharT* CString, size_t size)
		: m_pBuffer(CString)
		, m_size(size)
	{
	}

	template<StringLiteral _CharT>
	constexpr BasicStringView<_CharT>::BasicStringView(const CharT* CString)
		: BasicStringView(CString, GetCStrLength(CString))
	{
	}

	template<StringLiteral _CharT>
	BasicStringView<_CharT>::BasicStringView(const BasicStringView& other)
		: m_pBuffer(other.m_pBuffer)
		, m_size(other.m_size)
	{
	}

	template<StringLiteral _CharT>
	BasicStringView<_CharT>::BasicStringView(const BasicString<CharT>& string)
		: m_pBuffer(string.ConstBuffer())
		, m_size(string.Size())
	{
	}

	using StringView = BasicStringView<char>;
	using WStringView = BasicStringView<wchar_t>;
}