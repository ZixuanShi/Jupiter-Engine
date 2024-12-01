// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"
#include "Core/Validation/Assert.h"

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
		Invalid,

		Pressed,    // Newly pressed in last frame
		Released,   // Newly released in last frame

		Down,       // Currently down. No matter if it's newly pressed in last frame
		Up,         // Currently up. No matter if it's newly released in last frame

		Held,       // Pressed for more than one frame
		Smashed,    // Repeatedly pressed within a short time

		ItemCount,
	};
}