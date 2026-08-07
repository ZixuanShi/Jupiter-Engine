// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.InputCodes;

import jpt.TypeDefs;
import std;

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

    enum class TouchPhase : uint8
    {
        Began,
        Moved,
        Ended,
        Cancelled,
    };

    [[nodiscard]] const char* ToString(KeyCode key) noexcept;
    [[nodiscard]] const char* ToString(MouseButton button) noexcept;
}

/** Not exported: ToString is the only way out. Arrays rather than switches, so the static_asserts
    below can count them against the enums they mirror -- which is why they sit here, next to what
    they have to stay in step with, rather than in InputCodes.cpp. */
namespace jpt::local
{
    constexpr const char* kKeyNames[] =
    {
        "Unknown",

        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
        "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",

        "Num0", "Num1", "Num2", "Num3", "Num4",
        "Num5", "Num6", "Num7", "Num8", "Num9",

        "F1",  "F2",  "F3",  "F4",  "F5",  "F6",  "F7",  "F8",  "F9",  "F10",
        "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20",

        "LeftArrow", "RightArrow", "UpArrow", "DownArrow",

        "Space", "Enter", "Escape", "Tab", "Backspace", "Delete", "Insert",
        "Home", "End", "PageUp", "PageDown",

        "CapsLock", "NumLock", "ScrollLock", "PrintScreen", "Pause", "Function",

        "LeftShift",   "RightShift",
        "LeftControl", "RightControl",
        "LeftAlt",     "RightAlt",
        "LeftSuper",   "RightSuper",

        "Minus", "Equal", "LeftBracket", "RightBracket", "Backslash",
        "Semicolon", "Quote", "Grave", "Comma", "Period", "Slash",

        "Keypad0", "Keypad1", "Keypad2", "Keypad3", "Keypad4",
        "Keypad5", "Keypad6", "Keypad7", "Keypad8", "Keypad9",
        "KeypadDecimal", "KeypadPlus", "KeypadMinus", "KeypadMultiply",
        "KeypadDivide", "KeypadEnter", "KeypadEquals", "KeypadClear",
    };
    static_assert(std::size(kKeyNames) == static_cast<usize>(KeyCode::Count), "KeyCode names are out of sync with the enum");

    constexpr const char* kMouseButtonNames[] =
    {
        "Left",
        "Right",
        "Middle",
    };
    static_assert(std::size(kMouseButtonNames) == static_cast<usize>(MouseButton::Count), "MouseButton names are out of sync with the enum");
}
