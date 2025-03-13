// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <chrono>

#if !IS_RELEASE

namespace jpt
{
    /** The profiler will stop and record profiling data when it goes out of scope */
    class ScopedTimingProfiler
    {
    private:
        const char* m_label;
        const char* m_file;
        int m_line;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

    public:
        ScopedTimingProfiler(const char* label, const char* file, int line);
        ~ScopedTimingProfiler();
    };
}

/** Use this macro to profile a scope of code regarding time
    @example 
    
    void SomeFunction()
        {
            JPT_SCOPED_TIMING_PROFILER(topic_name_label);

            // Your code here
            // ...
        } */
#define JPT_SCOPED_TIMING_PROFILER(label) jpt::ScopedTimingProfiler timing_profilier(label, __FILE__, __LINE__)

#else
    #define JPT_SCOPED_TIMING_PROFILER(label)
#endif