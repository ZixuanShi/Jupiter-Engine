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

import jpt.Event.Manager;
import jpt.Event.Window.Create;
import jpt.Event.Window.Close;

export namespace jpt
{
	class WindowManager
	{
	private:
		DynamicArray<Window*> m_windows;	/**< All the windows managed by Jupiter. index at 0 is the main window */
		Framework_API m_frameworkAPI;

	public:
		/** Assign framework API and register events */
		bool PreInit(Framework_API frameworkAPI);

		/** Create main window */
		bool Init(const char* mainWindowTitle);

		void Update(TimePrecision deltaSeconds);
		void Shutdown();

		Window* Create(const char* title = "New Window", int32 width = Window::kDefaultWidth, int32 height = Window::kDefaultHeight);
		void Destroy(const Window* pWindowToDestroy);

		Window* GetMainWindow();
	};

	bool WindowManager::PreInit(Framework_API frameworkAPI)
	{
		m_frameworkAPI = frameworkAPI;

		EventManager::GetInstance().Register<Event_Window_Create>([this](const Event_Window_Create& eventWindowCreate)
			{
				Create(eventWindowCreate.GetTitle(), eventWindowCreate.GetWidth(), eventWindowCreate.GetHeight());
			});
		EventManager::GetInstance().Register<Event_Window_Close>([this](const Event_Window_Close& eventWindowClose)
			{
				Destroy(eventWindowClose.GetWindow());
			});

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

	Window* WindowManager::Create(const char* title /* = "New Window"*/, int32 width /*= Window::kDefaultWidth*/, int32 height /*= Window::kDefaultHeight*/)
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

		m_windows.Back()->Init(title, width, height);
		return m_windows.Back();
	}

	void WindowManager::Destroy(const Window* pWindowToDestroy)
	{
		if (pWindowToDestroy == GetMainWindow())
		{
			GetApplication()->SetShouldShutdown();
			return;
		}

		for (auto itr = m_windows.begin(); itr != m_windows.end(); ++itr)
		{
			Window* pWindow = *itr;
			if (pWindow == pWindowToDestroy)
			{
				JPT_SHUTDOWN(pWindow);
				m_windows.Erase(itr);
				break;
			}
		}
	}

	Window* WindowManager::GetMainWindow()
	{
		return m_windows[0];
	}
}