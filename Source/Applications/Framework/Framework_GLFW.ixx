// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"
#include "Applications/App/Application_Base.h"

#include <GLFW/glfw3.h>

export module jpt.Framework_GLFW;

import jpt.Framework_Base;
import jpt.Window_GLFW;

import jpt.TypeDefs;
import jpt.ToString;
import jpt.Utilities;

export namespace jpt
{
	class Framework_GLFW final : public Framework_Base
	{
		using Super = Framework_Base;

	private:
		Application_Base* m_pApp = nullptr;

	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

	private:
		Window_GLFW* m_pWindow = nullptr;
	};

	bool Framework_GLFW::Init()
	{
		JPT_ENSURE(Super::Init());
		JPT_ENSURE(glfwInit());

		glfwSetErrorCallback([](int32 error, const char* description)
			{
				JPT_ERROR("GLFW Error: " + ToString(error) + " - " + description);
			});

		m_pApp = GetApplication();
		m_pWindow = static_cast<Window_GLFW*>(m_pApp->GetWindow());

		return true;
	}

	void Framework_GLFW::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

		if (glfwWindowShouldClose(m_pWindow->GetGLFWwindow()))
		{
			m_pApp->ShutdownApp();
		}

		glfwPollEvents();
	}

	void Framework_GLFW::Shutdown()
	{
		Super::Shutdown();

		glfwTerminate();
	}
}