// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"
#include "Core/Validation/Assert.h"

#include <filesystem>
#include <fstream>

export module jpt.File.IO;

import jpt.String;
import jpt.String.Helpers;
import jpt.Optional;
import jpt.Serializer;

import jpt.File.Enums;
import jpt.File.Path;

import jpt.DynamicArray;

export namespace jpt::File
{
	/** @return		true if path exists in system. Could be either file or directory */
	bool Exists(const Path& absoluteFullPath)
	{
		if (absoluteFullPath.IsEmpty()) [[unlikely]]
		{
			return false;
		}

		std::error_code errorCode;
		const bool result = std::filesystem::exists(absoluteFullPath.ConstBuffer(), errorCode);
		if (errorCode) [[unlikely]]
		{
			JPT_ERROR("Error checking if file exists (%ls): %s", absoluteFullPath.ConstBuffer(), errorCode.message().c_str());
		}

		return result;
	}

	/** Creates a directory and all necessary parent folders */
	bool MakeDirectory(const Path& absoluteFullPath)
	{
		std::error_code errorCode;
		std::filesystem::create_directories(absoluteFullPath.ConstBuffer(), errorCode);
		if (errorCode) [[unlikely]]
		{
			JPT_ERROR("Error creating directory (%ls): %s", absoluteFullPath.ConstBuffer(), errorCode.message().c_str());
			return false;
		}

		return true;
	}

	/** Ensures path's parent folder exists */
	bool EnsureParentDirExists(const Path& absoluteFullPath)
	{
		if (absoluteFullPath.IsEmpty()) [[unlikely]]
		{
			return false;
		}

		const Path parentPath = absoluteFullPath.GetParent();
		if (!Exists(parentPath)) [[unlikely]]
		{
			return MakeDirectory(parentPath);
		}

		return true;
	}

	/** Deletes either file or directory */
	bool Delete(const Path& absoluteFullPath)
	{
		std::error_code errorCode;
		const bool result = std::filesystem::remove_all(absoluteFullPath.ConstBuffer(), errorCode);
		if (errorCode) [[unlikely]]
		{
			JPT_ERROR("Error deleting file (%ls): %s", absoluteFullPath.ConstBuffer(), errorCode.message().c_str());
		}

		return result;
	}

	/** @return		String data of a text file */
	Optional<String> ReadTextFile(const Path& path, SerializerMode mode = SerializerMode::Read)
	{
		Serializer serializer(path.ConstBuffer(), mode);

		if (!serializer.IsOpen()) [[unlikely]]
		{
			JPT_ERROR("Failed to open file for reading with SerializerMode::%u: %ls", static_cast<uint32>(mode), path.ConstBuffer());
			return Optional<String>();
		}

		char* buffer = serializer.ReadText();

		String content;
		content.MoveString(buffer);
		return content;
	}

	/** Saves text data to a file */
	bool WriteTextFile(const Path& path, const char* data, size_t sizeInBytes, SerializerMode mode = SerializerMode::WriteAll)
	{
		EnsureParentDirExists(path);
		Serializer serializer(path.ConstBuffer(), mode);

		if (!serializer.IsOpen()) [[unlikely]]
		{
			JPT_ERROR("Failed to open file for writing with SerializerMode::%u: %ls", static_cast<uint32>(mode), path.ConstBuffer());
			return false;
		}

		serializer.Write(data, sizeInBytes);
		return true;
	}
	bool WriteTextFile(const Path& path, const char* data, SerializerMode mode = SerializerMode::WriteAll)
	{
		return WriteTextFile(path, data, FindCharsCount(data) * sizeof(char), mode);
	}
	bool WriteTextFile(const Path& path, const String& data, SerializerMode mode = SerializerMode::WriteAll)
	{
		return WriteTextFile(path, data.ConstBuffer(), data.Size(), mode);
	}

	/** Appends content to file on disk. Write if not present */
	bool AppendTextFile(const Path& path, const char* data, size_t sizeInBytes, SerializerMode mode = SerializerMode::Append)
	{
		if(!EnsureParentDirExists(path))[[unlikely]]
		{
			return false;
		}

		Serializer serializer(path.ConstBuffer(), mode);

		if (!serializer.IsOpen()) [[unlikely]]
		{
			JPT_ERROR("Failed to open file for writing with SerializerMode::%u: %ls", static_cast<uint32>(mode), path.ConstBuffer());
			return false;
		}

		serializer.Write(data, sizeInBytes);
		return true;
	}
	bool AppendTextFile(const Path& path, const char* data, SerializerMode mode = SerializerMode::Append)
	{
		return AppendTextFile(path, data, FindCharsCount(data) * sizeof(char), mode);
	}
	bool AppendTextFile(const Path& path, const String& data, SerializerMode mode = SerializerMode::Append)
	{
		return AppendTextFile(path, data.ConstBuffer(), data.Size(), mode);
	}

	/** Loads binary data from a file */
	template<typename T>
	Optional<T> ReadBinaryFile(const Path& path, SerializerMode mode = SerializerMode::ReadBinary)
	{
		Serializer serializer(path.ConstBuffer(), mode);

		if (!serializer.IsOpen()) [[unlikely]]
		{
			JPT_ERROR("Failed to open file for reading with SerializerMode::%u: %ls", static_cast<uint32>(mode), path.ConstBuffer());
			return Optional<T>();
		}

		T obj;
		serializer.Read(obj);
		return obj;
	}

	DynamicArray<char> ReadBinaryFileArray(const Path& path, SerializerMode mode = SerializerMode::ReadBinary)
	{
		Serializer serializer(path.ConstBuffer(), mode);

		if (!serializer.IsOpen()) [[unlikely]]
		{
			JPT_ERROR("Failed to open file for reading with SerializerMode::%u: %ls", static_cast<uint32>(mode), path.ConstBuffer());
			return {};
		}

		const size_t fileSize = serializer.GetSize();
		DynamicArray<char> buffer(fileSize);

		serializer.Read(buffer.Buffer(), fileSize);

		serializer.Close();
		return buffer;
	}

	/** Saves binary data to a file */
	template<typename T>
	bool WriteBinaryFile(const Path& path, const T& obj, SerializerMode mode = SerializerMode::WriteAll)
	{
		Serializer serializer(path.ConstBuffer(), mode);

		if (!serializer.IsOpen()) [[unlikely]]
		{
			JPT_ERROR("Failed to open file for writing with SerializerMode::%u: %ls", static_cast<uint32>(mode), path.ConstBuffer());
			return false;
		}

		serializer.Write(obj);
		return true;
	}
}