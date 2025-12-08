// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Profiling/TimingProfiler.h"
#include "Debugging/Logger.h"
#include "Core/Validation/Assert.h"

#include <functional>

export module Benchmarks_Function;

import jpt.BenchmarksReporter;

import jpt.Function;
import jpt.TypeDefs;

// Test data
static constexpr int32 kCount = 1'000'000;
bool SampleFunction(int32 num) { return num > 0; }
bool SampleFunction2(int32 num) { return num < 0; }

void jptFunction_Call([[maybe_unused]] jpt::BenchmarksReporter& reporter)
{
    JPT_SCOPED_TIMING_PROFILER("Benchmarks_Function::RunBenchmarks_jptFunction");

    jpt::Function<bool(int32)> func = &SampleFunction;
    for (int32 i = 0; i < kCount; ++i)
    {
        func(i);
    }
}

void stdFunction_Call([[maybe_unused]] jpt::BenchmarksReporter& reporter)
{
    JPT_SCOPED_TIMING_PROFILER("Benchmarks_Function::RunBenchmarks_stdFunction");

    std::function<bool(int32)> func = &SampleFunction;
    for (int32 i = 0; i < kCount; ++i)
    {
        func(i);
    }
}

void jptFunction_Assign([[maybe_unused]] jpt::BenchmarksReporter& reporter)
{
    JPT_SCOPED_TIMING_PROFILER("Benchmarks_Function::RunBenchmarks_jptFunction_Assign");

    jpt::Function<bool(int32)> func;
    for (int32 i = 0; i < kCount; ++i)
    {
        if (i % 2 == 0)
        {
            func = &SampleFunction;
        }
        else
        {
            func = &SampleFunction2;
        }
    }
}

void stdFunction_Assign([[maybe_unused]] jpt::BenchmarksReporter& reporter)
{
    JPT_SCOPED_TIMING_PROFILER("Benchmarks_Function::RunBenchmarks_stdFunction_Assign");
    std::function<bool(int32)> func;
    for (int32 i = 0; i < kCount; ++i)
    {
        if (i % 2 == 0)
        {
            func = &SampleFunction;
        }
        else
        {
            func = &SampleFunction2;
        }
    }
}

export void RunBenchmarks_Function(jpt::BenchmarksReporter& reporter)
{
    jptFunction_Call(reporter);
    stdFunction_Call(reporter);

    jptFunction_Assign(reporter);
    stdFunction_Assign(reporter);
}