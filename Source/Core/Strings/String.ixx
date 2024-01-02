// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.String;

import jpt.Allocator;
import jpt.TypeDefs;
import jpt.StringUtils;

namespace jpt
{
	template<typename _CharType, class _AllocatorType = Allocator<_CharType>>
	class BasicString
	{
	public:
		using CharType	    = _CharType;
		using AllocatorType = _AllocatorType;

	public:
		static constexpr size_t npos = kInvalidValue<size_t>;

	private:
		CharType* m_pBuffer = nullptr;	/**< The pointer to the buffer representing this string's value */
		size_t m_size       = 0;		/**< How many characters in this string currently */
		size_t m_capacity   = 0;		/**< How many characters this string can hold before resizing */

	public:
		BasicString() = default;
		BasicString(const CharType* CString);

		/* Copy the content of C-Style string. Will assign the current m_pBuffer with the new copied data in memory */
		void CopyCString(const CharType* CString);
	};

	template<typename _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType>::BasicString(const CharType* CString)
	{
		CopyCString(CString);
	}

	template<typename _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::CopyCString(const CharType* CString)
	{
		//
	}
}

export namespace jpt
{
	using String = BasicString<char>;
	using WString = BasicString<wchar_t>;
}