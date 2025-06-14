// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module jpt.Framework_Win32;

import jpt.Application;
import jpt.Framework;

import jpt.Window;
import jpt.WindowManager;
import jpt.Window_Win32;

import jpt.Platform_Windows;

namespace jpt
{
    export class Framework_Win32 final : public Framework
    {
        using Super = Framework;

    private:
        WNDCLASSEX m_windowClass = { 0 };

    public:
        virtual bool PreInit() override;
        virtual bool Init() override;
        virtual void Update(TimePrecision deltaSeconds) override;
        virtual void Shutdown() override;

    public:
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        Platform_Windows* GetPlatform_Windows() const;
        const WNDCLASSEX& GetWindowClass() const { return m_windowClass; }
    };

    bool Framework_Win32::PreInit()
    {
        JPT_ENSURE(Super::PreInit());

        // Initialize the window class.
        m_windowClass.cbSize = sizeof(WNDCLASSEX);
        m_windowClass.style = CS_HREDRAW | CS_VREDRAW;
        m_windowClass.lpfnWndProc = &Framework_Win32::WindowProc;
        m_windowClass.hInstance = GetPlatform_Windows()->GetHINSTANCE();
        m_windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        m_windowClass.lpszClassName = Window_Win32::kClassName;
        RegisterClassEx(&m_windowClass);

        return true;
    }

    bool Framework_Win32::Init()
    {
        JPT_ENSURE(Super::Init());

        return true;
    }

    void Framework_Win32::Update(TimePrecision deltaSeconds)
    {
        Super::Update(deltaSeconds);

        MSG msg = { };
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void Framework_Win32::Shutdown()
    {
        UnregisterClass(m_windowClass.lpszClassName, GetPlatform_Windows()->GetHINSTANCE());

        Super::Shutdown();
    }

    LRESULT Framework_Win32::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        Window_Win32* pWindow = reinterpret_cast<Window_Win32*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        Application* pApp = GetApplication();

        switch (message)
        {
            case WM_CREATE:
            {
                LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));

                return 0;
            }
            case WM_DESTROY:
            {
                pApp->GetWindowManager()->Destroy(pWindow);

                if (pApp->GetMainWindow() == pWindow)
                {
                    PostQuitMessage(0);
                }

                return 0;
            }
            case WM_SIZE:
            {
                if (pWindow)
                {
                    pWindow->SetFrameSize(Vec2i(LOWORD(lParam), HIWORD(lParam)));
                }
                return 0;
            }
            default:
            { 
                return DefWindowProc(hWnd, message, wParam, lParam); 
            }
        }
    }

    Platform_Windows* Framework_Win32::GetPlatform_Windows() const
    {
        Platform_Windows* pPlatform_Windows = static_cast<Platform_Windows*>(GetApplication()->GetPlatform());
        JPT_ASSERT(pPlatform_Windows, "Platform is not set to windows");
        return pPlatform_Windows;
    }
}