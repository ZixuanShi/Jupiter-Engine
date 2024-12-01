// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module jpt.Input.Create;

import jpt.RawInput;
import jpt.RawInput_GLFW;

import jpt.Framework.Enums;

export namespace jpt
{
	RawInput* RawInput_Create(Framework_API frameworkAPI)
	{
		switch (frameworkAPI.Value())
		{
		case Framework_API::GLFW:
			return new Input::RawInput_GLFW();

		default:
			JPT_ERROR("Unsupported framework API: " + frameworkAPI.ToString());
			return nullptr;
		}
	}
}