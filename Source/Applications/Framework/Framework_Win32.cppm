// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module jpt.Framework_Win32;

import jpt.Framework;

export namespace jpt
{
    class Platform_Windows;

    class Framework_Win32 final : public Framework
    {
        using Super = Framework;

    private:
        WNDCLASSEX m_windowClass = { 0 };

    public:
        virtual bool PreInit() override;
        virtual bool Init() override;
        virtual void Update(TimePrecision deltaSeconds) override;
        virtual void Terminate() override;

    public:
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        Platform_Windows* GetPlatform_Windows() const;
        const WNDCLASSEX& GetWindowClass() const { return m_windowClass; }
    };
}