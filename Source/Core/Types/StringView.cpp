#include "JupiterPCH.h"
#include "StringView.h"

namespace jpt
{
	string_view::string_view(const char* inString)
		: m_pBuffer(inString)
		, m_size(jpt::strlen(inString))
	{
	}

	string_view::string_view(const char* inString, size_t size)
		: m_pBuffer(inString)
		, m_size(size)
	{
	}

	string_view::string_view(const jpt::string& inString)
		: m_pBuffer(inString.c_str())
		, m_size(inString.size())
	{
	}

	bool string_view::operator==(const char* inString) const
	{
		return IsSame(inString);
	}

	bool string_view::operator==(const jpt::string_view& inStringView) const
	{
		return IsSame(inStringView);
	}

	bool string_view::IsSame(const char* inString) const
	{
		if (m_size != jpt::strlen(inString))
		{
			return false;
		}

		return strncmp(m_pBuffer, inString, m_size) == 0;
	}

	bool string_view::IsSame(const jpt::string_view& inStringView) const
	{
		if (m_size != inStringView.size())
		{
			return false;
		}

		return strncmp(m_pBuffer, inStringView.data(), m_size) == 0;
	}

	std::ostream& operator<<(std::ostream& stream, const jpt::string_view& string)
	{
		printf("%.*s", static_cast<int>(string.size()), string.data());
		return stream;
	}
}
