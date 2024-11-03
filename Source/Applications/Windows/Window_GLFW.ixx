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

import jpt.Json;
import jpt.Json.Data;

import jpt.Input.KeyCode;
import jpt.Input.Manager;

import jpt.File.Path;
import jpt.File.Path.Utils;
import jpt.Time.TypeDefs;
import jpt.ProjectSettings;

import jpt.Event.Manager;
import jpt.Event.Window.Resize;
import jpt.Event.Mouse.ButtonPress;

static constexpr int32   kDefaultWidth  = 800;
static constexpr int32   kDefaultHeight = 600;
static const jpt::String kDefaultTitle  = "Unnamed";

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
		GLFWwindow* m_pWindow = nullptr;

	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

		GLFWwindow* GetGLFWwindow() const { return m_pWindow; }
	};

	bool Window_GLFW::Init()
	{
		JPT_ENSURE(Super::Init());

		// Load window settings from project settings
		ProjectSettings& projectSettings = ProjectSettings::GetInstance();
		int32 width  = projectSettings.Get("window_width",  kDefaultWidth);
		int32 height = projectSettings.Get("window_height", kDefaultHeight);
		String title = projectSettings.Get("window_title",  kDefaultTitle);

		// Create GLFW window
		m_pWindow = glfwCreateWindow(width, height, title.ConstBuffer(), nullptr, nullptr);
		if (!m_pWindow)
		{
			JPT_ERROR("Failed to create GLFW window");
			GetApplication()->GetFramework()->Shutdown();
			return false;
		}
		glfwMakeContextCurrent(m_pWindow);

		// Set current window callbacks
		glfwSetWindowUserPointer(m_pWindow, this);
		glfwSetFramebufferSizeCallback(m_pWindow, Callbacks::OnWindowResize);
		glfwSetMouseButtonCallback(m_pWindow, Callbacks::OnMouseButton);

		return true;
	}

	void Window_GLFW::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

		glfwSwapBuffers(m_pWindow);
	}

	void Window_GLFW::Shutdown()
	{
		Super::Shutdown();

		glfwDestroyWindow(m_pWindow);
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