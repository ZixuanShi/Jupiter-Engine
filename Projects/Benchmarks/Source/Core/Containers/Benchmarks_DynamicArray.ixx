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
	JPT_SCOPED_TIMING_PROFILER(jpt_DynamicArray_Add_Int);

	jpt::DynamicArray<int32> dynamicArray;
	dynamicArray.Reserve(1'000'000);

	for (int32 i = 0; i < 1'000'000; ++i)
	{
		dynamicArray.PushBack(i);
	}
}

void Benchmark_Vector_Add_Int()
{
	JPT_SCOPED_TIMING_PROFILER(std_vector_Add_Int);

	std::vector<int32> vector;
	vector.reserve(1'000'000);

	for (int32 i = 0; i < 1'000'000; ++i)
	{
		vector.push_back(i);
	}
}

void Benchmark_DynamicArray_Add_String()
{
	JPT_SCOPED_TIMING_PROFILER(jpt_DynamicArray_Add_String);

	jpt::DynamicArray<jpt::String> dynamicArray;
	dynamicArray.Reserve(1'000'000);

	for (int32 i = 0; i < 1'000'000; ++i)
	{
		dynamicArray.PushBack(jpt::ToString(i));
	}
}

void Benchmark_Vector_Add_String()
{
	JPT_SCOPED_TIMING_PROFILER(std_vector_Add_String);

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