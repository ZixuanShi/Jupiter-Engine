// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/Utils/SyncClient.h"
#include "Core/Minimal/Utilities.h"
#include "Debugging/Logger.h"

module Application_Benchmarks;

import jpt.CoreModules;
import jpt.BenchmarksReporter;
import jpt.FilePathHelpers;
import jpt.LaunchArgs;

import Benchmarks_Core;

bool Application_Benchmarks::PreInit()
{
    jpt::LaunchArgs::GetInstance().Set("noWindow");

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