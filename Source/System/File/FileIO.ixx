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
		std::fstream ifstream(path.ConstBuffer(), std::ios::in | std::ios::binary);
		if (!ifstream.is_open())
		{
			JPT_ERROR("Failed to open file: %ls", path.ConstBuffer());
			return Optional<String>();
		}

		String content;
		std::string line;
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

	/** Saves text data to a file */
	bool SaveTextFile(const File::Path& path, const char* data, size_t sizeInBytes)
	{
		std::fstream ofstream(path.ConstBuffer(), std::ios::out | std::ios::binary);
		if (!ofstream.is_open())
		{
			JPT_ERROR("Failed to open file: %ls", path.ConstBuffer());
			return false;
		}

		ofstream.write(data, sizeInBytes);
		ofstream.close();
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
		std::fstream ifstream(path.ConstBuffer(), std::ios::in | std::ios::binary);
		if (!ifstream.is_open())
		{
			JPT_ERROR("Failed to open file: %ls", path.ConstBuffer());
			return false;
		}

		Deserialize(obj, ifstream);

		ifstream.close();
		return true;
	}

	/** Saves binary data to a file */
	template<typename T>
	bool SaveBinaryFile(const File::Path& path, const T& obj)
	{
		std::fstream ofstream(path.ConstBuffer(), std::ios::out | std::ios::binary);
		if (!ofstream.is_open())
		{
			JPT_ERROR("Failed to open file: %ls", path.ConstBuffer());
			return false;
		}

		Serialize(obj, ofstream);

		ofstream.close();
		return true;
	}
}