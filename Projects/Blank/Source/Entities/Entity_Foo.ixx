// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

export module Entity.Foo;

import jpt.Entity;
import jpt.Event.Manager;

import jpt.Event.Mouse.ButtonPress;
import jpt.Event.Window.Close;
import jpt.Event.Window.Resize;

export class Entity_Foo final : public jpt::Entity
{
public:
	virtual bool PreInit() override;

	void OnMouseButtonPress(const jpt::Event_Mouse_ButtonPress& eventMouseButtonPress);
	void OnWindowResize(const jpt::Event_Window_Resize& eventWindowResize);
};

bool Entity_Foo::PreInit()
{
	jpt::EventManager::GetInstance().Register<jpt::Event_Mouse_ButtonPress>(this, &Entity_Foo::OnMouseButtonPress);
	jpt::EventManager::GetInstance().Register<jpt::Event_Window_Resize>(this, &Entity_Foo::OnWindowResize);

	return true;
}

void Entity_Foo::OnMouseButtonPress(const jpt::Event_Mouse_ButtonPress& eventMouseButtonPress)
{
	JPT_LOG("Entity_Foo::OnMouseButtonPress. %lu x: %i, y: %i, KeyCode: %s", eventMouseButtonPress.GetWindow(), eventMouseButtonPress.GetX(), eventMouseButtonPress.GetY(), eventMouseButtonPress.GetButton().ToString().ConstBuffer());
}

void Entity_Foo::OnWindowResize(const jpt::Event_Window_Resize& eventWindowResize)
{
	JPT_LOG("Entity_Foo::OnWindowResize. %lu width: %i, height: %i", eventWindowResize.GetWindow(), eventWindowResize.GetWidth(), eventWindowResize.GetHeight());
}
