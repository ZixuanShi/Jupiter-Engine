// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"
#include "Debugging/Assert.h"

#include <filesystem>
#include <fstream>

export module jpt.FileIO;

import jpt.String;
import jpt.String.Helpers;
import jpt.Optional;
import jpt.Serializer;

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

		if (!serializer.IsOpen())
		{
			JPT_ERROR("Failed to open file for reading with SerializerMode::Read: %ls", path.ConstBuffer());
			return Optional<String>();
		}

		char* buffer = serializer.ReadText();

		String content;
		content.MoveString(buffer);
		return content;
	}

	/** Saves text data to a file */
	bool WriteTextFile(const File::Path& path, const char* data, size_t sizeInBytes)
	{
		Serializer serializer(path.ConstBuffer(), SerializerMode::WriteAll);

		if (!serializer.IsOpen())
		{
			JPT_ERROR("Failed to open file for writing with SerializerMode::WriteAll: %ls", path.ConstBuffer());
			return false;
		}

		serializer.Write(data, sizeInBytes);
		return true;
	}
	bool WriteTextFile(const File::Path& path, const char* data)
	{
		return WriteTextFile(path, data, FindCharsCount(data) * sizeof(char));
	}
	bool WriteTextFile(const File::Path& path, const String& data)
	{
		return WriteTextFile(path, data.ConstBuffer(), data.Size());
	}

	/** Loads binary data from a file */
	template<typename T>
	Optional<T> ReadBinaryFile(const File::Path& path)
	{
		Serializer serializer(path.ConstBuffer(), SerializerMode::ReadBinary);

		if (!serializer.IsOpen())
		{
			JPT_ERROR("Failed to open file for reading with SerializerMode::ReadBinary: %ls", path.ConstBuffer());
			return Optional<T>();
		}

		T obj;
		serializer.Read(obj);
		return obj;
	}

	/** Saves binary data to a file */
	template<typename T>
	bool WriteBinaryFile(const File::Path& path, const T& obj)
	{
		Serializer serializer(path.ConstBuffer(), SerializerMode::WriteAll);

		if (!serializer.IsOpen())
		{
			JPT_ERROR("Failed to open file for writing with SerializerMode::WriteAll: %ls", path.ConstBuffer());
			return false;
		}

		serializer.Write(obj);
		return true;
	}
}