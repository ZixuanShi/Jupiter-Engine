// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <fstream>
#include <string>

export module jpt.FileIOUtils;

import jpt.Constants;
import jpt.FileEnums;
import jpt.FilePath;
import jpt.Optional;
import jpt.TypeTraits;

export namespace jpt
{
	/** Reads text file's content at given absolute path */
	// TODO: This API is definitely having wrong return type. It should be returning a "File_Text" instead of FilePath
	Optional<FilePath> LoadTextFile(const FilePath& absolutePath)
	{
		using TChar = typename FilePath::TChar;

		std::basic_ifstream<TChar> file;
		file.open(absolutePath.ConstBuffer());

		if (!file.is_open())
		{
			JPT_ERROR("Failed to open file: %s", absolutePath.ConstBuffer());
			return Optional<FilePath>();
		}

		std::basic_string<TChar> line;
		FilePath content;
		while (std::getline(file, line))
		{
			content += line.c_str();
			content += '\n';
		}

		file.close();

		return content;
	}
}