// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

export module jpt.InputManager.Create;

import jpt.InputManager;
import jpt.InputManager_GLFW;

import jpt.Framework.Enums;

export namespace jpt
{
	InputManager* InputManager_Create(Framework_API frameworkAPI)
	{
		switch (frameworkAPI.Value())
		{
		case Framework_API::GLFW:
			return new Input::InputManager_GLFW();

		default:
			JPT_ERROR("Unsupported framework API: " + frameworkAPI.ToString());
			return nullptr;
		}
	}
}