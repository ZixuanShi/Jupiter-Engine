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

template<typename TFunc>
void Profile(jpt::BenchmarksReporter& reporter, const char* topic, const char* context, size_t testCount, TFunc&& func)
{
	jpt::StopWatch::Point now;
	jpt::TimePrecision jptResult = 0.0;

	{
		now = jpt::StopWatch::Now();

		for (int32 i = 0; i < testCount; ++i)
		{
			func();
		}

		jptResult = jpt::StopWatch::GetMsFrom(now);
	}

	jpt::BenchmarkUnit unit{ topic, context, jptResult };
	reporter.Add(unit);
}

void Find(jpt::BenchmarksReporter& reporter)
{
	Profile(reporter, "String", "Find 1'000'000 elements", 1'000'000, []()
		{
			jpt::String str = "Hello Jupiter World";
			JPT_ASSERT(str.Find("Jupiter") == 6);
		});
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

			str.Replace("Jupiter", "o");
			JPT_ASSERT(str == "Hello World o");
		}

		jptResult = jpt::StopWatch::GetMsFrom(now);
	}

	jpt::BenchmarkUnit unit{ "String", "Replace 1'000'000 elements", jptResult };
	reporter.Add(unit);
}

void SubStr(jpt::BenchmarksReporter& reporter)
{

}

void Split(jpt::BenchmarksReporter& reporter)
{

}

export void RunBenchmarks_String(jpt::BenchmarksReporter& reporter)
{
	Find(reporter);
	Replace(reporter);
}