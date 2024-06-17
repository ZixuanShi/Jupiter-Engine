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
	Optional<String> LoadTextFile(const File::Path& path)
	{
		Serializer serializer(path.ConstBuffer(), std::ios::in | std::ios::binary);
		jpt::String content;
		content.MoveString(serializer.ReadText());
		return content;
	}

	/** Saves text data to a file */
	bool SaveTextFile(const File::Path& path, const char* data, size_t sizeInBytes)
	{
		Serializer serializer(path.ConstBuffer(), std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
		serializer.Write(data, sizeInBytes);
		return true;
	}
	bool SaveTextFile(const File::Path& path, const char* data)
	{
		return SaveTextFile(path, data, FindCharsCount(data) * sizeof(char));
	}

	/** Loads binary data from a file */
	template<typename T>
	bool LoadBinaryFile(const File::Path& path, T& obj)
	{
		Serializer serializer(path.ConstBuffer(), std::ios::in | std::ios::binary);
		Deserialize(obj, serializer);
		return true;
	}

	/** Saves binary data to a file */
	template<typename T>
	bool SaveBinaryFile(const File::Path& path, const T& obj)
	{
		Serializer serializer(path.ConstBuffer(), std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
		Serialize(obj, serializer);
		return true;
	}
}