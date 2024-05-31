// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Logger.h"
#include "Debugging/Assert.h"

#include <filesystem>

export module jpt.FileIO;

import jpt.File_Base;
import jpt.File.Enums;
import jpt.File.Path;
import jpt.File.PathUtils;

export namespace jpt::File
{
	// Exists
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
	bool Exists(ESource source, const Path& relativePath)
	{
		return Exists(GetAbsoluteFullPath(source, relativePath));
	}

	// Open


	// Read


	// Write


	// Close
}