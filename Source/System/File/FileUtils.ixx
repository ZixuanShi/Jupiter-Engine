// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Types/Enum.h"
#include "Debugging/Assert.h"

export module jpt.FileUtils;

export namespace jpt
{
	namespace FileUtils
	{
		enum class ESource : uint8
		{
			Unknown,
			Engine,
			Client,
		};

		JPT_ENUM_UINT8(EType,
			Unknown,
			txt,
			json,
		);
	}
}