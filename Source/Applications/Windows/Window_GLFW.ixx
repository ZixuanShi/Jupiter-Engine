// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <GLFW/glfw3.h>

export module jpt.Window_GLFW;

import jpt.Window;
import jpt.TypeDefs;
import jpt.Vector2;

namespace jpt
{
	export class Window_GLFW final : public Window
	{
		using Super = Window;

	private:
		GLFWwindow* m_pGLFWWindow = nullptr;

	public:
		virtual bool Init(const char* title, int32 width, int32 height) override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

		virtual bool CreateSurface(const DynamicArray<Any>& context) override;
		virtual void SetMousePosition(Vec2i position) override;
		virtual void SetCursorVisible(bool isVisible) override;

	public:
		virtual bool ShouldClose() const override;

		GLFWwindow* GetGLFWWindow() const { return m_pGLFWWindow; }
	};
}