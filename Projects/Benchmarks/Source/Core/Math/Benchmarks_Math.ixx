// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"
#include "Profiling/TimingProfiler.h"

export module Benchmarks_Math;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;

void Benchmark_Math_Min()
{
	JPT_SCOPED_TIMING_PROFILER(Min);
}

export void RunBenchmarks_Math()
{
	Benchmark_Math_Min();
}