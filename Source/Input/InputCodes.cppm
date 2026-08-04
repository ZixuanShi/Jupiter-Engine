// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.InputCodes;

import jpt.TypeDefs;

export namespace jpt
{
    /** Platform-agnostic. A backend translates its own codes into these; nothing translates back.
        Entries with no macOS equivalent stay for the backends that do have them. */
    enum class KeyCode : uint8
    {
        Unknown,

        // Letters
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

        // Digits
        Num0,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,

        // Function
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
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,

        // Arrows
        LeftArrow,
        RightArrow,
        UpArrow,
        DownArrow,

        // Editing and whitespace
        Space,
        Enter,
        Escape,
        Tab,
        Backspace,
        Delete,
        Insert,
        Home,
        End,
        PageUp,
        PageDown,

        // Locks and specials
        CapsLock,
        NumLock,
        ScrollLock,
        PrintScreen,
        Pause,
        Function,

        // Side-specific, and ordinary keys in the down-set rather than a separate bitfield.
        LeftShift,
        RightShift,
        LeftControl,
        RightControl,
        LeftAlt,
        RightAlt,
        LeftSuper,
        RightSuper,

        // Punctuation
        Minus,
        Equal,
        LeftBracket,
        RightBracket,
        Backslash,
        Semicolon,
        Quote,
        Grave,
        Comma,
        Period,
        Slash,

        // Keypad
        Keypad0,
        Keypad1,
        Keypad2,
        Keypad3,
        Keypad4,
        Keypad5,
        Keypad6,
        Keypad7,
        Keypad8,
        Keypad9,
        KeypadDecimal,
        KeypadPlus,
        KeypadMinus,
        KeypadMultiply,
        KeypadDivide,
        KeypadEnter,
        KeypadEquals,
        KeypadClear,

        Count,
    };

    enum class MouseButton : uint8
    {
        Left,
        Right,
        Middle,

        Count,
    };

    [[nodiscard]] const char* ToString(KeyCode key) noexcept;
    [[nodiscard]] const char* ToString(MouseButton button) noexcept;
}
