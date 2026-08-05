// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.InputCodes;

import jpt.TypeDefs;
import std;

namespace jpt
{
    namespace local
    {
        // An array, not a switch, so the static_assert below can count it.
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

        static_assert(std::size(kKeyNames) == static_cast<usize>(KeyCode::Count),
                      "KeyCode names are out of sync with the enum");

        constexpr const char* kMouseButtonNames[] =
        {
            "Left",
            "Right",
            "Middle",
        };

        static_assert(std::size(kMouseButtonNames) == static_cast<usize>(MouseButton::Count),
                      "MouseButton names are out of sync with the enum");
    }

    const char* ToString(KeyCode key) noexcept
    {
        const usize index = static_cast<usize>(key);
        return (index < std::size(local::kKeyNames)) ? local::kKeyNames[index] : "Unknown";
    }

    const char* ToString(MouseButton button) noexcept
    {
        const usize index = static_cast<usize>(button);
        return (index < std::size(local::kMouseButtonNames)) ? local::kMouseButtonNames[index] : "Unknown";
    }
}
