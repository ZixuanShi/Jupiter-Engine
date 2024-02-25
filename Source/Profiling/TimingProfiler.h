// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <chrono>

#if JPT_ENABLE_EDITOR_FEATURES

namespace jpt
{
	class TimingProfiler
	{
	private:
		const char* m_label;
		const char* m_file;
		int m_line;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

	public:
		TimingProfiler(const char* label, const char* file, int line);
		~TimingProfiler();
	};
}

	#define JPT_TIMING_PROFILER(label) jpt::TimingProfiler __timing_profiler__(label, __FILE__, __LINE__)
#else
	#define JPT_TIMING_PROFILER(label)
#endif