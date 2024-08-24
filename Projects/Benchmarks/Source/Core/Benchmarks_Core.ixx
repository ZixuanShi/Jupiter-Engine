// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module Benchmarks_Core;

/** Benchmark Modules */

import jpt.BenchmarksReporter;

// Containers
import Benchmarks_DynamicArray;
import Benchmarks_HashMap;

export void RunBenchmarks_Core(jpt::BenchmarksReporter& reporter)
{
	/** Benchmark Functions */

	// Containers
	RunBenchmarks_DynamicArray(reporter);
	RunBenchmarks_HashMap();
}