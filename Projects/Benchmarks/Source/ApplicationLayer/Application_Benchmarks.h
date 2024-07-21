// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include "Applications/App/Application_Base.h"

class Application_Benchmarks final : public jpt::Application_Base
{
private:
	using Super = jpt::Application_Base;

public:
	virtual bool Init() override;
};