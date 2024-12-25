// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

export module jpt.Window_GLFW;

import jpt.Window;

import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Vector2;
import jpt.Any;
import jpt.DynamicArray;

import jpt.Input.KeyCode;
import jpt.Input.Manager;
import jpt.RawInput;

import jpt.Time.TypeDefs;

import jpt.Graphics.Enums;

import jpt.Event.Manager;
import jpt.Event.Window.Resize;
import jpt.Event.Window.Close;
import jpt.Event.Mouse.ButtonPress;
import jpt.Event.Keyboard.KeyPress;

namespace jpt
{
	namespace Callbacks
	{
		void OnWindowClose(GLFWwindow* pGLFWWindow);
		void OnWindowResize(GLFWwindow* pGLFWWindow, int32 width, int32 height);

		void OnMouseButton(GLFWwindow* pGLFWWindow, int32 button, int32 action, int32 mods);
		void OnKey(GLFWwindow* pGLFWWindow, int32 key, int32 scancode, int32 action, int32 mods);
	}

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

	public:
		virtual bool ShouldClose() const override;

		GLFWwindow* GetGLFWWindow() const { return m_pGLFWWindow; }
	};

	bool Window_GLFW::Init(const char* title, int32 width, int32 height)
	{
		JPT_ENSURE(Super::Init(title, width, height));

		// Create GLFW window
		m_pGLFWWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!m_pGLFWWindow)
		{
			JPT_ERROR("Failed to create GLFW window");
			EventManager::GetInstance().Send(Event_Window_Close{ this });
			return false;
		}

		// Set current window callbacks
		glfwSetWindowUserPointer(m_pGLFWWindow, this);

		// Window
		glfwSetWindowCloseCallback(m_pGLFWWindow, Callbacks::OnWindowClose);
		glfwSetFramebufferSizeCallback(m_pGLFWWindow, Callbacks::OnWindowResize);

		// Input
		glfwSetMouseButtonCallback(m_pGLFWWindow, Callbacks::OnMouseButton);
		glfwSetKeyCallback(m_pGLFWWindow, Callbacks::OnKey);

		return true;
	}

	void Window_GLFW::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);
	}

	void Window_GLFW::Shutdown()
	{
		glfwDestroyWindow(m_pGLFWWindow);
		m_pGLFWWindow = nullptr;

		Super::Shutdown();
	}

	bool Window_GLFW::CreateSurface(const DynamicArray<Any>& context)
	{
		const Graphics_API api = GetApplication()->GetGraphicsAPI();

		switch (api.Value())
		{
		case Graphics_API::Vulkan:
		{
			VkInstance instance = context[0].As<VkInstance>();
			VkSurfaceKHR* pSurface = context[1].As<VkSurfaceKHR*>();
			return glfwCreateWindowSurface(instance, m_pGLFWWindow, nullptr, pSurface) == VK_SUCCESS;
		}
		default:
			JPT_ASSERT(false, "Un-implemented Graphics API: %s", api.ToString().ConstBuffer());
			return false;
		};
	}

	bool Window_GLFW::ShouldClose() const
	{
		return glfwWindowShouldClose(m_pGLFWWindow);
	}

	namespace Callbacks
	{
		void OnWindowClose(GLFWwindow* pGLFWWindow)
		{
			Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGLFWWindow));
			JPT_ASSERT(pWindow, "Couldn't cast window user pointer to jpt::Window");

			Event_Window_Close eventWindowClose = { pWindow };
			EventManager::GetInstance().Send(eventWindowClose);
		}

		void OnWindowResize(GLFWwindow* pGLFWWindow, int32 width, int32 height)
		{
			Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGLFWWindow));
			JPT_ASSERT(pWindow, "Couldn't cast window user pointer to jpt::Window");

			pWindow->ResizeFrame(Vec2i(width, height));

			Event_Window_Resize eventWindowResize = { pWindow, width, height };
			EventManager::GetInstance().Send(eventWindowResize);
		}

		void OnMouseButton(GLFWwindow* pGLFWWindow, int32 button, int32 action, [[maybe_unused]] int32 mods)
		{
			if (action == GLFW_PRESS)
			{
				Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGLFWWindow));

				double x, y;
				glfwGetCursorPos(pGLFWWindow, &x, &y);

				const Input::MouseButton mouseButton = InputManager::GetInstance().GetRawInput()->ToMouseButton(button);

				Event_Mouse_ButtonPress eventMouseButtonPress = { pWindow,
					                                              static_cast<int32>(x),
					                                              static_cast<int32>(y), 
					                                              mouseButton };

				EventManager::GetInstance().Send(eventMouseButtonPress);
			}
		}

		void OnKey(GLFWwindow* pGLFWWindow, int32 key, [[maybe_unused]] int32 scancode, int32 action, int32 mods)
		{
			if (action == GLFW_PRESS)
			{
				Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGLFWWindow));

				const Input::Key keyCode = InputManager::GetInstance().GetRawInput()->ToKey(key);
				const Input::Modifier modifiers = InputManager::GetInstance().GetRawInput()->ParseModifiers(mods);

				Event_Keyboard_KeyPress eventKeyboardKeyPress = { pWindow, keyCode, modifiers };
				EventManager::GetInstance().Send(eventKeyboardKeyPress);
			}
		}
	}
}