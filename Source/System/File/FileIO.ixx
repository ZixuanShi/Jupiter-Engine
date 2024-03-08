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
import jpt.FileUtils;
import jpt.TypeDefs;
import jpt.String;
import jpt.StringView;
import jpt.Optional;
import jpt.StrongPtr;

namespace jpt_private
{
	using namespace jpt;

	String GetAbsolutePath(FileUtils::ESource source, StringView relativePath)
	{
		String result;

		switch (source)
		{
			case FileUtils::ESource::Engine:
			{
				result += JPT_ENGINE_DIR;
				break;
			}
			case FileUtils::ESource::Client:
			{
				result += JPT_CLIENT_DIR;
				break;
			}

		default:
			JPT_ASSERT(false, "Invalid source");
			break;
		}

		result += relativePath.Buffer();
		return result;
	}

	Optional<String> GetFileContent(const String& absolutePath)
	{
		std::ifstream file;
		file.open(absolutePath.ConstBuffer());

		if(!file.is_open())
		{
			JPT_ERROR("Failed to open file: %s", absolutePath.ConstBuffer());
			return Optional<String>();
		}

		std::string line;
		String content;
		while (std::getline(file, line))
		{
			content += line.c_str();
			content += '\n';
		}

		file.close();

		return content;
	}
}

export namespace jpt
{
	namespace FileIO
	{
		Optional<StrongPtr<BaseFile>> Read(const String& absolutePath)
		{
			Optional<String> content = jpt_private::GetFileContent(absolutePath);

			if (!content)
			{
				return Optional<StrongPtr<BaseFile>>();
			}

			StrongPtr<BaseFile> file;
			return file;
		}

		Optional<StrongPtr<BaseFile>> Read(FileUtils::ESource source, StringView relativePath)
		{
			return Read(jpt_private::GetAbsolutePath(source, relativePath));
		}
	}
}