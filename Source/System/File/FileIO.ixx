// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"

#include <mutex>

export module jpt.FileIO;

import jpt.File;
import jpt.TypeDefs;
import jpt.String;
import jpt.StringView;
import jpt.Optional;

export namespace jpt
{
	namespace FileIO
	{
		/** */
		Optional<File> Read(StringView filePath)
		{
			JPT_IGNORE(filePath);
			return Optional<File>();

			//std::ifstream file(filePath.data(), std::ios::binary);
			//if (!file.is_open())
			//{
			//	return {};
			//}

			//File data;
			//file.seekg(0, std::ios::end);
			//data.size = file.tellg();
			//file.seekg(0, std::ios::beg);

			//data.data.resize(data.size);
			//file.read(data.data.data(), data.size);

			//return data;
		}

		/** */
		bool Write(StringView filePath, const File& data)
		{
			JPT_IGNORE(filePath, data);
			return true;

			//std::ofstream file(filePath.data(), std::ios::binary);
			//if (!file.is_open())
			//{
			//	return false;
			//}

			//file.write(data.data.data(), data.size);
			//return true;
		}
	}
}