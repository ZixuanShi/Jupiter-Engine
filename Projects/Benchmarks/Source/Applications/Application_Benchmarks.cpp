// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "System/Environment/SyncClient.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

module Application_Benchmarks;

import jpt.CoreModules;
import jpt.BenchmarksReporter;
import jpt.File.Path.Helpers;
import jpt.CommandLine;

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

JPT_SYNC_CLIENT(Benchmarks)