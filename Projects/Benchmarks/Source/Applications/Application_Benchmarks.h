// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include "Applications/App/Application.h"

class Application_Benchmarks final : public jpt::Application
{
private:
	using Super = jpt::Application;

public:
	virtual bool PreInit() override;
	virtual bool Init() override;
};