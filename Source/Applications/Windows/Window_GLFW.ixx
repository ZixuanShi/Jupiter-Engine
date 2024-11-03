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

import jpt.Input.KeyCode;
import jpt.Input.Manager;

import jpt.Time.TypeDefs;

import jpt.Event.Manager;
import jpt.Event.Window.Resize;
import jpt.Event.Window.Close;
import jpt.Event.Mouse.ButtonPress;

namespace jpt
{
	namespace Callbacks
	{
		void OnWindowResize(GLFWwindow* pGLFWwindow, int32 width, int32 height);
		void OnMouseButton(GLFWwindow* pGLFWwindow, int32 button, int32 action, int32 mods);
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

		GLFWwindow* GetGLFWwindow() const { return m_pGLFWWindow; }
	};

	bool Window_GLFW::Init(const char* title, int32 width, int32 height)
	{
		JPT_ENSURE(Super::Init(title, width, height));

		// Create GLFW window
		m_pGLFWWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!m_pGLFWWindow)
		{
			JPT_ERROR("Failed to create GLFW window");
			GetApplication()->GetFramework()->Shutdown();
			return false;
		}
		glfwMakeContextCurrent(m_pGLFWWindow);

		// Set current window callbacks
		glfwSetWindowUserPointer(m_pGLFWWindow, this);
		glfwSetFramebufferSizeCallback(m_pGLFWWindow, Callbacks::OnWindowResize);
		glfwSetMouseButtonCallback(m_pGLFWWindow, Callbacks::OnMouseButton);

		return true;
	}

	void Window_GLFW::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

		glfwSwapBuffers(m_pGLFWWindow);
	}

	void Window_GLFW::Shutdown()
	{
		Super::Shutdown();

		glfwDestroyWindow(m_pGLFWWindow);
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

	namespace Callbacks
	{
		void OnWindowResize(GLFWwindow* pGLFWwindow, int32 width, int32 height)
		{
			void* pWindow = glfwGetWindowUserPointer(pGLFWwindow);
			Event_Window_Resize eventWindowResize = { static_cast<Window*>(pWindow), width, height };
			EventManager::GetInstance().Send(eventWindowResize);
		}

		void OnMouseButton(GLFWwindow* pGLFWwindow, int32 button, int32 action, int32)
		{
			if (action == GLFW_PRESS)
			{
				double x, y;
				glfwGetCursorPos(pGLFWwindow, &x, &y);

				const Input::KeyCode keyCode = Input::Manager::GetInstance().ToKeyCode(button);
				Event_Mouse_ButtonPress eventMouseButtonPress = { static_cast<int32>(x), 
					                                              static_cast<int32>(y), 
					                                              keyCode };

				EventManager::GetInstance().Send(eventMouseButtonPress);
			}
		}
	}
}