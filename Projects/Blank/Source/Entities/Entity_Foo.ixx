// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module Entity.Foo;

import jpt.Entity;
import jpt.Event.Manager;

import jpt.Event.Key;
import jpt.Event.MouseButton;
import jpt.Event.Window.Close;
import jpt.Event.Window.Resize;

export class Entity_Foo final : public jpt::Entity
{
public:
	virtual bool PreInit() override;

	void OnKeyPress(const jpt::Event_Key& eventKeyboardKeyPress);
	void OnMouseButtonPress(const jpt::Event_Mouse_Button& eventMouseButtonPress);
	void OnWindowResize(const jpt::Event_Window_Resize& eventWindowResize);
};

bool Entity_Foo::PreInit()
{
	JPT_ENSURE(jpt::Entity::PreInit());

	jpt::EventManager::GetInstance().Register<jpt::Event_Key>(this, &Entity_Foo::OnKeyPress);
	jpt::EventManager::GetInstance().Register<jpt::Event_Mouse_Button>(this, &Entity_Foo::OnMouseButtonPress);
	jpt::EventManager::GetInstance().Register<jpt::Event_Window_Resize>(this, &Entity_Foo::OnWindowResize);

	return true;
}

void Entity_Foo::OnKeyPress(const jpt::Event_Key& eventKeyboardKeyPress)
{
	const bool hasCtrlMod = eventKeyboardKeyPress.HasModifier(jpt::Input::Modifier::Ctrl);
	const jpt::String hasCtrlModStr = hasCtrlMod ? "true" : "false";
	JPT_LOG("Entity_Foo::OnKeyPress. %lu Key: %s, Has Ctrl: %s", eventKeyboardKeyPress.GetWindow(), eventKeyboardKeyPress.GetKey().ToString().ConstBuffer(), hasCtrlModStr.ConstBuffer());
}

void Entity_Foo::OnMouseButtonPress(const jpt::Event_Mouse_Button& eventMouseButtonPress)
{
	const bool hasShiftMod = eventMouseButtonPress.HasModifier(jpt::Input::Modifier::Shift);
	const jpt::String hasShiftModStr = hasShiftMod ? "true" : "false";
	JPT_LOG("Entity_Foo::OnMouseButtonPress. %lu x: %i, y: %i, Mouse Button: %s, Has Shift: %s", eventMouseButtonPress.GetWindow(), eventMouseButtonPress.GetX(), eventMouseButtonPress.GetY(), eventMouseButtonPress.GetButton().ToString().ConstBuffer(), hasShiftModStr.ConstBuffer());
}

void Entity_Foo::OnWindowResize(const jpt::Event_Window_Resize& eventWindowResize)
{
	JPT_LOG("Entity_Foo::OnWindowResize. %lu width: %i, height: %i", eventWindowResize.GetWindow(), eventWindowResize.GetWidth(), eventWindowResize.GetHeight());
}
