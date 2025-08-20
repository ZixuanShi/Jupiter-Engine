// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <chrono>

export module jpt.StopWatch;

import jpt.TypeDefs;

export namespace jpt
{
    /** Use for timing and benchmarking */
    class StopWatch
    {
    public:
        using TClock = std::chrono::high_resolution_clock;
        using Point  = std::chrono::time_point<TClock>;

    private:
        Point m_start;

    public:
        static Point Now();
        static TimePrecision GetSecondsBetween(const Point& begin, const Point& end);
        static TimePrecision GetSecondsFrom(const Point& begin);
        static TimePrecision GetMsBetween(const Point& begin, const Point& end);
        static TimePrecision GetMsFrom(const Point& begin);

        void Start();
        TimePrecision GetDuration() const;
        TimePrecision GetDurationMs() const;
    };
}