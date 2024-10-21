// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Debugging/Logger.h"
#include "Profiling/TimingProfiler.h"

export module Benchmarks_HashMap;

import jpt.BenchmarksReporter;
import jpt.HashMap;
import jpt.TypeDefs;
import jpt.String;
import jpt.Utilities;

static void Add_Trivial(jpt::BenchmarksReporter& reporter)
{
	static constexpr size_t kCount = 1'000'000;
	jpt::StopWatch::Point now;
	jpt::TimePrecision jptResult = 0.0;

	{
		now = jpt::StopWatch::Now();

		jpt::HashMap<int32, int32> map;
		map.Reserve(kCount);
		for (int32 i = 0; i < kCount; ++i)
		{
			map.Add(i, i);
		}

		jptResult = jpt::StopWatch::GetMsFrom(now);
	}

	jpt::BenchmarkUnit unit{ "HashMap", "Add 1'000'000 elements", jptResult };
	reporter.Add(unit);
}

export void RunBenchmarks_HashMap(jpt::BenchmarksReporter& reporter)
{
	Add_Trivial(reporter);
}