// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

#include <fstream>
#include <string>

export module jpt.Serializer;

export namespace jpt
{
	enum class SerializerMode : std::ios_base::openmode
	{
		Read     = std::ios_base::in,
		Write    = std::ios_base::out,
		Append   = std::ios_base::app,
		Binary   = std::ios_base::binary,
		Truncate = std::ios_base::trunc,
		AtEnd    = std::ios_base::ate,

		Begin    = std::ios_base::beg,
		End      = std::ios_base::end,
		Current  = std::ios_base::cur,

		ReadBinary  = Read  | Binary,
		WriteBinary = Write | Binary,
		ReadAll     = Read  | Binary | Truncate,
		WriteAll    = Write | Binary | Truncate,
		ReadWrite   = Read  | Write,

		All = Read | Write | Binary | Truncate,
	};

	class Serializer;

	template<typename T>
	concept SerializationOverridden = requires(T obj, Serializer& serializer)
	{
		obj.Serialize(serializer);
		obj.Deserialize(serializer);
	};

	template<typename T>                constexpr bool IsSerializeOverridden    = false;
	template<SerializationOverridden T> constexpr bool IsSerializeOverridden<T> = true;

	class Serializer
	{
	private:
		std::fstream m_stream;

	public:
		Serializer() = default;
		Serializer(const wchar_t* path, SerializerMode mode);
		~Serializer();

		void Open(const wchar_t* path, SerializerMode mode);
		bool IsOpen() const;
		void Close();

		/** Writes raw buffer for given size to disk */
		void Write(const char* buffer, size_t sizeInBytes);

		/** Writes object without built-in Serialize() to disk */
		template<typename T> 
		requires(!SerializationOverridden<T>)
		void Write(const T& obj);

		/** Writes object with built-in Serialize() to disk */
		template<SerializationOverridden T> 
		void Write(const T& obj);

		/** Reads raw buffer for given size from disk */
		void Read(char* buffer, size_t sizeInBytes);

		/** Reads object without built-in Deserialize() from disk */
		template<typename T> 
		requires(!SerializationOverridden<T>)
		void Read(T& obj);

		/** Reads object with built-in Deserialize() from disk */
		template <SerializationOverridden T>
		void Read(T& obj);

		char* ReadText();

		/** Seeks to a specific position in the file */
		void Seek(size_t position, SerializerMode mode = SerializerMode::Current);

		/** Returns the size of the file in bytes */
		size_t GetSize();
	};

	Serializer::Serializer(const wchar_t* path, SerializerMode mode)
	{
		Open(path, mode);
		JPT_ASSERT(IsOpen(), "Failed to open file: %ls", path);
	}

	Serializer::~Serializer()
	{
		Close();
	}

	void Serializer::Open(const wchar_t* path, SerializerMode mode)
	{
		m_stream.open(path, static_cast<std::ios_base::openmode>(mode));
	}

	bool Serializer::IsOpen() const
	{
		return m_stream.is_open();
	}

	void Serializer::Close()
	{
		m_stream.close();
	}

	void Serializer::Write(const char* buffer, size_t sizeInBytes)
	{
		m_stream.write(buffer, sizeInBytes);
	}

	template<typename T> 
	requires(!SerializationOverridden<T>)
	void Serializer::Write(const T& obj)
	{
		Write(reinterpret_cast<const char*>(&obj), sizeof(T));
	}

	template<SerializationOverridden T>
	void Serializer::Write(const T& obj)
	{
		obj.Serialize(*this);
	}

	void Serializer::Read(char* buffer, size_t sizeInBytes)
	{
		m_stream.read(buffer, sizeInBytes);
	}

	template<typename T> 
	requires(!SerializationOverridden<T>)
	void Serializer::Read(T& obj)
	{
		Read(reinterpret_cast<char*>(&obj), sizeof(T));
	}

	template<SerializationOverridden T>
	void Serializer::Read(T& obj)
	{
		obj.Deserialize(*this);
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

	void Serializer::Seek(size_t position, SerializerMode mode /*= SerializerMode::Current*/)
	{
		m_stream.seekg(position, static_cast<std::ios_base::seekdir>(mode));
	}

	size_t Serializer::GetSize()
	{
		size_t current = m_stream.tellg();

		m_stream.seekg(0, std::ios_base::end);
		size_t size = m_stream.tellg();

		m_stream.seekg(current);
		return size;
	}
}