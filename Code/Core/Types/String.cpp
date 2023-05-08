#include "JupiterPCH.h"
#include "String.h"

namespace jpt
{
	string::string()
		: m_pBuffer(nullptr)
		, m_size(0)
	{
	}

	string::string(const char* inString)
	{
		CopyString(inString);
	}

	string& string::operator=(const char* inString)
	{
		CopyString(inString);
		return *this;
	}

	string::~string()
	{
		clear();
	}

	void string::clear()
	{
		JPT_SAFE_DELETE_ARRAY(m_pBuffer);
		m_size = 0;
	}

	const char* string::c_str() const
	{
		return m_pBuffer;
	}

	void string::CopyString(const char* inString)
	{
		m_size = strlen(inString);
		m_pBuffer = JPT_NEW_ARRAY(char, m_size + 1);	// +1 for null terminator '\0'
		strcpy_s(m_pBuffer, m_size + 1, inString);
	}
}

inline std::ostream& operator<<(std::ostream& stream, const jpt::string& string)
{
	stream << string.c_str();
	return stream;
}
