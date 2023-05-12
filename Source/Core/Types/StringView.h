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
		const char* data() const { return m_pBuffer; }
		size_t size() const { return m_size; }

		// Jupiter custom
		JPT_API friend std::ostream& operator<<(std::ostream& stream, const jpt::string_view& string);
	};
}