// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <fstream>
#include <string>

export module jpt.Serializer;

import jpt.String;
import jpt.Optional;
import jpt.File.Path;
import jpt.StringUtils;

export namespace jpt
{
	template<typename T>
	concept Serializable = requires(T obj, std::ifstream& is, std::ofstream& os)
	{
		obj.Serialize(os);
		obj.Deserialize(is);
	};

	template<Serializable T>
	void Serialize(const T& obj, std::ofstream& os)
	{
		obj.Serialize(os);
	}

	template<Serializable T>
	void Deserialize(T& obj, std::ifstream& is)
	{
		obj.Deserialize(is);
	}

	template<typename T> requires(!Serializable<T>)
	void Serialize(const T& obj, std::ofstream& os)
	{
		os.write(reinterpret_cast<const char*>(&obj), sizeof(T));
	}

	template<typename T> requires(!Serializable<T>)
	void Deserialize(T& obj, std::ifstream& is)
	{
		is.read(reinterpret_cast<char*>(&obj), sizeof(T));
	}

	/** @example:     
		jpt::Optional<jpt::String> content = jpt::DeserializeTextFile<jpt::String>({ ESource::Client, "Assets/Configs/TestJson.json" });
		JPT_LOG(content.Value()); */
	template<StringType TString>
	Optional<TString> LoadTextFile(const File::Path& path)
	{
		using TChar = TString::TChar;

		std::basic_ifstream<TChar> ifstream(path.ConstBuffer(), std::ios::in);
		if (!ifstream.is_open())
		{
			JPT_ERROR("Failed to open file: %s", path.ConstBuffer());
			return Optional<TString>();
		}

		TString content;
		std::basic_string<TChar> line;
		while (std::getline(ifstream, line))
		{
			content += line.c_str();

			if (!ifstream.eof())
			{
				content += '\n';
			}
		}

		ifstream.close();
		return content;
	}

	/** @example:
		const char* content = "Hello World";
		jpt::SerializeTextFile<char>({ ESource::Client, "Assets/Configs/UnitTestResults.txt" }, content, jpt::FindCharsCount(content)); */
	template<typename TChar>
	bool SaveTextFile(const File::Path& path, const TChar* data, size_t sizeInBytes)
	{
		std::basic_ofstream<TChar> ofstream(path.ConstBuffer(), std::ios::out);
		if (!ofstream.is_open())
		{
			JPT_ERROR("Failed to open file: %s", path.ConstBuffer());
			return false;
		}

		ofstream.write(data, sizeInBytes);
		ofstream.close();
		return true;
	}
	template<typename TChar>
	bool SaveTextFile(const File::Path& path, const TChar* data)
	{
		return SaveTextFile(path, data, FindCharsCount(data));
	}

	template<typename T>
	bool LoadBinaryFile(const File::Path& path, T& obj)
	{
		std::basic_ifstream<char> ifstream(path.ConstBuffer(), std::ios::binary);
		if (!ifstream.is_open())
		{
			JPT_ERROR("Failed to open file: %s", path.ConstBuffer());
			return false;
		}

		Deserialize(obj, ifstream);

		ifstream.close();
		return true;
	}

	template<typename T>
	bool SaveBinaryFile(const File::Path& path, const T& obj)
	{
		std::basic_ofstream<char> ofstream(path.ConstBuffer(), std::ios::binary);
		if (!ofstream.is_open())
		{
			JPT_ERROR("Failed to open file: %s", path.ConstBuffer());
			return false;
		}

		Serialize(obj, ofstream);

		ofstream.close();
		return true;
	}
}