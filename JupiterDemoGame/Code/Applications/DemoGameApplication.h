#pragma once
#include "Applications/Application.h"

class DemoGameApplication final : public jpt::Application
{
public:
	DemoGameApplication();

	virtual bool Init() override;
};

