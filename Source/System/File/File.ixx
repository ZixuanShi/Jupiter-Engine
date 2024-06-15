// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <fstream>
#include <string>

export module jpt.File;

import jpt.Any;
import jpt.Utilities;
import jpt.File.Path;

export namespace jpt::File
{
	/** Represents a File in the system
		Could contain text, binary, or any custom data

		Examples:

		// Save text file
		File saver;
		saver.SetText(L"Hello World");
		saver.Save({ ESource::Client, "Assets/NewTextFile.txt" });	// Will create a new file or overwrite existing one at Client directory's Assets/NewTextFile.txt with the content of "Hello World"

		// Load text file
		File loader;
		loader.Load({ ESource::Client, "Assets/NewTextFile.txt" });	// Will load the content of the file at Client directory's Assets/NewTextFile.txt into loader's data.
		JPT_LOG(loader.GetText());	// Will print "Hello World"

		// Save binary file
		File binarySaver;
		binarySaver.SetData<int>(42);
		binarySaver.SaveBinary<int>({ ESource::Client, "Assets/NewBinaryFile.bin" });	// Will create a new file or overwrite existing one at Client directory's Assets/NewBinaryFile.bin with the content of 42

		// Load binary file
		File binaryLoader;
		binaryLoader.LoadBinary<int>({ ESource::Client, "Assets/NewBinaryFile.bin" });	// Will load the content of the file at Client directory's Assets/NewBinaryFile.bin into loader's data.
		JPT_LOG(binaryLoader.GetData<int>());	// Will print 42
	*/
	class File
	{
	private:
		Path m_path;	/**< Absolute full path to this file */
		Any m_data;		/**< Data of this file. Could be WString for text, Custom type, etc. */

	public:
		constexpr File() = default;
		constexpr File(const Path& path);

		// Text
		bool Load(const Path& absoluteFullPath);
		bool Save(const Path& absoluteFullPath);

		// Binary
		template<typename T> bool LoadBinary(const Path& absoluteFullPath);
		template<typename T> bool SaveBinary(const Path& absoluteFullPath);

		// Data
		void SetText(const WString& text) { m_data = text; }
		const WString& GetText() const { return m_data.As<WString>(); }

		template<typename T> void SetData(const T& data) { m_data = data; }
		template<typename T>       T& GetData()       { return m_data.As<T>(); }
		template<typename T> const T& GetData() const { return m_data.As<T>(); }

		// Path
		const Path& GetPath() const { return m_path; }
	};

	constexpr File::File(const Path& path)
		: m_path(path)
	{
	}

	bool File::Load(const Path& absoluteFullPath)
	{
		std::wifstream ifstream(absoluteFullPath.ConstBuffer(), std::ios::in);
		if (!ifstream.is_open())
		{
			JPT_ERROR("Failed to open file: %s", absoluteFullPath.ConstBuffer());
			return false;
		}

		m_path = absoluteFullPath;
		m_data = WString();
		WString& content = m_data.As<WString>();

		std::wstring line;
		while (std::getline(ifstream, line))
		{
			content += line.c_str();

			if (!ifstream.eof())
			{
				content += '\n';
			}
		}

		ifstream.close();
		return true;
	}

	bool File::Save(const Path& absoluteFullPath)
	{
		std::wofstream ofstream(absoluteFullPath.ConstBuffer(), std::ios::out);
		if (!ofstream.is_open())
		{
			JPT_ERROR("Failed to open file: %s", absoluteFullPath.ConstBuffer());
			return false;
		}

		ofstream.write(m_data.As<WString>().ConstBuffer(), m_data.As<WString>().Count());
		ofstream.close();
		return true;
	}

	template<typename T>
	bool File::LoadBinary(const Path& absoluteFullPath)
	{
		std::basic_ifstream<char> ifstream(absoluteFullPath.ConstBuffer(), std::ios::binary);
		if (!ifstream.is_open())
		{
			JPT_ERROR("Failed to open file: %s", absoluteFullPath.ConstBuffer());
			return false;
		}

		m_path = absoluteFullPath;

		T data;
		ifstream.read(reinterpret_cast<char*>(&data), sizeof(T));
		SetData<T>(data);

		ifstream.close();
		return true;
	}

	template<typename T>
	bool File::SaveBinary(const Path& absoluteFullPath)
	{
		std::basic_ofstream<char> ofstream(absoluteFullPath.ConstBuffer(), std::ios::binary);
		if (!ofstream.is_open())
		{
			JPT_ERROR("Failed to open file: %s", absoluteFullPath.ConstBuffer());
			return false;
		}

		ofstream.write(reinterpret_cast<char*>(&m_data.As<T>()), sizeof(T));
		ofstream.close();
		return true;
	}
}