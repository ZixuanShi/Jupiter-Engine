// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <fstream>
#include <string>

export module jpt.FileIOUtils;

import jpt.FileEnums;
import jpt.FileTypeDefs;
import jpt.Optional;

export namespace jpt
{
	/** Reads text file's content at given absolute path */
	Optional<FilePath> GetTextFileContent(FilePathView absolutePath)
	{
		std::ifstream file;
		file.open(absolutePath.ConstBuffer());

		if (!file.is_open())
		{
			JPT_ERROR("Failed to open file: %s", absolutePath.ConstBuffer());
			return Optional<FilePath>();
		}

		std::string line;
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