// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <cstdint>

namespace jpt
{
    class IOSWindow
    {
    private:
        struct Impl;
        Impl* m_pImpl = nullptr;

    public:
        bool PreInit(std::int32_t argc, char* ppArgv[]);
        bool Init();
        void Run();
        void Terminate();
    };
}
