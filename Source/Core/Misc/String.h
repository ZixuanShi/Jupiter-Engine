// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include "Core/Algorithms/AlgorithmsHeaders.h"

namespace jpt
{
	/** @return How many characters inside the input const char* string except the null terminator */
	JPT_API size_t strlen(const char* inString);

	/** @return How many characters inside the input const wide char* string except the null terminator */
	JPT_API size_t wcslen(const wchar_t* inString);

	/** Convert from IntegerType to a char pointer holding the integer's value as string literal
		@param value:        The IntegerType value to convert to char*
		@param base:         The base of the value. Default to decimal as 10. Could be binary, oct, hex.
		@return A char pointer pointing to the memory where we store the converted number's string literal */
	template<typename IntegerType>
	inline char* IntegerToCStr(IntegerType value, int32 base = 10)
	{
		// Prepare data
		bool isNegative = false;	// Whether this value is negative or not
		char* result = nullptr;		// Final result string to return
		size_t index = 0;			// The index I'm using for char array, will be used as length of this value and string as well

		// Process 0
		if (value == 0)
		{
			result = new char[2];
			result[0] = '0';
			result[1] = '\0';
			return result;
		}
		// Process negative if IntegerType can be signed
		if constexpr (std::is_signed_v<IntegerType>)
		{
			if (value < 0)
			{
				isNegative = true;
				value = -value;
				++index;
			}
		}

		// Get Value's literal length
		IntegerType valueCopy = value;
		while (valueCopy > 0)
		{
			valueCopy /= base;
			++index;
		}
		// Allocate Result string, + 1 for the end terminater
		result = new char[index + 1];

		// Append string terminator at the end
		result[index] = '\0';
		--index;

		// Process each digit
		while (value > 0)
		{
			const int digit = value % base;

			// For different base, process differently
			if (digit > 9)
			{
				result[index] = static_cast<char>(digit - 10) + 'A';
			}
			else
			{
				result[index] = static_cast<char>(digit + '0');
			}

			// Update index and cut last digit of value
			--index;
			value /= base;
		}

		// If value was negative, add '-' at the beginning
		if (isNegative)
		{
			result[index] = '-';
			--index;
		}

		// Return result
		return result;
	}

	template<typename FloatingType>
	inline char* FloatingToCStr(FloatingType value)
	{
		char* buffer = new char[32];
		char format[5] = "%.3f";
		snprintf(buffer, 32, format, value);
		return buffer;
	}

	/** @return 0 if two strings are identical. InvalidValue if not */
	JPT_API int32 strncmp(const char* string1, const char* string2, size_t size);

	/** @return 0 if two wide strings are identical. InvalidValue if not */
	JPT_API int32 wcsncmp(const wchar_t* string1, const wchar_t* string2, size_t size);

	/** FNV1a c++11 constexpr compile time hash functions, 32 and 64 bit
		code license: public domain or equivalent
		@str: should be a null terminated string literal, value should be left out e.g hash_32_fnv1a_const("example")
		@see https://notes.underscorediscovery.com/constexpr-fnv1a/
		@see https://gist.github.com/underscorediscovery/81308642d0325fd386237cfa3b44785c#file-hash_fnv1a_constexpr-h */
	JPT_API static constexpr inline uint32 StringHash32(const char* const str, const uint32 value = 0x811c9dc5) noexcept { return (str[0] == '\0') ? value : StringHash32(&str[1], (value ^ uint32(str[0])) * 0x1000193); }
	JPT_API static constexpr inline  uint64 StringHash64(const char* const str, const uint64 value = 0xcbf29ce484222325) noexcept { return (str[0] == '\0') ? value : StringHash64(&str[1], (value ^ uint64(str[0])) * 0x100000001b3); }

	template<>
	struct hash<const char*>
	{
		size_t operator()(const char* key)
		{
			return jpt::StringHash64(key);
		}
	};

	/** Calculate the inString's size
		@param inString:	Templated const chartype string to check for size. */
	template<typename CharType>
	inline size_t GetStrLength(const CharType* inString)
	{
		if constexpr (jpt::IsSameType<CharType, char>::Value)
		{
			return jpt::strlen(inString);
		}
		else if (jpt::IsSameType<CharType, wchar_t>::Value)
		{
			return jpt::wcslen(inString);
		}
	}

	template<typename CharType>
	inline void StrCpy(CharType* pDestination, size_t sizeInBytes, const CharType* pSource)
	{
		if constexpr (jpt::IsSameType<CharType, char>::Value)
		{
			strcpy_s(pDestination, sizeInBytes, pSource);
		}
		else if (jpt::IsSameType<CharType, wchar_t>::Value)
		{
			wcscpy_s(pDestination, sizeInBytes, pSource);
		}
	}

	template<typename CharType>
	inline void StrNCpy(CharType* pDestination, size_t sizeInBytes, const CharType* pSource, size_t maxCount)
	{
		if constexpr (jpt::IsSameType<CharType, char>::Value)
		{
			strncpy_s(pDestination, sizeInBytes, pSource, maxCount);
		}
		else if (jpt::IsSameType<CharType, wchar_t>::Value)
		{
			wcsncpy_s(pDestination, sizeInBytes, pSource, maxCount);
		}
	}

	/* Compare string */
	template<typename CharType>
	inline bool StrNCmp(const CharType* pString1, const CharType* pString2, size_t size)
	{
		if (GetStrLength(pString1) != GetStrLength(pString2))
		{
			return false;
		}

		if constexpr (jpt::IsSameType<CharType, char>::Value)
		{
			return jpt::strncmp(pString1, pString2, size) == 0;
		}
		else if (jpt::IsSameType<CharType, wchar_t>::Value)
		{
			return jpt::wcsncmp(pString1, pString2, size) == 0;
		}
	}
	template<typename StringType>
	inline bool StringCmp(const StringType& string1, const StringType& string2, size_t size)
	{
		if (string1.size() != string2.size())
		{
			return false;
		}

		return jpt::StrNCmp(string1.c_str(), string2.c_str(), size);
	}

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
	template<typename Type>			
	inline std::enable_if_t<!std::is_arithmetic_v<Type>, jpt::string> ToString(const Type& object)
	{
		return object.to_string();
	}
	template<typename IntegerType>
	inline jpt::enable_if_t<std::is_integral_v<IntegerType>, jpt::string> ToString(IntegerType integer)
	{
		return jpt::string(jpt::IntegerToCStr(integer));
	}
	template<typename FloatingType>
	inline jpt::enable_if_t<std::is_floating_point_v<FloatingType>, jpt::string> ToString(FloatingType value)
	{
		return jpt::string(jpt::FloatingToCStr(value));
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
