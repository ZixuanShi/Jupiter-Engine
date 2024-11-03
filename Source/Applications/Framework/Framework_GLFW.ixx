// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"
#include "Applications/App/Application.h"

//#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//#define GLM_FORCE_RADIANS
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//#include <glm/vec4.hpp>
//#include <glm/mat4x4.hpp>

export module jpt.Framework_GLFW;

import jpt.Framework;
import jpt.Window_GLFW;

import jpt.TypeDefs;
import jpt.ToString;
import jpt.Utilities;

import jpt.Event.Manager;
import jpt.Event.Window.Close;

namespace jpt
{
	namespace Callbacks
	{
		void OnError(int32 error, const char* description);
	}

	export class Framework_GLFW final : public Framework
	{
		using Super = Framework;

	private:
		Application* m_pApp = nullptr;

	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

	private:
		Window_GLFW* m_pMainWindow = nullptr;
	};

	bool Framework_GLFW::Init()
	{
		JPT_ENSURE(Super::Init());

		JPT_ENSURE(glfwInit());
		//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_pApp = GetApplication();
		m_pMainWindow = static_cast<Window_GLFW*>(m_pApp->GetWindow());

		glfwSetErrorCallback(Callbacks::OnError);

		return true;
	}

	void Framework_GLFW::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

		if (glfwWindowShouldClose(m_pMainWindow->GetGLFWwindow()))
		{
			Event_Window_Close eventWindowClose = { m_pMainWindow };
			EventManager::GetInstance().Send(eventWindowClose);
		}

		glfwPollEvents();
	}

	void Framework_GLFW::Shutdown()
	{
		Super::Shutdown();

		glfwTerminate();
	}

	namespace Callbacks
	{
		void OnError(int32 error, const char* description)
		{
			JPT_ERROR("GLFW Error: " + ToString(error) + " - " + description);
		}
	}
}