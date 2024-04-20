// Copyright Jupiter Technologies, Inc. All Rights Reserved.

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

	enum class EFileType : uint8
	{
		Unknown,
		txt,
		json,
	};
}
