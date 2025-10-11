// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module jpt.Window_Win32;

import jpt.Window;

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
        virtual void Terminate() override;

    public:
        HWND GetHWND() const { return m_hwnd; }
    };
}