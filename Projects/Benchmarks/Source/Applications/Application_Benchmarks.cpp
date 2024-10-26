// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Application_Benchmarks.h"

#include "Applications/SyncClient.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

import jpt.CoreModules;
import jpt.BenchmarksReporter;
import jpt.File.Path.Helpers;

import Benchmarks_Core;

bool Application_Benchmarks::PreInit()
{
	jpt::CommandLine::GetInstance().Set("no_window");

	JPT_ENSURE(Super::PreInit());

	return true;
}

bool Application_Benchmarks::Init()
{
	JPT_ENSURE(Super::Init());

	jpt::BenchmarksReporter reporter;
	
	RunBenchmarks_Core(reporter);
	
	reporter.Finalize();
	reporter.LogResults();

	return true;
}

JPT_SYNC_CLIENT(Application_Benchmarks)