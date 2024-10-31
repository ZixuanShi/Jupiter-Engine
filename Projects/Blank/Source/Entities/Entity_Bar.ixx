// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

export module Entity.Bar;

import jpt.Entity;
import jpt.Event.Manager;

import jpt.Event.Mouse.ButtonPress;
import jpt.Event.Window.Close;
import jpt.Event.Window.Resize;

export class Entity_Bar final : public jpt::Entity
{
public:
	virtual bool PreInit() override;

	void OnWindowClose(const jpt::Event_Window_Close& eventWindowClose);
};

bool Entity_Bar::PreInit()
{
	jpt::EventManager::GetInstance().Register<jpt::Event_Window_Close>(this, &Entity_Bar::OnWindowClose);

	return true;
}

void Entity_Bar::OnWindowClose(const jpt::Event_Window_Close& eventWindowClose)
{
	JPT_LOG("Entity_Bar::OnWindowClose. %lu", eventWindowClose.GetWindow());
}