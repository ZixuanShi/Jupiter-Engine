// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"
#include "Debugging/Assert.h"

export module jpt.Input.Enums;

export namespace jpt::Input
{
	JPT_ENUM_UINT8(Type,
		Keyboard,
		Mouse,
		Gamepad,
		ItemCount,
		Unknown,
	);
}