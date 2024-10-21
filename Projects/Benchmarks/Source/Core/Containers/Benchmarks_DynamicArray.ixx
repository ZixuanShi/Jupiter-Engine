// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vector>

export module Benchmarks_DynamicArray;

import jpt.BenchmarksReporter;
import jpt.DynamicArray;
import jpt.TypeDefs;

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

export void RunBenchmarks_DynamicArray(jpt::BenchmarksReporter& reporter)
{
	Add_Trivial(reporter);
	Erase_Trivial(reporter);


}