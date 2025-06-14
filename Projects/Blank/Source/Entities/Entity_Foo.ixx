// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module Entity.Foo;

import jpt.Application;

import jpt.Renderer;
import jpt.Graphics_Enums;

import jpt.Entity;
import jpt.EventManager;
import jpt.Input_Enums;

import jpt.Event_Key;
import jpt.Event_Mouse_Button;
import jpt.Event_Window_Close;
import jpt.Event_Window_Resize;

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
    const jpt::Input::Key key = eventKeyboardKey.GetKey();
    const jpt::Input::KeyState keyState = eventKeyboardKey.GetState();

    const bool hasCtrlMod = eventKeyboardKey.HasModifier(jpt::Input::Modifier::Ctrl);
    const jpt::String hasCtrlModStr = hasCtrlMod ? "true" : "false";

    JPT_LOG("Entity_Foo::OnKeyPress. %lu Key: %s, Has Ctrl: %s", eventKeyboardKey.GetWindow(), jpt::ToString(eventKeyboardKey.GetKey()).ConstBuffer(), hasCtrlModStr.ConstBuffer());

    // toggle VSync on Ctrl + V
    if (key == jpt::Input::Key::V &&
        hasCtrlMod && 
        keyState == jpt::Input::KeyState::Pressed)
    {
        const jpt::VSyncMode vsyncMode = jpt::GetGraphicsSettings().GetVSyncMode();
        jpt::GetGraphicsSettings().SetVSyncMode(vsyncMode == jpt::VSyncMode::On ? jpt::VSyncMode::Off : jpt::VSyncMode::On);
    }

    // Resize
    if (key == jpt::Input::Key::R &&
        hasCtrlMod && 
        keyState == jpt::Input::KeyState::Pressed)
    {
        jpt::GetApplication()->GetMainWindow()->Resize(Vec2i(480, 480));
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
