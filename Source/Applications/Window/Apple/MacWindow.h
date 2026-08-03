// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <cstdint>

namespace jpt
{
    class MacWindow
    {
    private:
        /** Defined in MacWindow.mm, where ARC applies. Declaring the AppKit members here
            would be an ODR hazard: this header is compiled as C++ by Application.cppm and as
            ObjC++ by MacWindow.mm, and only the latter would emit retain/release. */
        struct Impl;
        Impl* m_pImpl = nullptr;

    public:
        bool PreInit(std::int32_t argc, char* ppArgv[]);
        bool Init();
        void Run();
        void Terminate();
    };
}
