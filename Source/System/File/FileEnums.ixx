// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"
#include "Debugging/Assert.h"

export module jpt.File.Enums;

import jpt.TypeDefs;

export namespace jpt::File
{
	enum class Source : uint8
	{
		Engine,
		Client,		// Client development folder
		Output,		// Build output folder
		Saved,		
	};

	/** Supported file types */
	JPT_ENUM_UINT8(EExtension,
		Unknown,
		txt,
		json,
		bin,
	);

	enum class EType : uint8
	{
		Unknown,
		Text,
		Binary,
	};
}
