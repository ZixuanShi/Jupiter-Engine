// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application_Base.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <GLFW/glfw3.h>

export module jpt.Window_GLFW;

import jpt.Window_Base;
import jpt.Utilities;

namespace jpt
{
	export class Window_GLFW final : public Window_Base
	{
		using Super = Window_Base;

	private:
		GLFWwindow* m_pWindow = nullptr;

	public:
		virtual bool Init() override;
		virtual void Update() override;
	};

	bool Window_GLFW::Init()
	{
		JPT_ENSURE(Super::Init());

		m_pWindow = glfwCreateWindow(800, 600, "Jupiter Technologies", nullptr, nullptr);
		if (!m_pWindow)
		{
			glfwTerminate();
			return false;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(m_pWindow);

		return true;
	}

	void Window_GLFW::Update()
	{
		Super::Update();

		if (glfwWindowShouldClose(m_pWindow))
		{
			Application_Base::GetInstance()->TerminateApp();
		}

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(m_pWindow);

		/* Poll for and process events */
		glfwPollEvents();
	}
}