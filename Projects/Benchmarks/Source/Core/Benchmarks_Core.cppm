// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module Benchmarks_Core;

/** Benchmark Modules */

import jpt.BenchmarksReporter;

// Containers
import Benchmarks_DynamicArray;
import Benchmarks_HashMap;

// Functional
import Benchmarks_Function;

// Strings
import Benchmarks_String;

// Math
import Benchmarks_Math;

export void RunBenchmarks_Core(jpt::BenchmarksReporter& reporter)
{
    /** Benchmark Functions */

    // Containers
    //RunBenchmarks_DynamicArray(reporter);
    //RunBenchmarks_HashMap(reporter);

    // Functional
    RunBenchmarks_Function(reporter);

    // Strings
    //RunBenchmarks_String(reporter);

    // Math
    //RunBenchmarks_Math(reporter);
}