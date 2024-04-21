// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Logger.h"
#include "Debugging/Assert.h"

#include <fstream>
#include <mutex>
#include <string>

export module jpt.FileIO;

import jpt.BaseFile;
import jpt.FileEnums;
import jpt.FileTypeDefs;
import jpt.FileUtils;
import jpt.Optional;
import jpt.String;
import jpt.StringView;
import jpt.TypeDefs;

export namespace jpt
{
	Optional<BaseFile> ReadFile(const FilePath& absolutePath)
	{
		// Parse directory, file name, and extension

		Optional<String> content = GetTextFileContent(absolutePath);

		if (!content)
		{
			return Optional<BaseFile>();
		}

		BaseFile file;
		return file;
	}

	Optional<BaseFile> ReadFile(ESource source, FilePathView relativePath)
	{
		return ReadFile(GetAbsolutePath(source, relativePath));
	}
}