// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if !IS_CONFIG_RELEASE

#include "TimingProfiler.h"

#include "Debugging/Logger.h"

import jpt.TimeTypeDefs;
import jpt.StopWatch;

namespace jpt
{
    ScopedTimingProfiler::ScopedTimingProfiler(const char* label, const char* file, int line)
        : m_label(label)
        , m_file(file)
        , m_line(line)
    {
        m_start = StopWatch::Now();
    }
    
    ScopedTimingProfiler::~ScopedTimingProfiler()
    {
        const TimePrecision milliseconds = StopWatch::GetMsFrom(m_start);
        const String message = String::Format<256>("TimingProfiler: %s took %f ms", m_label, milliseconds);
        Logger::GetInstance().Log(Logger::Type::Debug, m_line, m_file, message.ConstBuffer());
    }
}
#endif // !IS_CONFIG_RELEASE