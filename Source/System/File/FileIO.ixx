// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Logger.h"
#include "Debugging/Assert.h"

#include <filesystem>
#include <fstream>

export module jpt.FileIO;

import jpt.File.Enums;
import jpt.File.Path;
import jpt.File.PathUtils;

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

	// Delete
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

	// Write


	// Close
}