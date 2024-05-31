// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"
#include "Debugging/Assert.h"

export module jpt.File.Enums;

import jpt.TypeDefs;

export namespace jpt::File
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

	enum class EType : uint8
	{
		Unknown,
		Text,
		Binary,
	};
}
