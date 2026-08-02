// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <cstdint>

namespace jpt
{
    bool CreateAppWindow(std::int32_t width, std::int32_t height, const char* title);

    // argc/argv come straight from main: UIApplicationMain declares argv non-null, so
    // passing nullptr trips -Wnonnull. macOS ignores them.
    void RunAppLoop(int argc, char* argv[]);
}
