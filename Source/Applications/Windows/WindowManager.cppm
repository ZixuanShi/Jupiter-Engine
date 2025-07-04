// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

export module jpt.WindowManager;

import jpt.Application;

import jpt.Window;
import jpt.Window_GLFW;
import jpt.Window_Win32;

import jpt.Renderer;
import jpt.Framework_Enums;

import jpt.Status;
import jpt.DynamicArray;
import jpt.TypeDefs;

import jpt.EventManager;
import jpt.Event_Window_Close;

export namespace jpt
{
    class WindowManager
    {
    public:
        static constexpr size_t kMaxWindows = 16;    /**< Maximum number of windows Jupiter can manage simultaneously */

    private:
        DynamicArray<Window*> m_windows;    /**< All the windows managed by Jupiter. index at 0 is the main window */

    public:
        bool PreInit();
        bool Init(const char* mainWindowTitle);

        void Update(TimePrecision deltaSeconds);
        void Terminate();

    public:
        Window* Create(const char* title = Window::kDefaultTitle, int32 width = Window::kDefaultWidth, int32 height = Window::kDefaultHeight);
        void Destroy(const Window* pWindowToDestroy);

    public:
        Window* GetMainWindow();
    };

    bool WindowManager::PreInit()
    {
        EventManager::GetInstance().Register<Event_Window_Close>([this](const Event_Window_Close& eventWindowClose)
            {
                Destroy(eventWindowClose.GetWindow());
            });

        return true;
    }

    bool WindowManager::Init(const char* mainWindowTitle)
    {
        JPT_ASSERT(m_windows.IsEmpty(), "WindowManager already initialized");

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

    void WindowManager::Terminate()
    {
        for (Window* window : m_windows)
        {
            JPT_TERMINATE(window);
        }
        m_windows.Clear();
    }

    Window* WindowManager::Create(const char* title /* = Window::kDefaultTitle*/, int32 width /*= Window::kDefaultWidth*/, int32 height /*= Window::kDefaultHeight*/)
    {
        JPT_ASSERT(m_windows.Count() < kMaxWindows, "Maximum number of windows reached");
        
        const FrameworkAPI api = GetApplication()->GetFrameworkAPI();
        switch (api.Value())
        {
        case FrameworkAPI::GLFW:
            m_windows.EmplaceBack(Allocator<Window_GLFW>::New());
            break;

        case FrameworkAPI::Win32:
            m_windows.EmplaceBack(Allocator<Window_Win32>::New());
            break;

        default:
            JPT_ASSERT(false, "Un-implemented Framework API: %s", ToString(api).ConstBuffer());
            return nullptr;
        }

        Window* pWindow = m_windows.Back();
        pWindow->Init(title, width, height);
        return pWindow;
    }

    void WindowManager::Destroy(const Window* pWindowToDestroy)
    {
        if (pWindowToDestroy == GetMainWindow())
        {
            GetApplication()->SetStatus(Status::Success);
            return;
        }

        for (auto itr = m_windows.begin(); itr != m_windows.end(); ++itr)
        {
            Window* pWindow = *itr;
            if (pWindow == pWindowToDestroy)
            {
                JPT_TERMINATE(pWindow);
                m_windows.Erase(itr);
                break;
            }
        }
    }

    Window* WindowManager::GetMainWindow()
    {
        JPT_ASSERT(!m_windows.IsEmpty(), "No windows created");
        return m_windows[0];
    }
}