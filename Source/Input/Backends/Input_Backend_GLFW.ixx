// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"
#include <GLFW/glfw3.h>

export module jpt.Input.Backend_GLFW;

import jpt.Input.Backend_Base;
import jpt.Input.KeyCode;

import jpt.TypeDefs;
import jpt.Constants;

export namespace jpt::Input
{
	class Backend_GLFW final : public Backend_Base
	{
	public:
		virtual bool IsPressed(KeyCode key) const override;
		virtual bool IsReleased(KeyCode key) const override;
		virtual uint32 FromKeyCode(KeyCode key) const override;
	};

	bool Backend_GLFW::IsPressed(KeyCode key) const
	{
		const uint32 glfwKey = FromKeyCode(key);
		if (glfwKey != kInvalidValue<uint32>)
		{
			return glfwGetKey(glfwGetCurrentContext(), glfwKey) == GLFW_PRESS;
		}

		return false;
	}

	bool Backend_GLFW::IsReleased(KeyCode) const
	{
		return false;
	}

	uint32 Backend_GLFW::FromKeyCode(KeyCode key) const
	{
		switch (key.Value())
		{
		case KeyCode::Keyboard_Escape: return GLFW_KEY_ESCAPE;

		default:
			JPT_ERROR("KeyCode \"%s\" is not converted yet", key.ToString().ConstBuffer());
			return kInvalidValue<uint32>;
		}
	}
}