// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Window.Create;

import jpt.Window;
import jpt.Window_GLFW;

import jpt.Framework.Enums;

export namespace jpt
{
	Window* Window_Create(Framework_API api)
	{
		switch (api.Value())
		{
		case Framework_API::GLFW:
			return new Window_GLFW();

		default:
			JPT_ASSERT(false, "Un-implemented Framework API: %s", api.ToString().ConstBuffer());
			return nullptr;
		}
	}
}