// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Profiling/TimingProfiler.h"
#include "Debugging/Logger.h"

#include <string>

export module Benchmarks_String;

import jpt.BenchmarksReporter;
import jpt.String;
import jpt.ToString;
import jpt.TypeDefs;
import jpt.Rand;

static void ToString(jpt::BenchmarksReporter& reporter)
{
	static constexpr size_t kCount = 1'000'000;
	jpt::StopWatch::Point now;
	jpt::TimePrecision jptResult = 0.0;

	{
		now = jpt::StopWatch::Now();
		jpt::String str;

		for (int32 i = 0; i < kCount; ++i)
		{
			str = "string 1, " + jpt::ToString(i) + ". string 2, " + jpt::ToString(i);
		}

		jptResult = jpt::StopWatch::GetMsFrom(now);
	}

	jpt::BenchmarkUnit unit{ "String", "ToString 1'000'000 elements", jptResult, 0.0 };
	reporter.Add(unit);
}

static void Format(jpt::BenchmarksReporter& reporter)
{
	static constexpr size_t kCount = 1'000'000;
	jpt::StopWatch::Point now;
	jpt::TimePrecision jptResult = 0.0;

	{
		now = jpt::StopWatch::Now();
		jpt::String str;

		for (int32 i = 0; i < kCount; ++i)
		{
			str = jpt::String::Format<128>("string 1, %d. string 2, ", i);
		}

		jptResult = jpt::StopWatch::GetMsFrom(now);
	}

	jpt::BenchmarkUnit unit{ "String", "Format 1'000'000 elements", jptResult, 0.0 };
	reporter.Add(unit);
}

export void RunBenchmarks_String(jpt::BenchmarksReporter& reporter)
{
	ToString(reporter);
	Format(reporter);
}