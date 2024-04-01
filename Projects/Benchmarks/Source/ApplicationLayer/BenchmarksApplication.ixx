// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module BenchmarksApplication;

import jpt.ApplicationBase;
import jpt.Utilities;

import Benchmarks_Core;

export class BenchmarksApplication final : public jpt::ApplicationBase
{
private:
	using Super = jpt::ApplicationBase;

public:
	virtual bool Init() override final;
};

bool BenchmarksApplication::Init()
{
	JPT_ENSURE(Super::Init());

	//JPT_ASSERT(IS_PROFILING, "Benchmarks project should only run on Profiling config");

	RunBenchmarks_Core();

	return true;
}
