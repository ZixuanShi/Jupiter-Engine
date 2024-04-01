// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"
#include "Profiling/TimingProfiler.h"

#include <algorithm>

export module Benchmarks_Sorting;

import jpt.CoreModules;

static constexpr size_t kBigArraySize = 100'000;

void Benchmark_Sortring_Trivial_Big_QuickSort()
{
	JPT_SCOPED_TIMING_PROFILER(JPT_Sortring_Trivial);

	jpt::DynamicArray<int32> arr;
	arr.Reserve(kBigArraySize);

	for (int32 i = 0; i < kBigArraySize; ++i)
	{
		arr.EmplaceBack(jpt::RNG::Global().RandInMax(100));
	}

	jpt::QuickSort(arr.Buffer(), 0, arr.Size() - 1, [](int32 a, int32 b) {return a < b; });
}

void Benchmark_Sortring_Trivial_Big_InsertionSort()
{
	JPT_SCOPED_TIMING_PROFILER(JPT_Sortring_Trivial);

	jpt::DynamicArray<int32> arr;
	arr.Reserve(kBigArraySize);

	for (int32 i = 0; i < kBigArraySize; ++i)
	{
		arr.EmplaceBack(jpt::RNG::Global().RandInMax(100));
	}

	jpt::InsertionSort(arr.Buffer(), 0, arr.Size() - 1, [](int32 a, int32 b) {return a < b; });
}

export void RunBenchmarks_Sorting()
{
	Benchmark_Sortring_Trivial_Big_QuickSort();
	Benchmark_Sortring_Trivial_Big_InsertionSort();
}