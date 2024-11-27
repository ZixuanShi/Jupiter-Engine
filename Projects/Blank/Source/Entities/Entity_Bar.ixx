// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

export module Entity.Bar;

import jpt.Entity;

import jpt.Window.Manager;

import jpt.Event.Manager;
import jpt.Event.Mouse.ButtonPress;
import jpt.Event.Window.Close;
import jpt.Event.Window.Resize;

export class Entity_Bar final : public jpt::Entity
{
public:
	virtual bool PreInit() override;
};

bool Entity_Bar::PreInit()
{
	return true;
}