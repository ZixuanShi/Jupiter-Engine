// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"
#include "Profiling/TimingProfiler.h"

#include <vector>
#include <string>

export module Benchmarks_DynamicArray;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.String;
import jpt.DynamicArray;

void Benchmark_DynamicArray_Add_Int()
{
	JPT_TIMING_PROFILER("jpt::DynamicArray Add Int");

	jpt::DynamicArray<int32> dynamicArray;
	dynamicArray.Reserve(1'000'000);

	for (int32 i = 0; i < 1'000'000; ++i)
	{
		dynamicArray.PushBack(i);
	}
}

void Benchmark_Vector_Add_Int()
{
	JPT_TIMING_PROFILER("std::vector Add Int");

	std::vector<int32> vector;
	vector.reserve(1'000'000);

	for (int32 i = 0; i < 1'000'000; ++i)
	{
		vector.push_back(i);
	}
}

void Benchmark_DynamicArray_Add_String()
{
	JPT_TIMING_PROFILER("jpt::DynamicArray Add String");

	jpt::DynamicArray<std::string> dynamicArray;
	dynamicArray.Reserve(1'000'000);

	for (int32 i = 0; i < 1'000'000; ++i)
	{
		dynamicArray.PushBack(std::to_string(i));
	}
}

void Benchmark_Vector_Add_String()
{
	JPT_TIMING_PROFILER("std::vector Add String");

	std::vector<std::string> vector;
	vector.reserve(1'000'000);

	for (int32 i = 0; i < 1'000'000; ++i)
	{
		vector.push_back(std::to_string(i));
	}
}

export void RunBenchmarks_DynamicArray()
{
	Benchmark_DynamicArray_Add_Int();
	Benchmark_Vector_Add_Int();

	Benchmark_DynamicArray_Add_String();
	Benchmark_Vector_Add_String();
}