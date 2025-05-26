// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module Entity.Foo;

import jpt.Application;
import jpt.Renderer;

import jpt.Entity;
import jpt.Event.Manager;
import jpt.Input.Enums;

import jpt.Event.Key;
import jpt.Event.MouseButton;
import jpt.Event.Window.Close;
import jpt.Event.Window.Resize;

export class Entity_Foo final : public jpt::Entity
{
public:
    virtual bool PreInit() override;

    void OnKey(const jpt::Event_Key& eventKeyboardKey);
    void OnMouseButton(const jpt::Event_Mouse_Button& eventMouseButton);
    void OnWindowResize(const jpt::Event_Window_Resize& eventWindowResize);
};

bool Entity_Foo::PreInit()
{
    JPT_ENSURE(jpt::Entity::PreInit());

    jpt::EventManager::GetInstance().Register<jpt::Event_Key>(this, &Entity_Foo::OnKey);
    jpt::EventManager::GetInstance().Register<jpt::Event_Mouse_Button>(this, &Entity_Foo::OnMouseButton);
    jpt::EventManager::GetInstance().Register<jpt::Event_Window_Resize>(this, &Entity_Foo::OnWindowResize);

    return true;
}

void Entity_Foo::OnKey(const jpt::Event_Key& eventKeyboardKey)
{
    const bool hasCtrlMod = eventKeyboardKey.HasModifier(jpt::Input::Modifier::Ctrl);
    const jpt::String hasCtrlModStr = hasCtrlMod ? "true" : "false";
    JPT_LOG("Entity_Foo::OnKeyPress. %lu Key: %s, Has Ctrl: %s", eventKeyboardKey.GetWindow(), jpt::ToString(eventKeyboardKey.GetKey()).ConstBuffer(), hasCtrlModStr.ConstBuffer());

    // toggle VSync on Ctrl + V
    if (eventKeyboardKey.GetKey() == jpt::Input::Key::V &&
        hasCtrlMod && 
        eventKeyboardKey.GetState() == jpt::Input::KeyState::Pressed)
    {
        const bool isVSyncOn = jpt::GetGraphicsSettings().IsVSyncOn();
        jpt::GetGraphicsSettings().SetVSyncOn(!isVSyncOn);
    }
}

void Entity_Foo::OnMouseButton(const jpt::Event_Mouse_Button& eventMouseButton)
{
    const bool hasShiftMod = eventMouseButton.HasModifier(jpt::Input::Modifier::Shift);
    const jpt::String hasShiftModStr = hasShiftMod ? "true" : "false";
    JPT_LOG("Entity_Foo::OnMouseButtonPress. %lu x: %i, y: %i, Mouse Button: %s, Has Shift: %s", eventMouseButton.GetWindow(), eventMouseButton.GetX(), eventMouseButton.GetY(), jpt::ToString(eventMouseButton.GetButton()).ConstBuffer(), hasShiftModStr.ConstBuffer());
}

void Entity_Foo::OnWindowResize(const jpt::Event_Window_Resize& eventWindowResize)
{
    JPT_LOG("Entity_Foo::OnWindowResize. %lu width: %i, height: %i", eventWindowResize.GetWindow(), eventWindowResize.GetWidth(), eventWindowResize.GetHeight());
}
