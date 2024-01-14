// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.StringView;

import jpt.Allocator;
import jpt.TypeDefs;
import jpt.Concepts;
import jpt.StringUtils;
import jpt.String;

namespace jpt
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
		constexpr BasicStringView() = default;
		constexpr BasicStringView(const CharT* CString, size_t size);
		constexpr BasicStringView(const CharT* CString);

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
}

export namespace jpt
{
	using StringView = BasicStringView<char>;
	using WStringView = BasicStringView<wchar_t>;
}