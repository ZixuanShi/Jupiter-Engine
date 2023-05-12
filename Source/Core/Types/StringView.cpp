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

	std::ostream& operator<<(std::ostream& stream, const jpt::string_view& string)
	{
		printf("%.*s", static_cast<int>(string.size()), string.data());
		return stream;
	}
}
