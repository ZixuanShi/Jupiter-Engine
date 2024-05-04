// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Logger.h"
#include "Debugging/Assert.h"

#include <fstream>
#include <mutex>
#include <string>

export module jpt.File.IO;

import jpt.File.Base;
import jpt.File.Enums;
import jpt.File.Path;
import jpt.File.Path.Utils;
import jpt.File.Utils;
import jpt.File.IO.Utils;
import jpt.Optional;
import jpt.String;
import jpt.StringView;
import jpt.TypeDefs;

export namespace jpt
{
	Optional<File_Base> ReadFile(const FilePath& absoluteFullPath)
	{
		// Get extension to determine file type
		//EExtension extension = GetExtension(absoluteFullPath);

		Optional<FilePath> content = GetTextFileContent(absoluteFullPath);

		if (!content)
		{
			return Optional<File_Base>();
		}

		File_Base file;
		return file;
	}

	Optional<File_Base> ReadFile(ESource source, const FilePath& relativePath)
	{
		return ReadFile(GetAbsoluteFullPath(source, relativePath));
	}
}