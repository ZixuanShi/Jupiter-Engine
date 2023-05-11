#include "JupiterPCH.h"
#include "String.h"

namespace jpt
{
	string::string()
		: m_pBuffer(nullptr)
		, m_size(0)
		, m_capacity(0)
	{
	}

	string::string(const jpt::string& other)
		: m_pBuffer(nullptr)
		, m_size(0)
		, m_capacity(0)
	{
		CopyString(other.c_str());
	}

	string& string::operator=(const jpt::string& other)
	{
		clear();
		CopyString(other.c_str());
		return *this;
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

	string& string::append(const char* inString)
	{
		size_t inStringSize = std::strlen(inString);
		m_size += inStringSize;
		return *this;
	}

	void string::CopyString(const char* inString)
	{
		m_size = std::strlen(inString);
		m_capacity = m_size;
		m_pBuffer = new char[m_size + 1];	// +1 for null terminator '\0'
		strcpy_s(m_pBuffer, m_size + 1, inString);
	}
}

std::ostream& operator<<(std::ostream& stream, const jpt::string& string)
{
	stream << string.c_str();
	return stream;
}
