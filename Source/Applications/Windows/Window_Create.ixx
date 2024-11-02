// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Window.Create;

import jpt.Window;
import jpt.Window_GLFW;

export namespace jpt
{
	Window* Window_Create()
	{
#if IS_PLATFORM_WIN64
		return new Window_GLFW();
#else
#error "Window_Create() is not implemented for this platform."
		return nullptr;
#endif
	}
}