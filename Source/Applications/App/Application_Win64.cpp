// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/App/Application_Win64.h"

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <GLFW/glfw3.h>

import jpt.CommandLine;
import jpt.Utilities;
import jpt.ToString;

namespace jpt
{
	bool Application_Win64::PreInit()
	{
		JPT_ENSURE(Super::PreInit());
		JPT_ENSURE(m_hInstance != nullptr);

		return true;
	}

	bool Application_Win64::Init()
	{
		JPT_ENSURE(Super::Init());

		if (CommandLine::GetInstance().Has("no_window"))
		{
			m_shouldTerminate = true;
			return true;
		}

		/* Initialize the library */
		if (!glfwInit())
		{
			return false;
		}

		/* Create a windowed mode window and its OpenGL context */
		m_pWindow = glfwCreateWindow(640, 480, "Jupiter Engine", NULL, NULL);
		if (!m_pWindow)
		{
			glfwTerminate();
			return false;
		}

		glfwSetErrorCallback([](int32 error, const char* description)
			{
				JPT_LOG("GLFW Error: " + ToString(error) + " - " + description);
			});

		/* Make the window's context current */
		glfwMakeContextCurrent(m_pWindow);

		return true;
	}

	void Application_Win64::Update()
	{
		Super::Terminate();

		m_shouldTerminate = glfwWindowShouldClose(m_pWindow);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(m_pWindow);

		/* Poll for and process events */
		glfwPollEvents();
	}

	void Application_Win64::Terminate()
	{
		Super::Terminate();

		glfwTerminate();
	}
}