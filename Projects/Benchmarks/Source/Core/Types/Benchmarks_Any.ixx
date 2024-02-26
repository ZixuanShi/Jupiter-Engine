// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"
#include "Profiling/TimingProfiler.h"

#include <vector>
#include <string>
#include <any>

export module Benchmarks_Any;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.String;
import jpt.ToString;
import jpt.DynamicArray;
import jpt.Any;

void Benchmark_JptAny_Trivial()
{
	JPT_TIMING_PROFILER("jpt::Any Trivial Type");

	jpt::Any any;

	for (int32 i = 0; i < 1000000; ++i)
	{
		any = i;
	}
}

void Benchmark_StdAny_Trivial()
{
	JPT_TIMING_PROFILER("std::any Trivial Type");

	std::any any;

	for (int32 i = 0; i < 1000000; ++i)
	{
		any = i;
	}
}

void Benchmark_JptAny_NonTrivial()
{
	JPT_TIMING_PROFILER("jpt::Any NonTrivial Type");

	jpt::Any any;

	for (int32 i = 0; i < 1000000; ++i)
	{
		any = jpt::ToString(i);
	}
}

void Benchmark_StdAny_NonTrivial()
{
	JPT_TIMING_PROFILER("std::any NonTrivial Type");

	std::any any;

	for (int32 i = 0; i < 1000000; ++i)
	{
		any = jpt::ToString(i);
	}
}

export void RunBenchmarks_Any()
{
	Benchmark_JptAny_Trivial();
	Benchmark_StdAny_Trivial();

	Benchmark_JptAny_NonTrivial();
	Benchmark_StdAny_NonTrivial();
}