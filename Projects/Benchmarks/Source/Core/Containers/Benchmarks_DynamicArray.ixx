// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Profiling/TimingProfiler.h"
#include "Debugging/Logger.h"

#include <vector>

export module Benchmarks_DynamicArray;

import jpt.BenchmarksReporter;
import jpt.DynamicArray;
import jpt.String;
import jpt.ToString;
import jpt.TypeDefs;
import jpt.Rand;

static void Add_Trivial(jpt::BenchmarksReporter& reporter)
{
	static constexpr size_t kCount = 1'000'000;
	jpt::StopWatch::Point now;
	jpt::TimePrecision jptResult = 0.0;
	jpt::TimePrecision stdResult = 0.0;

	{
		now = jpt::StopWatch::Now();

		jpt::DynamicArray<int32> dynamicArray;
		dynamicArray.Reserve(kCount);

		for (int32 i = 0; i < kCount; ++i)
		{
			dynamicArray.EmplaceBack(i);
		}

		jptResult = jpt::StopWatch::GetMsFrom(now);
	}

	{
		now = jpt::StopWatch::Now();

		std::vector<int32> stdVector;
		stdVector.reserve(kCount);

		for (int32 i = 0; i < kCount; ++i)
		{
			stdVector.emplace_back(i);
		}

		stdResult = jpt::StopWatch::GetMsFrom(now);
	}

	jpt::BenchmarkUnit unit{ "DynamicArray", "EmplaceBack 1'000'000 elements", jptResult, stdResult };
	reporter.Add(unit);
}

static void Erase_Trivial(jpt::BenchmarksReporter& reporter)
{
	static constexpr size_t kCount = 1'000'000;
	jpt::StopWatch::Point now;
	jpt::TimePrecision jptResult = 0.0;
	jpt::TimePrecision stdResult = 0.0;

	{
		now = jpt::StopWatch::Now();

		jpt::DynamicArray<int32> dynamicArray;
		dynamicArray.Resize(kCount);

		for (int32 i = 0; i < kCount; ++i)
		{
			dynamicArray.Pop();
		}

		jptResult = jpt::StopWatch::GetMsFrom(now);
	}

	{
		now = jpt::StopWatch::Now();

		std::vector<int32> stdVector;
		stdVector.resize(kCount);

		for (int32 i = 0; i < kCount; ++i)
		{
			stdVector.pop_back();
		}

		stdResult = jpt::StopWatch::GetMsFrom(now);
	}

	jpt::BenchmarkUnit unit{ "DynamicArray", "Pop 1'000'000 elements", jptResult, stdResult };
	reporter.Add(unit);
}

static void Iterate()
{
	JPT_SCOPED_TIMING_PROFILER("DynamicArrray iterate");

	static constexpr size_t kCount = 1'000'000;

	jpt::DynamicArray<jpt::String> dynamicArray;
	dynamicArray.Resize(kCount);

	int32 i = 0;
	for (jpt::DynamicArray<jpt::String>::Iterator iterator = dynamicArray.begin(); iterator < dynamicArray.end(); ++iterator)
	{
		*iterator = jpt::ToString(i);
		++i;
	}

	JPT_LOG(dynamicArray[jpt::RNG::Global().MaxInt(kCount)]);	
}

export void RunBenchmarks_DynamicArray(jpt::BenchmarksReporter& reporter)
{
	Add_Trivial(reporter);
	Erase_Trivial(reporter);

	Iterate();
}