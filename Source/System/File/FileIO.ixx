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
import jpt.StrongPtr;
import jpt.TypeDefs;

export namespace jpt
{
	Optional<StrongPtr<BaseFile>> ReadFile(const String& absolutePath)
	{
		Optional<String> content = GetTextFileContent(absolutePath);

		if (!content)
		{
			return Optional<StrongPtr<BaseFile>>();
		}

		StrongPtr<BaseFile> file;
		return file;
	}

	Optional<StrongPtr<BaseFile>> ReadFile(ESource source, StringView relativePath)
	{
		return ReadFile(GetAbsolutePath(source, relativePath));
	}
}