// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module Benchmarks_Core;

/** Benchmark Modules */

// Containers
import Benchmarks_DynamicArray;
import Benchmarks_HashMap;

export void RunBenchmarks_Core()
{
	/** Benchmark Functions */

	// Containers
	RunBenchmarks_DynamicArray();
	RunBenchmarks_HashMap();
}