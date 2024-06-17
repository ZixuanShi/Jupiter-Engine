// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

#include <fstream>
#include <string>

export module jpt.Serializer;

export namespace jpt
{
	class Serializer
	{
	private:
		std::fstream m_stream;

	public:
		Serializer() = default;
		Serializer(const wchar_t* path, std::ios_base::openmode mode);
		~Serializer();

		void Open(const wchar_t* path, std::ios_base::openmode mode);
		bool IsOpen() const;

		void Close();

		void Write(const char* buffer, size_t sizeInBytes);
		void Read(char* buffer, size_t sizeInBytes);
		char* ReadText();
	};

	Serializer::Serializer(const wchar_t* path, std::ios_base::openmode mode)
	{
		Open(path, mode);
		JPT_ASSERT(IsOpen(), "Failed to open file: %ls", path);
	}

	Serializer::~Serializer()
	{
		Close();
	}

	void Serializer::Open(const wchar_t* path, std::ios_base::openmode mode)
	{
		m_stream.open(path, mode);
	}

	bool Serializer::IsOpen() const
	{
		return m_stream.is_open();
	}

	void Serializer::Write(const char* buffer, size_t sizeInBytes)
	{
		m_stream.write(buffer, sizeInBytes);
	}
	void Serializer::Read(char* buffer, size_t sizeInBytes)
	{
		m_stream.read(buffer, sizeInBytes);
	}

	char* Serializer::ReadText()
	{
		std::string content;
		std::string line;
		while (std::getline(m_stream, line))
		{
			content += line.c_str();

			if (!m_stream.eof())
			{
				content += '\n';
			}
		}

		char* buffer = new char[content.size() + 1];
		strcpy_s(buffer, content.size() + 1, content.c_str());
		return buffer;
	}

	void Serializer::Close()
	{
		m_stream.close();
	}
}