// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Logger.h"
#include "Debugging/Assert.h"

#include <filesystem>
#include <fstream>

export module jpt.FileIO;

import jpt.String;
import jpt.Optional;
import jpt.Serializer;
import jpt.SerializationUtils;

import jpt.File.Enums;
import jpt.File.Path;

export namespace jpt::File
{
	/** @return		true if path exists in system. Could be either file or directory */
	bool Exists(const Path& absoluteFullPath)
	{
		std::error_code errorCode;
		const bool result = std::filesystem::exists(absoluteFullPath.ConstBuffer(), errorCode);
		if (errorCode)
		{
			JPT_ERROR("Error checking if file exists: %s", errorCode.message().c_str());
		}

		return result;
	}

	/** Creates a directory and all necessary parent folders */
	bool MakeDirectory(const Path& absoluteFullPath)
	{
		std::error_code errorCode;
		const bool result = std::filesystem::create_directories(absoluteFullPath.ConstBuffer(), errorCode);
		if (errorCode)
		{
			JPT_ERROR("Error creating file: %s", errorCode.message().c_str());
		}

		return result;
	}

	/** Deletes either file or directory */
	bool Delete(const Path& absoluteFullPath)
	{
		std::error_code errorCode;
		const bool result = std::filesystem::remove_all(absoluteFullPath.ConstBuffer(), errorCode);
		if (errorCode)
		{
			JPT_ERROR("Error deleting file: %s", errorCode.message().c_str());
		}

		return result;
	}

	/** @return		String data of a text file */
	Optional<String> ReadTextFile(const File::Path& path)
	{
		Serializer serializer(path.ConstBuffer(), SerializerMode::Read);
		char* buffer = serializer.ReadText();

		jpt::String content;
		content.MoveString(buffer);
		return content;
	}

	/** Saves text data to a file */
	void WriteTextFile(const File::Path& path, const char* data, size_t sizeInBytes)
	{
		Serializer serializer(path.ConstBuffer(), SerializerMode::WriteAll);
		serializer.Write(data, sizeInBytes);
	}
	void WriteTextFile(const File::Path& path, const char* data)
	{
		WriteTextFile(path, data, FindCharsCount(data) * sizeof(char));
	}
	void WriteTextFile(const File::Path& path, const String& data)
	{
		WriteTextFile(path, data.ConstBuffer(), data.Size());
	}

	/** Loads binary data from a file */
	template<typename T>
	void ReadBinaryFile(const File::Path& path, T& obj)
	{
		Serializer serializer(path.ConstBuffer(), SerializerMode::ReadBinary);
		Deserialize(obj, serializer);
	}

	/** Saves binary data to a file */
	template<typename T>
	void WriteBinaryFile(const File::Path& path, const T& obj)
	{
		Serializer serializer(path.ConstBuffer(), SerializerMode::WriteAll);
		Serialize(obj, serializer);
	}
}