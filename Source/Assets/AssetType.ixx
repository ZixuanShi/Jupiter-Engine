// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"

export module jpt.Asset.Type;

export namespace jpt
{
	JPT_ENUM_UINT8(AssetType, 
		Texture, 
		Shader,
		Model,
		);
}