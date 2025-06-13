// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module jpt.Window_Win32;

import jpt.Application;
import jpt.Window;

import jpt.Time.TypeDefs;
import jpt.Platform.Windows;
import jpt.String.Helpers;

namespace jpt
{
    export class Window_Win32 final : public Window
    {
        using Super = Window;

    public:
        static constexpr LPCWSTR kClassName = L"Jupiter Engine Win32 Class";

    private:
        HWND m_hwnd = nullptr;

    public:
        virtual bool Init(const char* title, int32 width, int32 height) override;
        virtual void Shutdown() override;

    public:
        HWND GetHWND() const { return m_hwnd; }
    };

    bool Window_Win32::Init(const char* title, int32 width, int32 height)
    {
        Super::Init(title, width, height);

        RECT windowRect = { 0,0, static_cast<LONG>(width), static_cast<LONG>(height) };
        AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

        const Platform_Windows* pPlatform_Windows = dynamic_cast<Platform_Windows*>(GetApplication()->GetPlatform());
        JPT_ASSERT(pPlatform_Windows, "Platform is not set to windows");

        const LPCWSTR titleW = ToWChars(title);

        // Create the window and store a handle to it
        m_hwnd = CreateWindow(
            kClassName,
            titleW,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr,        // We have no parent window.
            nullptr,        // We aren't using menus.
            pPlatform_Windows->GetHINSTANCE(),
            this);

        delete[] titleW;

        ShowWindow(m_hwnd, pPlatform_Windows->GetnCmdShow());

        return true;
    }

    void Window_Win32::Shutdown()
    {
        DestroyWindow(m_hwnd);

        Super::Shutdown();
    }
}