// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"
#include "Core/Validation/Assert.h"

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

		Baked
	};
}
