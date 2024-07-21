// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <GLFW/glfw3.h>

export module Window_GLFW;

import jpt.Window_Base;

namespace jpt
{
	export class Window_GLFW : public Window_Base
	{
	private:
		GLFWwindow* m_pWindow = nullptr;

	public:

	};
}