// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "TimingProfiler.h"

#include "Debugging/Logger.h"

import jpt.Time.TypeDefs;
import jpt.StopWatch;

using namespace jpt;

TimingProfiler::TimingProfiler(const char* label)
	: m_label(label)
{
	m_start = StopWatch::Now();
}

TimingProfiler::~TimingProfiler()
{
	const TimePrecision milliseconds = StopWatch::GetMsFrom(m_start);
	JPT_SYSTEM_INFO("TimingProfiler: %s took %f ms", m_label, milliseconds);
}