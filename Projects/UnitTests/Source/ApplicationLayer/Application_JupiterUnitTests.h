// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.Application_Base;

class Application_JupiterUnitTests final : public jpt::Application_Base
{
private:
	using Super = jpt::Application_Base;

public:
	virtual bool Init() override final;
};

