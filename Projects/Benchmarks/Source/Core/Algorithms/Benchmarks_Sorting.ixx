// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"
#include "Profiling/TimingProfiler.h"

#include <algorithm>

export module Benchmarks_Sorting;

import jpt.CoreModules;

static constexpr size_t kBigArraySize    = 100'000;
static constexpr size_t kMedianArraySize = 256;
static constexpr size_t kSmallArraySize  = 16;

jpt::DynamicArray<int32> GenerateRandomArray(size_t size)
{
	jpt::DynamicArray<int32> arr;
	arr.Reserve(size);

	for (int32 i = 0; i < size; ++i)
	{
		arr.EmplaceBack(jpt::RNG::Global().RandInMax(100));
	}

	return arr;
}

void Benchmark_Sortring_QuickSort(size_t arraySize, const char* pLabel)
{
	JPT_SCOPED_TIMING_PROFILER(pLabel);

	jpt::DynamicArray<int32> arr = GenerateRandomArray(arraySize);
	jpt::QuickSort(arr, 0, arr.Size() - 1, [](int32 a, int32 b) {return a < b; });
}

void Benchmark_Sortring_InsertionSort(size_t arraySize, const char* pLabel)
{
	JPT_SCOPED_TIMING_PROFILER(pLabel);

	jpt::DynamicArray<int32> arr = GenerateRandomArray(arraySize);
	jpt::InsertionSort(arr, 0, arr.Size() - 1, [](int32 a, int32 b) {return a < b; });
}

void Benchmark_Sortring_HeapSort(size_t arraySize, const char* pLabel)
{
	JPT_SCOPED_TIMING_PROFILER(pLabel);

	jpt::DynamicArray<int32> arr = GenerateRandomArray(arraySize);
	jpt::HeapSort(arr, arr.Size(), [](int32 a, int32 b) {return a < b; });
}

void Benchmark_Sortring_IntroSort(size_t arraySize, const char* pLabel)
{
	JPT_SCOPED_TIMING_PROFILER(pLabel);

	jpt::DynamicArray<int32> arr = GenerateRandomArray(arraySize);
	jpt::IntroSort(arr, 0, arr.Size() - 1, [](int32 a, int32 b) {return a < b; });
}

export void RunBenchmarks_Sorting()
{
	Benchmark_Sortring_QuickSort    (kBigArraySize, "QuickSort Big");
	Benchmark_Sortring_InsertionSort(kBigArraySize, "Insertion Big");
	Benchmark_Sortring_IntroSort    (kBigArraySize, "Intro     Big");
	Benchmark_Sortring_HeapSort     (kBigArraySize, "Heap      Big");

	JPT_LOG("-----------------------");

	Benchmark_Sortring_QuickSort    (kMedianArraySize, "QuickSort Median");
	Benchmark_Sortring_InsertionSort(kMedianArraySize, "Insertion Median");
	Benchmark_Sortring_IntroSort    (kMedianArraySize, "Intro     Median");
	Benchmark_Sortring_HeapSort     (kMedianArraySize, "Heap      Median");

	JPT_LOG("-----------------------");

	Benchmark_Sortring_QuickSort    (kSmallArraySize, "QuickSort Small");
	Benchmark_Sortring_InsertionSort(kSmallArraySize, "Insertion Small");
	Benchmark_Sortring_IntroSort    (kSmallArraySize, "Intro     Small");
	Benchmark_Sortring_HeapSort     (kSmallArraySize, "Heap      Small");
}