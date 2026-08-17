// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// A plain translation unit, not a module: this is the one suite that reaches GetApp(), and
// GetApp.h in a .cppm is a cycle in the module import graph. The CMake *.cpp glob picks it up
// as an ordinary source, so it registers exactly like every .cppm suite does.

#include <SDL3/SDL.h>

#include "Applications/GetApp.h"

import jpt.Input;
import jpt.InputCodes;
import jpt.InputEvents;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Window;
import std;

namespace jpt::local
{
    /** The SDL_Scancode translation, driven through the real Window::OnEvent that SDL_AppEvent
        calls. Only SDL's own delivery is left untested, and that cannot be scripted. Needs a live
        window, which is why the suites run from Init() rather than PreInit(). */
    static void SdlEventTest(TestCase& test)
    {
        Input& input = GetApp().GetInput();
        Window& window = GetApp().GetWindow();

        KeyCode received = KeyCode::Unknown;
        const auto handle = input.OnKeyDown().Add([&received](const KeyEvent& event) { received = event.key; });

        const struct { SDL_Scancode scancode; KeyCode expected; const char* what; } kCases[] =
        {
            { SDL_SCANCODE_W,         KeyCode::W,           "W" },
            { SDL_SCANCODE_A,         KeyCode::A,           "A" },
            { SDL_SCANCODE_S,         KeyCode::S,           "S" },
            { SDL_SCANCODE_D,         KeyCode::D,           "D" },
            { SDL_SCANCODE_R,         KeyCode::R,           "R" },
            { SDL_SCANCODE_LEFT,      KeyCode::LeftArrow,   "LeftArrow" },
            { SDL_SCANCODE_UP,        KeyCode::UpArrow,     "UpArrow" },
            { SDL_SCANCODE_ESCAPE,    KeyCode::Escape,      "Escape" },
            { SDL_SCANCODE_SPACE,     KeyCode::Space,       "Space" },
            { SDL_SCANCODE_BACKSPACE, KeyCode::Backspace,   "Backspace" },
            { SDL_SCANCODE_DELETE,    KeyCode::Delete,      "Delete" },
            { SDL_SCANCODE_LSHIFT,    KeyCode::LeftShift,   "LeftShift" },
            { SDL_SCANCODE_RSHIFT,    KeyCode::RightShift,  "RightShift" },
            { SDL_SCANCODE_LGUI,      KeyCode::LeftSuper,   "LeftSuper" },
            { SDL_SCANCODE_GRAVE,     KeyCode::Grave,       "Grave" },
            { SDL_SCANCODE_SLASH,     KeyCode::Slash,       "Slash" },
            { SDL_SCANCODE_KP_9,      KeyCode::Keypad9,     "Keypad9" },
            { SDL_SCANCODE_KP_CLEAR,  KeyCode::KeypadClear, "KeypadClear" },
            { SDL_SCANCODE_F1,        KeyCode::F1,          "F1" },
            { SDL_SCANCODE_F20,       KeyCode::F20,         "F20" },
        };

        SDL_Event event{};
        for (const auto& scancodeCase : kCases)
        {
            received = KeyCode::Unknown;
            event.type = SDL_EVENT_KEY_DOWN;
            event.key.scancode = scancodeCase.scancode;
            event.key.repeat = false;
            window.OnEvent(event);
            test.Expect(received == scancodeCase.expected, "Scancode {} translated to {}, expected {}",
                        static_cast<int32>(scancodeCase.scancode), ToString(received), scancodeCase.what);

            event.type = SDL_EVENT_KEY_UP;
            window.OnEvent(event);
        }

        // A scancode with no row must land on Unknown rather than a neighbouring key.
        received = KeyCode::A;
        event.type = SDL_EVENT_KEY_DOWN;
        event.key.scancode = SDL_SCANCODE_LANG1;
        window.OnEvent(event);
        test.Expect(received == KeyCode::Unknown, "An unmapped scancode translated to {}", ToString(received));
        event.type = SDL_EVENT_KEY_UP;
        window.OnEvent(event);

        input.OnKeyDown().Remove(handle);

        // CapsLock is a plain key to SDL, where AppKit reported only a whole modifier mask. Down
        // and up here still mean engaged and disengaged, not physically held.
        event.key.scancode = SDL_SCANCODE_CAPSLOCK;
        event.type = SDL_EVENT_KEY_DOWN;
        window.OnEvent(event);
        test.Expect(input.IsKeyDown(KeyCode::CapsLock), "CapsLock did not register as engaged");
        event.type = SDL_EVENT_KEY_UP;
        window.OnEvent(event);
        test.Expect(!input.IsKeyDown(KeyCode::CapsLock), "CapsLock stayed engaged after its key up");

        // SDL numbers left 1, middle 2 and right 3 -- the opposite of AppKit, which put right at 1
        // and middle at 2. Nothing else in the engine says so.
        MouseButton receivedButton = MouseButton::Left;
        const auto buttonHandle = input.OnMouseButton().Add([&receivedButton](const MouseButtonEvent& event) { receivedButton = event.button; });

        const struct { std::uint8_t number; MouseButton expected; } kButtons[] =
        {
            { SDL_BUTTON_LEFT,   MouseButton::Left },
            { SDL_BUTTON_MIDDLE, MouseButton::Middle },
            { SDL_BUTTON_RIGHT,  MouseButton::Right },
        };

        for (const auto& buttonCase : kButtons)
        {
            // Position stays at the origin it already holds, so this leaves no phantom delta
            // behind for the first real mouse move.
            event.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
            event.button.button = buttonCase.number;
            event.button.which = 0;
            event.button.x = 0.0f;
            event.button.y = 0.0f;
            window.OnEvent(event);
            test.Expect(receivedButton == buttonCase.expected, "Button {} translated to {}, expected {}", buttonCase.number, ToString(receivedButton), ToString(buttonCase.expected));
            test.Expect(input.IsMouseButtonDown(buttonCase.expected), "Button {} did not register as down", buttonCase.number);

            event.type = SDL_EVENT_MOUSE_BUTTON_UP;
            window.OnEvent(event);
            test.Expect(!input.IsMouseButtonDown(buttonCase.expected), "Button {} stayed down after release", buttonCase.number);
        }

        input.OnMouseButton().Remove(buttonHandle);
    }

    static TestCase s_sdlEvent("Input.SdlEventTranslation", &SdlEventTest);
}
