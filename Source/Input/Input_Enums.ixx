// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"
#include "Core/Validation/Assert.h"

#include <string>

export module jpt.Input.Enums;

import jpt.TypeDefs;
import jpt.Hash;
import jpt.String;
import jpt.ToString;

export namespace jpt::Input
{
    JPT_ENUM_UINT8(Type,
        Keyboard,
        Mouse,
        Gamepad,
    );

    JPT_ENUM_UINT8(KeyState,
        Invalid,

        Pressed,    // Newly pressed in last frame
        Released,   // Newly released in last frame

        Down,       // Currently down. No matter if it's newly pressed in last frame
        Up,         // Currently up. No matter if it's newly released in last frame

        Held,       // Pressed for more than one frame
        Smashed,    // Repeatedly pressed within a short time

        ItemCount,
    );

    JPT_ENUM_UINT8(MouseButton,
        Left,
        Right,
        Wheel,
        Button4,
        Button5,
        Button6,
        Button7,

        Invalid,
        );

    JPT_ENUM_UINT8(Key,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        Num_0,
        Num_1,
        Num_2,
        Num_3,
        Num_4,
        Num_5,
        Num_6,
        Num_7,
        Num_8,
        Num_9,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        // Special keys
        Escape,
        Tab,
        CapsLock,
        Shift_Left,
        Shift_Right,
        Ctrl_Left,
        Ctrl_Right,
        Alt_Left,
        Alt_Right,
        Super_Left,
        Super_Right,
        Space,
        Enter,
        Backspace,
        Bracket_Left,
        Bracket_Right,
        Semicolon,
        Quote,
        Comma,
        Period,
        Slash,
        Backslash,
        Tilde,
        Equal,
        Hyphen,

        // Function keys
        Insert,
        Delete,
        Home,
        End,
        PageUp,
        PageDown,
        PrintScreen,
        ScrollLock,
        Pause,

        // Arrows
        Arrow_Left,
        Arrow_Right,
        Arrow_Up,
        Arrow_Down,

        // Numpad
        Numpad_Lock,
        Numpad_0,
        Numpad_1,
        Numpad_2,
        Numpad_3,
        Numpad_4,
        Numpad_5,
        Numpad_6,
        Numpad_7,
        Numpad_8,
        Numpad_9,
        Numpad_Add,
        Numpad_Subtract,
        Numpad_Multiply,
        Numpad_Divide,
        Numpad_Decimal,
        Numpad_Enter,

        Invalid,
        );

    JPT_ENUM_UINT8(Modifier,
        Shift = (1 << 0),
        Ctrl = (1 << 1),
        Alt = (1 << 2),
        Super = (1 << 3),

        Invalid = 0,
        );
}

export JPT_ENUM_HASH(jpt::Input::Key);
export JPT_ENUM_HASH(jpt::Input::KeyState);
export JPT_ENUM_HASH(jpt::Input::MouseButton);
export JPT_ENUM_HASH(jpt::Input::Modifier);

export JPT_ENUM_TO_STRING(jpt::Input::Key);
export JPT_ENUM_TO_STRING(jpt::Input::KeyState);
export JPT_ENUM_TO_STRING(jpt::Input::MouseButton);
export JPT_ENUM_TO_STRING(jpt::Input::Modifier);