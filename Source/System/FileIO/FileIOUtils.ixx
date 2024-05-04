// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <fstream>
#include <string>

export module jpt.FileIOUtils;

import jpt.Constants;
import jpt.TypeTraits;
import jpt.FileEnums;
import jpt.FileTypeDefs;
import jpt.Optional;

export namespace jpt
{
	/** Reads text file's content at given absolute path */
	Optional<FilePath> GetTextFileContent(FilePathView absolutePath)
	{
		using TChar = typename FilePathView::TChar;

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

	/** @return		    File's extension of the given path
		@param path		Can be either relative or absolute. As long as it ends with .<type> format
		@example		GetExtension("Assets/Config/JupiterEngine.json") will return json */
	EExtension GetExtension(FilePathView path)
	{
		const size_t dotPos = path.FindLastOf('.');
		if (dotPos == npos)
		{
			JPT_ERROR("Failed to find extension in path: %s", path.ConstBuffer());
			return EExtension::Unknown;
		}

		// TODO
		return EExtension::Unknown;
	}
}