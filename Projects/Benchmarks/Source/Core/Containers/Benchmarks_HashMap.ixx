// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Debugging/Logger.h"
#include "Profiling/TimingProfiler.h"

export module Benchmarks_HashMap;

import jpt.HashMap;
import jpt.TypeDefs;
import jpt.String;
import jpt.Utilities;

static void JPT_HashMap_Add()
{
	JPT_SCOPED_TIMING_PROFILER("JPT_HashMap_Add");

	jpt::HashMap<int32, jpt::String> map;

	for (int32 i = 0; i < 1'000'000; ++i)
	{
		map.Add(i, "Hello World");
	}
}

export void RunBenchmarks_HashMap()
{
	JPT_HashMap_Add();
}