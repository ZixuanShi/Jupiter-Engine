// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"

export module jpt.Window.Manager;

import jpt.Window;
import jpt.Window_GLFW;

import jpt.Framework.Enums;

import jpt.DynamicArray;

import jpt.Time.TypeDefs;

static constexpr int32 kDefaultWindowWidth = 800;
static constexpr int32 kDefaultWindowHeight = 600;

export namespace jpt
{
	class WindowManager
	{
	private:
		DynamicArray<Window*> m_windows;	/**< index at 0 will always be the main window */
		Framework_API m_frameworkAPI;

	public:
		bool PreInit(Framework_API frameworkAPI);
		bool Init(const char* mainWindowTitle);
		void Update(TimePrecision deltaSeconds);
		void Shutdown();

		Window* Create(const char* title);
		Window* GetMainWindow();
	};

	bool WindowManager::PreInit(Framework_API frameworkAPI)
	{
		m_frameworkAPI = frameworkAPI;
		return true;
	}

	bool WindowManager::Init(const char* mainWindowTitle)
	{
		// Create main window
		Create(mainWindowTitle);

		return true;
	}

	void WindowManager::Update(TimePrecision deltaSeconds)
	{
		for (Window* window : m_windows)
		{
			window->Update(deltaSeconds);
		}
	}

	void WindowManager::Shutdown()
	{
		for (Window* window : m_windows)
		{
			JPT_SHUTDOWN(window);
		}
		m_windows.Clear();
	}

	Window* WindowManager::Create(const char* title)
	{
		switch (m_frameworkAPI.Value())
		{
		case Framework_API::GLFW:
			m_windows.EmplaceBack(new Window_GLFW());
			break;

		default:
			JPT_ASSERT(false, "Un-implemented Framework API: %s", m_frameworkAPI.ToString().ConstBuffer());
			return nullptr;
		}

		m_windows.Back()->Init(title, kDefaultWindowWidth, kDefaultWindowHeight);
		return m_windows.Back();
	}

	Window* WindowManager::GetMainWindow()
	{
		return m_windows[0];
	}
}