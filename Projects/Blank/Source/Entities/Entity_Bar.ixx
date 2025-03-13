// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module Entity.Bar;

import jpt.Entity;

import jpt.Window.Manager;

import jpt.Event.Manager;
import jpt.Event.MouseButton;
import jpt.Event.Window.Close;
import jpt.Event.Window.Resize;

export class Entity_Bar final : public jpt::Entity
{
public:
    virtual bool PreInit() override;
};

bool Entity_Bar::PreInit()
{
    JPT_ENSURE(jpt::Entity::PreInit());
    return true;
}