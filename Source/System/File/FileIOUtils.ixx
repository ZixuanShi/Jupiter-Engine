// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Logger.h"

#include <fstream>
#include <string>

export module jpt.FileIOUtils;

import jpt.Constants;
import jpt.File_Text;
import jpt.FileEnums;
import jpt.FilePath;
import jpt.Optional;
import jpt.TypeTraits;

export namespace jpt
{
	/** Reads text file's content at given absolute path */
	Optional<File_Text> LoadTextFile(const FilePath& absolutePath)
	{
		using TChar = typename FilePath::TChar;

		std::basic_ifstream<TChar> file;
		file.open(absolutePath.ConstBuffer());

		if (!file.is_open())
		{
			JPT_ERROR("Failed to open file: %s", absolutePath.ConstBuffer());
			return Optional<File_Text>();
		}

		std::basic_string<TChar> line;
		WString content;
		while (std::getline(file, line))
		{
			content += line.c_str();
			content += '\n';
		}

		file.close();

		JPT_IGNORE(content);
		return File_Text();
	}
}