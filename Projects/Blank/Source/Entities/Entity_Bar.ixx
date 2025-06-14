// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module Entity.Bar;

import jpt.Entity;

import jpt.WindowManager;

import jpt.EventManager;
import jpt.Event_Mouse_Button;
import jpt.Event_Window_Close;
import jpt.Event_Window_Resize;

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