#pragma once

namespace jpt
{
	class JPT_API string_view
	{
	private:
		const char* m_pBuffer = nullptr;
		size_t m_size = 0;

	public:
		string_view() = delete;
		string_view(const char* inString);
		string_view(const char* inString, size_t size);
		string_view(const jpt::string& inString);

	public:
		// Element access
		const char* data() const { return m_pBuffer; }

		// Capacity
		size_t size() const { return m_size; }

		// Non-member functions
		bool operator==(const char* inString) const;
		bool operator==(const jpt::string_view& inStringView) const;

		// Jupiter custom
		JPT_API friend std::ostream& operator<<(std::ostream& stream, const jpt::string_view& string);

	private:
		bool IsSame(const char* inString) const;
		bool IsSame(const jpt::string_view& inStringView) const;
	};
}