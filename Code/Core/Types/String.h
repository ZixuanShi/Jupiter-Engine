#pragma once

namespace jpt
{
	class JPT_API string
	{
	public:
		string();
		string(const char* inString);
		string& operator=(const char* inString);
		~string();

	public:
		void clear();
		const char* c_str() const;

	private:
		void CopyString(const char* inString);

	private:
		// The pointer to the buffer representing this string's value
		char* m_pBuffer;

		// How many characters in this string
		size_t m_size;
	};
}

inline std::ostream& operator<<(std::ostream& stream, const jpt::string& string);
template<> inline const jpt::string kInvalidValue<jpt::string> = "Invalid";
template<> inline const jpt::string kDefaultValue<jpt::string> = "Undefined";