// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Minimal/Macros.h"
#include "Core/Types/Enum.h"
#include "Debugging/Assert.h"

#include <fstream>
#include <mutex>
#include <string>

export module jpt.FileUtils;

import jpt.FileEnums;
import jpt.FileTypeDefs;
import jpt.String;
import jpt.StringView;
import jpt.Optional;

export namespace jpt
{
	/** Designed to be implemented in client project's application
		@return Client's project root directory. */
	const char* GetClientDir();

	/** Replaces directory slashes to platform-correct version */
	template<StringType TString>
	void FixSlashes(TString& path)
	{
		using TChar = typename TString::TChar;

		path.Replace(JPT_GET_PROPER_STRING(TChar, \\), JPT_GET_PROPER_STRING(TChar, / ));
	}

	/** Returns the absolute path of the given relative path */
	FilePath GetAbsolutePath(ESource source, FilePathView relativePath)
	{
		FilePath result;

		switch (source)
		{
		case ESource::Engine:
		{
			result.Append(JPT_ENGINE_DIR);
			break;
		}
		case ESource::Client:
		{
			result.Append(GetClientDir());
			break;
		}

		default:
			JPT_ASSERT(false, "Invalid source");
			break;
		}

		result.Append(relativePath.ConstBuffer(), relativePath.Count());
		return result;
	}

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