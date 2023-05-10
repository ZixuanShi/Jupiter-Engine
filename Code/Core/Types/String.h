#pragma once

namespace jpt
{
	class JPT_API string
	{
	private:
		char* m_pBuffer;	// The pointer to the buffer representing this string's value
		size_t m_size;		// How many characters in this string

	public:
		string();
		string(const jpt::string& other);
		string& operator=(const jpt::string& other);
		string(const char* inString);
		string& operator=(const char* inString);
		~string();

	public:
		void clear();
		const char* c_str() const { return m_pBuffer; }
		char* GetBuffer() const { return m_pBuffer; }

	private:
		void CopyString(const char* inString);
	};
}

inline std::ostream& operator<<(std::ostream& stream, const jpt::string& string);
template<> inline const jpt::string kInvalidValue<jpt::string> = "Invalid";
template<> inline const jpt::string kDefaultValue<jpt::string> = "Undefined";