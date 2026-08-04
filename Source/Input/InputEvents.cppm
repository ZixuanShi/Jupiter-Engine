// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.InputEvents;

import jpt.InputCodes;
import jpt.Vector2;

export namespace jpt
{
    struct KeyEvent
    {
        KeyCode key = KeyCode::Unknown;
        bool isRepeat = false;      // OS auto-repeat, not "held" -- that is Input::IsKeyDown.
    };

    struct MouseButtonEvent
    {
        MouseButton button = MouseButton::Left;
        bool isDown = false;
        Vec2 position = Vec2::Zero();
    };

    struct MouseMoveEvent
    {
        Vec2 position = Vec2::Zero();
        Vec2 delta = Vec2::Zero();
    };

    struct MouseScrollEvent
    {
        Vec2 delta = Vec2::Zero();
        bool isPrecise = false;     // Trackpad continuous delta, rather than a wheel notch.
    };
}
