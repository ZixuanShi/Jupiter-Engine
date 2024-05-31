// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Logger.h"

#include <fstream>
#include <string>

export module jpt.FileIOUtils;

import jpt.Constants;
import jpt.Optional;
import jpt.TypeTraits;

import jpt.File_Text;
import jpt.File.Enums;
import jpt.File.Path;

export namespace jpt::File
{
	/** Reads text file's content at given absolute path */
	Optional<File_Text> LoadTextFile(const Path& absolutePath)
	{
		using TChar = typename Path::TChar;

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