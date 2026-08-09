// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <cstdint>

#include "Applications/Window/WindowBase.h"

namespace jpt
{
    class WindowMac : public WindowBase
    {
    private:
        /** Defined in WindowMac.mm, where ARC applies. Declaring the AppKit members here
            would be an ODR hazard: this header is compiled as C++ by Application.cppm and as
            ObjC++ by WindowMac.mm, and only the latter would emit retain/release. */
        struct Impl;
        Impl* m_pImpl = nullptr;

    public:
        bool PreInit(std::int32_t argc, char* ppArgv[]);
        bool Init();
        void Run();
        void Terminate();

        /** Hides the pointer and pins it in place, so a look drag cannot walk it into a screen
            edge and stall. Idempotent, because AppKit counts hide against unhide rather than
            latching: an unmatched call strands the pointer for the rest of the session. */
        void SetCursorCaptured(bool captured);
    };
}
