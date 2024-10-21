// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Profiling/TimingProfiler.h"
#include "Debugging/Logger.h"

export module Benchmarks_Math;

import jpt.BenchmarksReporter;
import jpt.Math;
import jpt.TypeDefs;

static void Min([[maybe_unused]] jpt::BenchmarksReporter& reporter)
{
	JPT_SCOPED_TIMING_PROFILER("Min");

	int32 min = 0;
	for (size_t i = 0; i < 1'000'000; ++i)
	{
		min = jpt::Min(1, 2, 3, 4, 6, 8);
	}

	JPT_LOG("Min %i", min);
}

export void RunBenchmarks_Math(jpt::BenchmarksReporter& reporter)
{
	//Min(reporter);
}