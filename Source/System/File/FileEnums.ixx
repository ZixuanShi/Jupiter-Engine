// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"
#include "Debugging/Assert.h"

export module jpt.FileEnums;

import jpt.TypeDefs;

export namespace jpt
{
	enum class ESource : uint8
	{
		Unknown,
		Engine,
		Client,
	};

	/** Supported file types */
	JPT_ENUM_UINT8(EExtension,
		Unknown,
		txt,
		json
	);

	enum class EFileType : uint8
	{
		Unknown,
		Text,
		Binary,
	};
}
