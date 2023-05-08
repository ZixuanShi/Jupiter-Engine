#pragma once

#if IS_PROFILING
namespace jpt
{
	// Profiler using time
	class JPT_API TimeProfiler
	{
	public:
		TimeProfiler(const char* pLabel);
		~TimeProfiler();

	private:
		const char* m_pLabel;
		Timer m_timer;
	};

	// Used for timing a scope if it's profiling config. Example: 
	/*
	* void Foo()
	* {
	*	JPT_USE_TIMING_PROFILER("Foo")
	* 
	*	// Work stuff...
	* }
	*/
	#define JPT_START_TIMING_PROFILER(label) jpt::TimeProfiler profiler(label)
}
#else
	// Do nothing if it's not profiling configuration
	#define JPT_START_TIMING_PROFILER(label)
#endif