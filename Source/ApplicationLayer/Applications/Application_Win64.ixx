// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_WIN64

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <Windows.h>
#include <GLFW/glfw3.h>

export module jpt.Application_Win64;

import jpt.Application_Base;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.CommandLine;

struct GLFWwindow;

namespace jpt
{
	export class Application_Win64 : public Application_Base
	{
	private:
		using Super = Application_Base;

	protected:
		/** The handle to an instance or handle to a module.
		    The operating system uses this value to identify the executable or EXE when it's loaded in memory. 
		    Certain Windows functions need the instance handle, for example to load icons or bitmaps. */
		HINSTANCE m_hInstance = nullptr;

		/** A flag that indicates whether the main application window is minimized, maximized, or shown normally. */
		int32 m_nCmdShow = 0;

		GLFWwindow* m_pWindow = nullptr;

	public:
		virtual bool PreInit() override;
		virtual bool Init() override;
		virtual void Update() override;
		virtual void Terminate() override;

	public:
		void SetHINSTANCE(HINSTANCE hInstance) { m_hInstance = hInstance; }
		void SetnCmdShow(int32 nCmdShow) { m_nCmdShow = nCmdShow; }
	};

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

#endif // IS_PLATFORM_WIN64