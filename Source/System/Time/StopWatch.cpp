// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <chrono>
#include <time.h>

module jpt.StopWatch;

namespace jpt
{
    StopWatch::Point StopWatch::Now()
    {
        return TClock::now();
    }

    TimePrecision StopWatch::GetSecondsBetween(const Point& begin, const Point& end)
    {
        const std::chrono::duration<TimePrecision> diff = end - begin;
        return diff.count();
    }

    TimePrecision StopWatch::GetSecondsFrom(const Point& begin)
    {
        const auto end = Now();
        return GetSecondsBetween(begin, end);
    }

    TimePrecision StopWatch::GetMsBetween(const Point& begin, const Point& end)
    {
        return GetSecondsBetween(begin, end) * static_cast<TimePrecision>(1000.0f);
    }

    TimePrecision StopWatch::GetMsFrom(const Point& begin)
    {
        return GetSecondsFrom(begin) * static_cast<TimePrecision>(1000.0f);
    }

    void StopWatch::Start()
    {
        m_start = Now();
    }

    TimePrecision StopWatch::GetDuration() const
    {
        return GetSecondsFrom(m_start);
    }

    TimePrecision StopWatch::GetDurationMs() const
    {
        return GetMsFrom(m_start);
    }
}