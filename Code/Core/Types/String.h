#pragma once

namespace jpt
{
	class JPT_API string
	{
	private:
		char* m_pBuffer;	// The pointer to the buffer representing this string's value
		size_t m_size;		// How many characters in this string currently.
		size_t m_capacity;	// How many characters this string can hold before resizing.

	public:
		// Member functions
		string();
		string(const jpt::string& other);
		string& operator=(const jpt::string& other);
		string(const char* inString);
		string& operator=(const char* inString);
		~string();

	public:
		// Element Access
		const char* c_str() const { return m_pBuffer; }
		char* data() const { return m_pBuffer; }

		// Capacity
		size_t size() const { return m_size; }
		size_t capacity() const { return m_capacity; }

		// Operations
		void clear();
		string& append(const char* inString);

		// Search

	private:
		void UpdateBuffer(size_t inCapacity);
		void CopyString(const char* inString);
	};
}

JPT_API inline std::ostream& operator<<(std::ostream& stream, const jpt::string& string);