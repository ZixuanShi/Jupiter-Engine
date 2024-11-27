// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <GLFW/glfw3.h>

export module jpt.Window_GLFW;

import jpt.Window;

import jpt.Framework;

import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Vector2;

import jpt.InputManager;
import jpt.Input.KeyCode;

import jpt.Time.TypeDefs;

import jpt.Event.Manager;
import jpt.Event.Window.Resize;
import jpt.Event.Window.Close;
import jpt.Event.Mouse.ButtonPress;
import jpt.Event.Keyboard.KeyPress;

import jpt.Renderer;

namespace jpt
{
	namespace Callbacks
	{
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

		virtual bool ShouldClose() const override;
		virtual Vec2i GetSize() const override;

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
		glfwSetFramebufferSizeCallback(m_pGLFWWindow, Callbacks::OnWindowResize);
		glfwSetMouseButtonCallback(m_pGLFWWindow, Callbacks::OnMouseButton);
		glfwSetKeyCallback(m_pGLFWWindow, Callbacks::OnKey);

		return true;
	}

	void Window_GLFW::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

		// HACK: This shouldn't be here in Window class. I need to properly design Vulkan + GLFW for multiple windows
		// Currently this means, if one of the windows is minimized, the whole application will be paused
		while (IsMinimized())
		{
			glfwWaitEvents();
		}
	}

	void Window_GLFW::Shutdown()
	{
		glfwDestroyWindow(m_pGLFWWindow);
		m_pGLFWWindow = nullptr;

		Super::Shutdown();
	}

	bool Window_GLFW::ShouldClose() const
	{
		const bool shouldClose = glfwWindowShouldClose(m_pGLFWWindow);
		if (shouldClose)
		{
			Event_Window_Close eventWindowClose = { this };
			EventManager::GetInstance().Send(eventWindowClose);
		}

		return shouldClose;
	}

	Vec2i Window_GLFW::GetSize() const
	{
		int32 width = 0;
		int32 height = 0;
		glfwGetFramebufferSize(m_pGLFWWindow, &width, &height);
		return Vec2i(width, height);
	}

	namespace Callbacks
	{
		void OnWindowResize(GLFWwindow* pGLFWWindow, int32 width, int32 height)
		{
			Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGLFWWindow));
			JPT_ASSERT(pWindow, "Couldn't cast window user pointer to jpt::Window");

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

				const Input::KeyCode keyCode = GetApplication()->GetInputManager()->ToKeyCode(button);

				Event_Mouse_ButtonPress eventMouseButtonPress = { pWindow,
					                                              static_cast<int32>(x),
					                                              static_cast<int32>(y), 
					                                              keyCode };

				EventManager::GetInstance().Send(eventMouseButtonPress);
			}
		}

		void OnKey(GLFWwindow* pGLFWWindow, int32 key, [[maybe_unused]] int32 scancode, int32 action, [[maybe_unused]] int32 mods)
		{
			if (action == GLFW_PRESS)
			{
				Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGLFWWindow));
				const Input::KeyCode keyCode = GetApplication()->GetInputManager()->ToKeyCode(key);

				Event_Keyboard_KeyPress eventKeyboardKeyPress = { pWindow, keyCode };
				EventManager::GetInstance().Send(eventKeyboardKeyPress);
			}
		}
	}
}