#include "JupiterPCH.h"
#include "String.h"

namespace jpt
{
	string::string(const char* inString)
	{
		CopyString(inString);
	}

	string::string(const jpt::string& other)
	{
		CopyString(other);
	}

	string::string(char*&& inString) noexcept
	{
		TakeString(std::move(inString));
	}

	string::string(jpt::string&& other) noexcept
	{
		TakeString(std::move(other));
	}

	string& string::operator=(const char* inString)
	{
		if (this->c_str() == inString)
		{
			return *this;
		}

		clear();
		CopyString(inString);
		return *this;
	}

	string& string::operator=(const jpt::string& other)
	{
		if (this == &other)
		{
			return *this;
		}

		clear();
		CopyString(other);
		return *this;
	}

	string& string::operator=(char*&& inString) noexcept
	{
		if (this->data() == inString)
		{
			return *this;
		}

		clear();
		TakeString(std::move(inString));
		return *this;
	}

	string& string::operator=(jpt::string&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		clear();
		TakeString(std::move(other));
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

	void string::append(const char* inString)
	{
		const size_t inStringSize = jpt::strlen(inString);
		const size_t newSize = m_size + inStringSize;

		if (newSize > m_capacity)
		{
			UpdateBuffer(newSize * kCapacityMultiplier);		// Reserve some memory storage to append stuff
		}

		strcpy_s(m_pBuffer + m_size, inStringSize + 1, inString);
		m_size = newSize;
	}

	void string::append(const string& inString)
	{
		const size_t newSize = m_size + inString.size();

		if (newSize > m_capacity)
		{
			UpdateBuffer(newSize * kCapacityMultiplier);		// Reserve some memory storage to append stuff
		}

		strcpy_s(m_pBuffer + m_size, inString.size() + 1, inString.c_str());
		m_size = newSize;
	}

	string& string::operator+=(const char* inString)
	{
		append(inString);
		return *this;
	}

	string& string::operator+=(const string& inString)
	{
		append(inString);
		return *this;
	}

	string string::substr(size_t pos, size_t count /*= npos*/) const
	{
		if (count == kInvalidValue<size_t>)
		{
			count = m_size - pos;
		}

		JPT_ASSERT((pos + count) <= m_size, "substr cannot exceeds string's bound");

		char* subStrBuffer = new char[count + 1];
		strncpy_s(subStrBuffer, count + 1, &m_pBuffer[pos], count);

		jpt::string result;
		result.TakeString(subStrBuffer);
		return result;
	}

	size_t string::find(const char* stringToFind) const
	{
		const size_t stringToFindSize = jpt::strlen(stringToFind);

		jpt::string current;
		for (size_t i = 0; i < m_size; ++i)
		{
			if ((i + stringToFindSize) > m_size)
			{
				return npos;
			}

			current = substr(i, stringToFindSize);
			if (current == stringToFind)
			{
				return i;
			}
		}

		return npos;
	}

	size_t string::find_first_of(const char* stringToFind) const
	{
		return find(stringToFind);
	}

	size_t string::find_last_of(const char* stringToFind) const
	{
		const size_t stringToFindSize = jpt::strlen(stringToFind);

		jpt::string current;
		for (size_t i = m_size - 1; i > 0; --i)
		{
			if ((i - stringToFindSize) < 0)
			{
				return npos;
			}

			current = substr(i - stringToFindSize, stringToFindSize);
			if (current == stringToFind)
			{
				return i - stringToFindSize;
			}
		}

		return npos;
	}

	string string::operator+(const char* inString) const
	{
		string str = *this;
		str.append(inString);
		return str;
	}

	string string::operator+(const string& inString) const
	{
		string str = *this;
		str.append(inString.c_str());
		return str;
	}

	void string::UpdateBuffer(size_t inCapacity)
	{
		char* pNewBuffer = new char[inCapacity + 1];

		if (m_pBuffer)
		{
			strcpy_s(pNewBuffer, m_size + 1, m_pBuffer);
			delete[] m_pBuffer;
		}

		m_pBuffer = pNewBuffer;
		m_capacity = inCapacity;
	}

	void string::CopyString(const char* inString)
	{
		m_size = jpt::strlen(inString);
		UpdateBuffer(m_size * kCapacityMultiplier);		// Reserve some memory storage to append stuff

		JPT_ASSERT(m_pBuffer, "m_pBuffer shouldn't be nullptr");
		strcpy_s(m_pBuffer, m_size + 1, inString);
	}

	void string::CopyString(const jpt::string& inString)
	{
		m_size = inString.size();
		UpdateBuffer(m_size * kCapacityMultiplier);		// Reserve some memory storage to append stuff

		JPT_ASSERT(m_pBuffer, "m_pBuffer shouldn't be nullptr");
		strcpy_s(m_pBuffer, m_size + 1, inString.c_str());
	}

	void string::TakeString(char* inString)
	{
		m_pBuffer  = inString;
		m_size     = jpt::strlen(inString);
		m_capacity = m_size;
	}

	void string::TakeString(jpt::string&& inString)
	{
		m_pBuffer  = inString.m_pBuffer;
		m_size     = inString.m_size;
		m_capacity = inString.m_capacity;

		inString.m_pBuffer  = nullptr;
		inString.m_size     = 0;
		inString.m_capacity = 0;
	}

	bool string::IsSame(const char* inString) const
	{
		if (m_size != jpt::strlen(inString))
		{
			return false;
		}

		return strncmp(m_pBuffer, inString, m_size) == 0;
	}

	bool string::IsSame(const jpt::string& inString) const
	{
		if (m_size != inString.size())
		{
			return false;
		}

		return strncmp(m_pBuffer, inString.c_str(), m_size) == 0;
	}

	jpt::string operator+(const char* leftString, const jpt::string& rightString)
	{
		return jpt::string(leftString) += rightString;
	}

	std::ostream& operator<<(std::ostream& stream, const jpt::string& string)
	{
		stream << string.c_str();
		return stream;
	}
}
