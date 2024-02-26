// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if JPT_ENABLE_EDITOR_FEATURES

#include "TimingProfiler.h"

#include "Debugging/Logger.h"

import jpt.Time.TypeDefs;
import jpt.StopWatch;

using namespace jpt;

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
	const jpt::String message = jpt::String::Format<256>("TimingProfiler: %s took %f ms", m_label, milliseconds);
	jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Log, m_line, m_file, message.ConstBuffer());
}

#endif