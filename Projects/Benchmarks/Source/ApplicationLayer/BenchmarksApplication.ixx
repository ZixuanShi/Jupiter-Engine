// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module BenchmarksApplication;

import jpt.Application_Base;
import jpt.Utilities;

import Benchmarks_Core;

export class Application_Benchmarks final : public jpt::Application_Base
{
private:
	using Super = jpt::Application_Base;

public:
	virtual bool Init() override final;
};

bool Application_Benchmarks::Init()
{
	JPT_ENSURE(Super::Init());

	//JPT_ASSERT(IS_PROFILING, "Benchmarks project should only run on Profiling config");

	RunBenchmarks_Core();

	return true;
}
