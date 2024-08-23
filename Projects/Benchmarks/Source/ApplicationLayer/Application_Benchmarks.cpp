// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Application_Benchmarks.h"

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

import jpt.Utilities;
import jpt.CommandLine;

import Benchmarks_Core;

bool Application_Benchmarks::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	jpt::CommandLine::GetInstance().Set("no_window");

	return true;
}

bool Application_Benchmarks::Init()
{
	JPT_ENSURE(Super::Init());

	RunBenchmarks_Core();

	return true;
}
