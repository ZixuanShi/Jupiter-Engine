// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Application_Factories;

import jpt.Framework_Base;
import jpt.Framework_GLFW;

import jpt.Window_Base;
import jpt.Window_GLFW;

namespace jpt
{
	export Framework_Base* Framework_Create()
	{
#if IS_PLATFORM_WIN64
		return new Framework_GLFW();
#else
#error "Framework_Create() is not implemented for this platform."
		return nullptr;
#endif
	}

	export Window_Base* Window_Create()
	{
#if IS_PLATFORM_WIN64
		return new Window_GLFW();
#else
#error "Window_Create() is not implemented for this platform."
		return nullptr;
#endif
	}
}