// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application_Base.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

export module jpt.Window_GLFW;

import jpt.Window_Base;

import jpt.Optional;
import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.ProjectSettings;
import jpt.Json;
import jpt.Json.Data;
import jpt.File.Path;
import jpt.File.Path.Utils;
import jpt.Time.TypeDefs;

void ResizeViewportCallback(GLFWwindow* pGLFWwindow, int32 width, int32 height)
{
	JPT_LOG("Resized window %lu to width: %i, height: %i", pGLFWwindow, width, height);
	glViewport(0, 0, width, height);
}

namespace jpt
{
	export class Window_GLFW final : public Window_Base
	{
		using Super = Window_Base;

	private:
		GLFWwindow* m_pWindow = nullptr;

	public:
		virtual bool Init(Application_Base* pApp) override;
		virtual void Update(TimePrecision deltaSeconds) override;

		GLFWwindow* GetGLFWwindow() const { return m_pWindow; }
	};

	bool Window_GLFW::Init(Application_Base* pApp)
	{
		JPT_ENSURE(Super::Init(pApp));

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		int32 width = 800;
		int32 height = 600;
		String title = "Unnamed";

		ProjectSettings& projectSettings = ProjectSettings::GetInstance();
		projectSettings.TryGet("window_width", width);
		projectSettings.TryGet("window_height", height);
		projectSettings.TryGet("window_title", title);

		m_pWindow = glfwCreateWindow(width, height, title.ConstBuffer(), nullptr, nullptr);
		if (!m_pWindow)
		{
			JPT_ERROR("Failed to create GLFW window");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_pWindow);

		glfwSetFramebufferSizeCallback(m_pWindow, ResizeViewportCallback);
		glfwGetFramebufferSize(m_pWindow, &width, &height);

		return true;
	}

	void Window_GLFW::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

		glfwSwapBuffers(m_pWindow);
	}
}