// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include "Core/Algorithms/AlgorithmsHeaders.h"

namespace jpt
{
	/** Jupiter's string implementation
	    @param CharType: coule be char as regular string, or wchar_t as wide string */
	template<typename CharType>
	class basic_string
	{
	public:
		static constexpr size_t npos = kInvalidValue<size_t>;

	private:
		CharType* m_pBuffer  = nullptr;	    // The pointer to the buffer representing this string's value
		size_t m_size        = 0;			// How many characters in this string currently.
		size_t m_capacity    = 0;			// How many characters this string can hold before resizing.
		static constexpr size_t kCapacityMultiplier = 2;

	public:
		// Member functions
		basic_string() = default;
		basic_string(const CharType* inString);
		basic_string(const jpt::basic_string<CharType>& other);
		basic_string(CharType*&& inString) noexcept;
		basic_string(jpt::basic_string<CharType>&& other) noexcept;
		basic_string& operator=(const CharType* inString);
		basic_string& operator=(const jpt::basic_string<CharType>& other);
		basic_string& operator=(CharType*&& inString) noexcept;
		basic_string& operator=(jpt::basic_string<CharType>&& other) noexcept;
		~basic_string();

	public:
		// Element Access
		const CharType* c_str() const { return m_pBuffer; }
		CharType* data() const { return m_pBuffer; }

		// Capacity
		size_t size() const { return m_size; }
		size_t capacity() const { return m_capacity; }
		bool empty() const { return m_size == 0; }

		// Operations
		void clear();
		void append(const CharType* inString);
		void append(const basic_string& inString);
		basic_string& operator+=(const CharType* inString);
		basic_string& operator+=(const basic_string<CharType>& inString);
		basic_string substr(size_t pos, size_t count = npos) const;

		/* Replace the some content of this string with the new given one within a range
			@param stringToFind:	The string we want to remove and replace 
			@param stringToReplace:	The string to replace the original data
			@param startIndex:		The start index to start searching. Default to 0 
			@param endIndex:		The end index to stop operation. Default to size() */
		basic_string& replace(const CharType* stringToFind, const CharType* stringToReplace, size_t startIndex = 0, size_t endIndex = npos);

		// Search
		size_t find(const CharType* stringToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		size_t find_first_of(const CharType* stringToFind, size_t startIndex = 0, size_t endIndex = npos) const;
		size_t find_last_of(const CharType* stringToFind, size_t startIndex = 0, size_t endIndex = npos) const;

		// Non-member functions
		basic_string operator+(const CharType* inString) const;
		basic_string operator+(const basic_string<CharType>& inString) const;
		bool operator==(const CharType* inString) const;
		bool operator==(const basic_string<CharType>& inString) const;

		/* Copy the content of inString.Will replace the current buffer entirely.
			@param inString: The new string to hold in buffer */
		void CopyString(const CharType* inString);
		void CopyString(const jpt::basic_string<CharType>& inString);

		/* Take the content of inString.Will assign the buffer to this new string
			@param inString: The new string to take in buffer */
		void TakeString(CharType* inString);
		void TakeString(jpt::basic_string<CharType>&& inString);

	private:
		/* Called when the current buffer is not big enough to hold a new string to join.Update the buffer to a larger sizeand increase capacity
			@param inCapacity: The capacity for the new buffer. Typically current m_size * kCapacityMultiplier */
		void UpdateBuffer(size_t inCapacity);
	};

	template<typename CharType>
	inline basic_string<CharType>::basic_string(const CharType* inString)
	{
		CopyString(inString);
	}

	template<typename CharType>
	inline basic_string<CharType>::basic_string(const jpt::basic_string<CharType>& other)
	{
		CopyString(other);
	}

	template<typename CharType>
	inline basic_string<CharType>::basic_string(CharType*&& inString) noexcept
	{
		TakeString(jpt::move(inString));
	}

	template<typename CharType>
	inline basic_string<CharType>::basic_string(jpt::basic_string<CharType>&& other) noexcept
	{
		TakeString(jpt::move(other));
	}

	template<typename CharType>
	inline basic_string<CharType>& basic_string<CharType>::operator=(const CharType* inString)
	{
		if (this->c_str() == inString)
		{
			return *this;
		}

		clear();
		CopyString(inString);
		return *this;
	}

	template<typename CharType>
	inline basic_string<CharType>& basic_string<CharType>::operator=(const jpt::basic_string<CharType>& other)
	{
		if (this == &other)
		{
			return *this;
		}

		clear();
		CopyString(other);
		return *this;
	}

	template<typename CharType>
	inline basic_string<CharType>& basic_string<CharType>::operator=(CharType*&& inString) noexcept
	{
		if (this->data() == inString)
		{
			return *this;
		}

		clear();
		TakeString(jpt::move(inString));
		return *this;
	}

	template<typename CharType>
	inline basic_string<CharType>& basic_string<CharType>::operator=(jpt::basic_string<CharType>&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		clear();
		TakeString(jpt::move(other));
		return *this;
	}

	template<typename CharType>
	inline basic_string<CharType>::~basic_string<CharType>()
	{
		clear();
	}

	template<typename CharType>
	inline void basic_string<CharType>::clear()
	{
		JPT_SAFE_DELETE_ARRAY(m_pBuffer);
		m_size = 0;
		m_capacity = 0;
	}

	template<typename CharType>
	inline void basic_string<CharType>::append(const CharType* inString)
	{
		const size_t inStringSize = GetStrLength(inString);
		if (inStringSize == 0)
		{
			return;
		}

		const size_t newSize = m_size + inStringSize;
		if (newSize > m_capacity)
		{
			UpdateBuffer(newSize * kCapacityMultiplier);		// Reserve some memory storage to append stuff
		}

		StrCpy(m_pBuffer + m_size, inStringSize + sizeof(CharType), inString);

		m_size = newSize;
	}

	template<typename CharType>
	inline void basic_string<CharType>::append(const basic_string<CharType>& inString)
	{
		if (inString.empty())
		{
			return;
		}

		const size_t newSize = m_size + inString.size();
		if (newSize > m_capacity)
		{
			UpdateBuffer(newSize * kCapacityMultiplier);		// Reserve some memory storage to append stuff
		}

		StrCpy(m_pBuffer + m_size, inString.size() + sizeof(CharType), inString.c_str());

		m_size = newSize;
	}

	template<typename CharType>
	inline basic_string<CharType>& basic_string<CharType>::operator+=(const CharType* inString)
	{
		append(inString);
		return *this;
	}

	template<typename CharType>
	inline basic_string<CharType>& basic_string<CharType>::operator+=(const basic_string<CharType>& inString)
	{
		append(inString);
		return *this;
	}

	template<typename CharType>
	inline basic_string<CharType> basic_string<CharType>::substr(size_t pos, size_t count /*= npos*/) const
	{
		if (count == kInvalidValue<size_t>)
		{
			count = m_size - pos;
		}

		JPT_ASSERT((pos + count) <= m_size, "substr cannot exceeds string's bound");

		if (count == 0)
		{
			return jpt::basic_string<CharType>();
		}

		CharType* subStrBuffer = new CharType[count + sizeof(CharType)];

		StrNCpy(subStrBuffer, count + sizeof(CharType), &m_pBuffer[pos], count);

		jpt::basic_string<CharType> result;
		result.TakeString(subStrBuffer);
		return result;
	}

	template<typename CharType>
	inline basic_string<CharType>& basic_string<CharType>::replace(const CharType* stringToFind, const CharType* stringToReplace, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/)
	{
		if (endIndex == npos)
		{
			endIndex = m_size;
		}

		const basic_string<CharType> replaced(stringToReplace);
		const size_t stringToFindSize = GetStrLength(stringToFind);

		size_t foundPos = startIndex;
		while (true)
		{
			foundPos = find(stringToFind, startIndex, endIndex);
			if (foundPos == npos)
			{
				break;
			}

			const basic_string<CharType> pre = substr(0, foundPos);
			const basic_string<CharType> suff = substr(foundPos + stringToFindSize);

			*this = pre + replaced + suff;
			startIndex = foundPos + replaced.size();	// In case 'stringToReplace' contains 'stringToFind', like replacing 'x' with 'yx'		
		}

		return *this;
	}

	template<typename CharType>
	inline size_t basic_string<CharType>::find(const CharType* stringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		const size_t stringToFindSize = GetStrLength(stringToFind);
		jpt::ClampTo(endIndex, size_t(0), m_size);

		jpt::basic_string<CharType> current;
		for (size_t i = startIndex; i < endIndex; ++i)
		{
			if ((i + stringToFindSize) > endIndex)
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

	template<typename CharType>
	inline size_t basic_string<CharType>::find_first_of(const CharType* stringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		return find(stringToFind, startIndex, endIndex);
	}

	template<typename CharType>
	inline size_t basic_string<CharType>::find_last_of(const CharType* stringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/) const
	{
		const size_t stringToFindSize = GetStrLength(stringToFind);
		
		if (endIndex == npos)
		{
			endIndex = m_size;
		}

		jpt::basic_string<CharType> current;
		for (size_t i = endIndex - 1; i > startIndex; --i)
		{
			if ((i - stringToFindSize) < startIndex)
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

	template<typename CharType>
	inline basic_string<CharType> basic_string<CharType>::operator+(const CharType* inString) const
	{
		basic_string<CharType> str = *this;
		str.append(inString);
		return str;
	}

	template<typename CharType>
	inline basic_string<CharType> basic_string<CharType>::operator+(const basic_string<CharType>& inString) const
	{
		basic_string<CharType> str = *this;
		str.append(inString.c_str());
		return str;
	}

	template<typename CharType>
	inline void basic_string<CharType>::UpdateBuffer(size_t inCapacity)
	{
		CharType* pNewBuffer = new CharType[inCapacity + sizeof(CharType)];

		if (m_pBuffer)
		{
			StrCpy(pNewBuffer, m_size + sizeof(CharType), m_pBuffer);
			delete[] m_pBuffer;
		}

		m_pBuffer = pNewBuffer;
		m_capacity = inCapacity;
	}

	template<typename CharType>
	inline void basic_string<CharType>::CopyString(const CharType* inString)
	{
		m_size = GetStrLength(inString);

		if (empty())
		{
			return;
		}

		UpdateBuffer(m_size * kCapacityMultiplier);		// Reserve some memory storage to append stuff

		JPT_ASSERT(m_pBuffer, "m_pBuffer shouldn't be nullptr");

		StrCpy(m_pBuffer, m_size + sizeof(CharType), inString);
	}

	template<typename CharType>
	inline void basic_string<CharType>::CopyString(const jpt::basic_string<CharType>& inString)
	{
		m_size = inString.size();
		if (empty())
		{
			return;
		}

		UpdateBuffer(m_size * kCapacityMultiplier);		// Reserve some memory storage to append stuff

		JPT_ASSERT(m_pBuffer, "m_pBuffer shouldn't be nullptr");

		StrCpy(m_pBuffer, m_size + sizeof(CharType), inString.c_str());
	}

	template<typename CharType>
	inline void basic_string<CharType>::TakeString(CharType* inString)
	{
		m_pBuffer = inString;
		m_size = GetStrLength(inString);
		m_capacity = m_size;
	}

	template<typename CharType>
	inline void basic_string<CharType>::TakeString(jpt::basic_string<CharType>&& inString)
	{
		m_pBuffer = inString.m_pBuffer;
		m_size = inString.m_size;
		m_capacity = inString.m_capacity;

		inString.m_pBuffer = nullptr;
		inString.m_size = 0;
		inString.m_capacity = 0;
	}

	template<typename CharType>
	inline bool basic_string<CharType>::operator==(const CharType* inString) const
	{
		return StrNCmp(m_pBuffer, inString, m_size);
	}

	template<typename CharType>
	inline bool basic_string<CharType>::operator==(const basic_string<CharType>& inString) const
	{
		return StringCmp(*this, inString, m_size);
	}

	using string = basic_string<char>;
	using wstring = basic_string<wchar_t>;

	template<typename CharType>
	inline jpt::basic_string<CharType> operator+(const CharType* leftString, const jpt::basic_string<CharType>& rightString)
	{
		return jpt::basic_string<CharType>(leftString) += rightString;
	}

	inline std::ostream& operator<<(std::ostream& stream, const jpt::string& string)
	{
		stream << string.c_str();
		return stream;
	}

	inline std::wostream& operator<<(std::wostream& stream, const jpt::wstring& string)
	{
		stream << string.c_str();
		return stream;
	}

	// Converts an input object to string if it has this functionality
	// Primitive types should be specialized below
	template<class Type>
	inline jpt::string to_string(const Type& object)
	{
		return object.to_string();
	}
	template<>
	inline jpt::string to_string<int32>(const int32& num)
	{
		return jpt::string(jpt::itoa(num));
	}
}

namespace std
{
	template<>
	struct hash<jpt::string>
	{
		size_t operator()(const jpt::string& key)
		{
			return jpt::StringHash64(key.c_str());
		}
	};
}
