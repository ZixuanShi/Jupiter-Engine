// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Application_Benchmarks.h"

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

import jpt.CoreModules;
import jpt.BenchmarksReporter;

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

#pragma region Engine-Client Communications
constexpr const wchar_t* jpt::File::GetClientDirW() { return JPT_CLIENT_DIR_W; }
constexpr const wchar_t* jpt::File::GetOutputDirW() { return JPT_OUTPUT_DIR_W; }
#pragma endregion