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

import jpt.Event.Manager;
import jpt.Event.Window.Close;

namespace jpt
{
	namespace Callbacks
	{
		void OnError(int32 error, const char* description);
	}

	export class Framework_GLFW final : public Framework_Base
	{
		using Super = Framework_Base;

	private:
		Application_Base* m_pApp = nullptr;

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