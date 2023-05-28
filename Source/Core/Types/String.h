#pragma once

namespace jpt
{
	class JPT_API string
	{
	public:
		static constexpr size_t npos = kInvalidValue<size_t>;

	private:
		char* m_pBuffer   = nullptr;	// The pointer to the buffer representing this string's value
		size_t m_size     = 0;			// How many characters in this string currently.
		size_t m_capacity = 0;			// How many characters this string can hold before resizing.
		static constexpr size_t kCapacityMultiplier = 2;

	public:
		// Member functions
		string();
		string(const char* inString);
		string(const jpt::string& other);
		string(char*&& inString) noexcept;
		string(jpt::string&& other) noexcept;
		string& operator=(const char* inString);
		string& operator=(const jpt::string& other);
		string& operator=(char*&& inString) noexcept;
		string& operator=(jpt::string&& other) noexcept;
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
		void append(const char* inString);
		void append(const string& inString);
		string& operator+=(const char* inString);
		string& operator+=(const string& inString);
		string substr(size_t pos, size_t count = npos) const;

		// Search
		size_t find(const char* stringToFind) const;
		size_t find_first_of(const char* stringToFind) const;
		size_t find_last_of(const char* stringToFind) const;

		// Non-member functions
		string operator+(const char* inString) const;
		string operator+(const string& inString) const;
		bool operator==(const char* inString) const { return IsSame(inString); }
		bool operator==(const string& inString) const { return IsSame(inString); }

		// Jupiter custom
		JPT_API friend jpt::string operator+(const char* leftString, const jpt::string& rightString);
		JPT_API friend std::ostream& operator<<(std::ostream& stream, const jpt::string& string);

	private:
		// Called when the current buffer is not big enough to hold a new string to join. Update the buffer to a larger size and increase capacity
		// - inCapacity: The capacity for the new buffer. Typically current m_size * kCapacityMultiplier
		void UpdateBuffer(size_t inCapacity);

		// Copy the content of inString. Will replace the current buffer entirely.
		// - inString: The new string to hold in buffer
		void CopyString(const char* inString);
		void CopyString(const jpt::string& inString);

		// Take the content of inString. Will assign the buffer to this new string
		// - inString: The new string to take in buffer
		void TakeString(char* inString);
		void TakeString(jpt::string&& inString);

		// Returns: true if this string's value is exactly the same as the inString
		bool IsSame(const char* inString) const;
		bool IsSame(const jpt::string& inString) const;
	};
}