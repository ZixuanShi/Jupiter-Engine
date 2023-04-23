#pragma once
#include "Applications/Application.h"

class DemoGameApplication : public jpt::Application
{
public:
	DemoGameApplication();

	virtual bool Init() override final;
};

