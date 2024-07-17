// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Application_Benchmarks.h"

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

import jpt.Utilities;

import Benchmarks_Core;

bool Application_Benchmarks::Init()
{
	JPT_ENSURE(Super::Init());

	RunBenchmarks_Core();

	return true;
}
