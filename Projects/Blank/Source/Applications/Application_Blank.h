// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include "Applications/App/Application.h"

import jpt.Time.TypeDefs;

import Entity.Foo;
import Entity.Bar;

class Application_Blank final : public jpt::Application
{
	using Super = jpt::Application;

private:
	Entity_Foo m_foo;
	Entity_Bar m_bar;

public:
	virtual bool PreInit() override;
	virtual void Update(jpt::TimePrecision deltaSeconds) override;
	virtual void Shutdown() override;
};