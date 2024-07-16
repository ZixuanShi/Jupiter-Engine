// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.Application_Base;

import Settings;

class Application_JupiterUnitTests final : public jpt::Application_Base
{
private:
	using Super = jpt::Application_Base;

private:

public:
	Settings m_settings;

public:
	virtual bool Init() override final;
};

