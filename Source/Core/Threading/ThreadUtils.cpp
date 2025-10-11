// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <thread>

module jpt.ThreadUtils;

namespace jpt
{
    void Sleep(int32 seconds)
    {
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    }

    void SleepMs(int32 milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    Index GetThreadId()
    {
        return static_cast<Index>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    }
}