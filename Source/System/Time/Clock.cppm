// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <chrono>

export module jpt.Clock;

import jpt.DateTime;
import jpt.TypeDefs;
import jpt.TimeTypeDefs;

export namespace jpt
{
    /** Use for simulating real-life time. Getting specific Year, Month, Day, Hour, Minute, Second */
    class Clock
    {
        using TClock = std::chrono::system_clock;

    public:
        using Point = std::chrono::time_point<TClock>;

    public:
        static Point Now();
        static RawTimeType RawNow();
        static DateTime GetCurrentDateTime();
    };
}