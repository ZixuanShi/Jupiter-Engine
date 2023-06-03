#pragma once

namespace jpt
{
	template<typename CharType>
	class string_view
	{
	private:
		const CharType* m_pBuffer = nullptr;
		size_t m_size = 0;

	public:
		string_view() = delete;
		string_view(const CharType* inString);
		string_view(const CharType* inString, size_t size);
		string_view(const basic_string<CharType>& inString);

	public:
		// Element access
		const CharType* data() const { return m_pBuffer; }

		// Capacity
		size_t size() const { return m_size; }

		// Non-member functions
		bool operator==(const CharType* inString) const;
		bool operator==(const jpt::string_view<CharType>& inStringView) const;

		// Jupiter custom
		JPT_API friend std::ostream& operator<<(std::ostream& stream, const jpt::string_view<CharType>& string);
	};

	template<typename CharType>
	inline string_view<CharType>::string_view(const CharType* inString)
		: m_pBuffer(inString)
	{
		if constexpr (jpt::IsSameType<CharType, char>::Value)
		{
			m_size = jpt::strlen(inString);
		}
		else if (jpt::IsSameType<CharType, wchar_t>::Value)
		{
			m_size = jpt::wcslen(inString);
		}
	}

	template<typename CharType>
	inline string_view<CharType>::string_view(const CharType* inString, size_t size)
		: m_pBuffer(inString)
		, m_size(size)
	{
	}

	template<typename CharType>
	inline string_view<CharType>::string_view(const basic_string<CharType>& inString)
		: m_pBuffer(inString.c_str())
		, m_size(inString.size())
	{
	}

	template<typename CharType>
	inline bool string_view<CharType>::operator==(const CharType* inString) const
	{
		if constexpr (jpt::IsSameType<CharType, char>::Value)
		{
			if (m_size != jpt::strlen(inString))
			{
				return false;
			}
			return strncmp(m_pBuffer, inString, m_size) == 0;
		}
		else if (jpt::IsSameType<CharType, wchar_t>::Value)
		{
			if (m_size != jpt::wcslen(inString))
			{
				return false;
			}
			return wcsncmp(m_pBuffer, inString, m_size) == 0;
		}
	}

	template<typename CharType>
	inline bool string_view<CharType>::operator==(const jpt::string_view<CharType>& inStringView) const
	{
		if (m_size != inStringView.size())
		{
			return false;
		}

		if constexpr (jpt::IsSameType<CharType, char>::Value)
		{
			return strncmp(m_pBuffer, inStringView.data(), m_size) == 0;
		}
		else if (jpt::IsSameType<CharType, wchar_t>::Value)
		{
			return wcsncmp(m_pBuffer, inStringView.data(), m_size) == 0;
		}
	}

	template<typename CharType>
	inline std::ostream& operator<<(std::ostream& stream, const jpt::string_view<CharType>& string)
	{
		printf("%.*s", static_cast<int32>(string.size()), string.data());
		return stream;
	}
}