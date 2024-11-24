// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Profiling/TimingProfiler.h"
#include "Debugging/Logger.h"
#include "Debugging/Assert.h"
#include "Core/Minimal/CoreMacros.h"

#include <string>

export module Benchmarks_String;

import jpt.BenchmarksReporter;
import jpt.String;
import jpt.ToString;
import jpt.TypeDefs;
import jpt.Rand;

void Find(jpt::BenchmarksReporter& reporter)
{
	static constexpr size_t kCount = 1'000'000;
	jpt::StopWatch::Point now;
	jpt::TimePrecision jptResult = 0.0;

	{
		now = jpt::StopWatch::Now();

		for (int32 i = 0; i < kCount; ++i)
		{
			jpt::String str = "Hello Jupiter World";
			JPT_ASSERT(str.Find("Jupiter") == 6);
		}

		jptResult = jpt::StopWatch::GetMsFrom(now);
	}

	jpt::BenchmarkUnit unit{ "String", "Replace 1'000'000 elements", jptResult, 0.0 };
	reporter.Add(unit);
}

void Replace(jpt::BenchmarksReporter& reporter)
{
	static constexpr size_t kCount = 1'000'000;
	jpt::StopWatch::Point now;
	jpt::TimePrecision jptResult = 0.0;

	{
		now = jpt::StopWatch::Now();

		for (int32 i = 0; i < kCount; ++i)
		{
			jpt::String str = "Hello World o";
			str.Replace("o", "Jupiter");
			JPT_ASSERT(str == "HellJupiter WJupiterrld Jupiter");
		}

		jptResult = jpt::StopWatch::GetMsFrom(now);
	}

	jpt::BenchmarkUnit unit{ "String", "Replace 1'000'000 elements", jptResult, 0.0 };
	reporter.Add(unit);
}

export void RunBenchmarks_String(jpt::BenchmarksReporter& reporter)
{
	Find(reporter);
	Replace(reporter);
}