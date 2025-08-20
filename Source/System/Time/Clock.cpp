// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <time.h>

module jpt.Clock;

namespace jpt
{
    Clock::Point Clock::Now()
    {
        return TClock::now();
    }

    RawTimeType Clock::RawNow()
    {
        return time(nullptr);
    }

    DateTime Clock::GetCurrentDateTime()
    {
        return DateTime(RawNow());
    }
}