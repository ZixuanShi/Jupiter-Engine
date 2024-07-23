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
import jpt.File.Path;
import jpt.JsonUtils;
import jpt.JsonData;
import jpt.Optional;
import jpt.String;

namespace jpt
{

	export class Window_GLFW final : public Window_Base
	{
		using Super = Window_Base;

	private:
		GLFWwindow* m_pWindow = nullptr;

	public:
		virtual bool Init(Application_Base* pApp) override;
		virtual void Update() override;
	};

	bool Window_GLFW::Init(Application_Base* pApp)
	{
		JPT_ENSURE(Super::Init(pApp));

		const File::Path clientConfigPath = { File::ESource::Client, "Assets/Config/Settings.json" };
		Optional<jpt::JsonMap> settings = ReadJsonFile(clientConfigPath);
		if (!settings)
		{
			JPT_ERROR("Failed to read settings from file: %ls", clientConfigPath.ConstBuffer());
			return false;
		}

		const jpt::JsonMap& settingsMap = settings.Value();
		const int32 width = settingsMap["window_width"];
		const int32 height = settingsMap["window_height"];
		const String title = settingsMap["window_title"];
		m_pWindow = glfwCreateWindow(width, height, title.ConstBuffer(), nullptr, nullptr);
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
			m_pApp->TerminateApp();
		}

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(m_pWindow);

		/* Poll for and process events */
		glfwPollEvents();
	}
}