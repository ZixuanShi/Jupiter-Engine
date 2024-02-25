// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module Benchmarks_Core;

/** Benchmark Modules */

// Containers
import Benchmarks_DynamicArray;

// Types
import Benchmarks_Any;

import jpt.Utilities;

export void RunBenchmarks_Core()
{
	/** Unit Test Functions */

	// Containers
	RunBenchmarks_DynamicArray();

	// Types
	RunBenchmarks_Any();
}