// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module Benchmarks_Core;

/** Benchmark Modules */

// Algorithms
import Benchmarks_Sorting;

// Containers
import Benchmarks_DynamicArray;

// Math
import Benchmarks_Math;

// Types
import Benchmarks_Any;
import Benchmarks_Enum;

import jpt.Utilities;

export void RunBenchmarks_Core()
{
	/** Unit Test Functions */

	// Algorithms
	RunBenchmarks_Sorting();

	// Containers
	//RunBenchmarks_DynamicArray();

	// Math
	//RunBenchmarks_Math();

	// Types
	//RunBenchmarks_Any();
	RunBenchmarks_Enum();
}