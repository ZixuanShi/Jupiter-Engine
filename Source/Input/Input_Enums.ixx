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

	enum class KeyState
	{
		Free,	 // Not pressed
		Pressed,
		Released,
		Held,    // Pressed for more than one frame
		Smashed, // Repeatedly pressed within a short time
		Unknown,
		ItemCount,
	};
}