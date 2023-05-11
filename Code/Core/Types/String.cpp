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
		, m_size(other.m_size)
		, m_capacity(other.m_capacity)
	{
		CopyString(other.c_str());
	}

	string& string::operator=(const jpt::string& other)
	{
		CopyString(other.c_str());
		return *this;
	}

	string::string(const char* inString)
		: m_pBuffer(nullptr)
		, m_size(0)
		, m_capacity(0)
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

	string& string::append(const char* )
	{
		// TODO
		return *this;
	}

	void string::UpdateBuffer(size_t inCapacity)
	{
		char* pNewBuffer = new char[inCapacity + 1];

		if (m_pBuffer)
		{
			strcpy_s(pNewBuffer, m_size + 1, m_pBuffer);
			JPT_DELETE_ARRAY(m_pBuffer);
		}

		m_pBuffer = pNewBuffer;
		m_capacity = inCapacity;
	}

	void string::CopyString(const char* inString)
	{
		clear();

		m_size = jpt::strlen(inString);
		UpdateBuffer(m_size * 2);		// Reserve some memory storage to append stuff

		JPT_ASSERT(m_pBuffer, "m_pBuffer shouldn't be nullptr");
		strcpy_s(m_pBuffer, m_size + 1, inString);
	}
}

std::ostream& operator<<(std::ostream& stream, const jpt::string& string)
{
	stream << string.c_str();
	return stream;
}
