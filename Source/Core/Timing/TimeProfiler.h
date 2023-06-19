// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#if IS_PROFILING
namespace jpt
{
	/** Profiler regarding timing. Will log how much time a single function spent */
	class JPT_API TimeProfiler
	{
	private:
		const char* m_pLabel;
		Timer m_timer;

	public:
		TimeProfiler(const char* pLabel);
		~TimeProfiler();
	};

	/** Used for timing a scope if it's profiling config. Example: 
		void Foo()
		{
			JPT_USE_TIMING_PROFILER("Foo")
			
			// Work stuff...
		}
		*/
	#define JPT_START_TIMING_PROFILER(label) jpt::TimeProfiler profiler(label)
}
#else
	// Do nothing if it's not profiling configuration
	#define JPT_START_TIMING_PROFILER(label)
#endif