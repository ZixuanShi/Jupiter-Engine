// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <chrono>

namespace jpt
{
	class TimingProfiler
	{
	private:
		const char* m_label;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

	public:
		TimingProfiler(const char* label);
		~TimingProfiler();
	};
}

#if JPT_ENABLE_EDITOR_FEATURES
	#define JPT_TIMING_PROFILER(label) jpt::TimingProfiler __timing_profiler__(label)
#else
	#define JPT_TIMING_PROFILER(label)
#endif