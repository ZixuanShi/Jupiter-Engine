// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <thread>

export module jpt.Thread.Utils;

import jpt.TypeDefs;

export namespace jpt
{
    void Sleep(int32 seconds)
    {
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    }

    void SleepMs(int32 milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
}