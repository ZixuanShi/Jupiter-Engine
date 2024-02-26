// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"

#include <mutex>

export module jpt.FileIO;

import jpt.TypeDefs;
import jpt.String;
import jpt.StringView;
import jpt.Optional;

export namespace jpt
{
	namespace FileIO
	{
		/** */
		Optional<String> ReadFile(StringView filePath);

		/** */
		bool WriteFile(StringView filePath, StringView data);

		Optional<String> ReadFile(StringView filePath)
		{
			JPT_IGNORE(filePath);
			return Optional<String>();
		}

		bool WriteFile(StringView filePath, StringView data)
		{
			JPT_IGNORE(filePath, data);
			return false;
		}
	}
}