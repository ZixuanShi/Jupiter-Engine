// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <cstdint>

namespace jpt
{
    bool CreateAppWindow(std::int32_t width, std::int32_t height, const char* title);
    void RunAppLoop();
}
