// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

// The implementation unit is where GetApp.h belongs: it carries `import jpt.ApplicationBase`,
// which in an interface unit would be a cycle in the module import graph.
#include <SDL3/SDL.h>

#include "Applications/GetApp.h"

module UnitTests_SdlEvent;

import jpt.Input;
import jpt.InputCodes;
import jpt.InputEvents;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Window;
import std;

/** The SDL_Scancode translation, driven through the real Window::OnEvent that SDL_AppEvent
    calls. Only SDL's own delivery is left untested, and that cannot be scripted. Needs a live
    window, which is why the suites run from Init() rather than PreInit(). */
void RunUnitTests_SdlEventTranslation(jpt::TestCase& test)
{
    jpt::Input& input = jpt::GetApp().GetInput();
    jpt::Window& window = jpt::GetApp().GetWindow();

    jpt::KeyCode received = jpt::KeyCode::Unknown;
    const auto handle = input.OnKeyDown().Add([&received](const jpt::KeyEvent& event) { received = event.key; });

    const struct { SDL_Scancode scancode; jpt::KeyCode expected; const char* what; } kCases[] =
    {
        { SDL_SCANCODE_W,         jpt::KeyCode::W,           "W" },
        { SDL_SCANCODE_A,         jpt::KeyCode::A,           "A" },
        { SDL_SCANCODE_S,         jpt::KeyCode::S,           "S" },
        { SDL_SCANCODE_D,         jpt::KeyCode::D,           "D" },
        { SDL_SCANCODE_R,         jpt::KeyCode::R,           "R" },
        { SDL_SCANCODE_LEFT,      jpt::KeyCode::LeftArrow,   "LeftArrow" },
        { SDL_SCANCODE_UP,        jpt::KeyCode::UpArrow,     "UpArrow" },
        { SDL_SCANCODE_ESCAPE,    jpt::KeyCode::Escape,      "Escape" },
        { SDL_SCANCODE_SPACE,     jpt::KeyCode::Space,       "Space" },
        { SDL_SCANCODE_BACKSPACE, jpt::KeyCode::Backspace,   "Backspace" },
        { SDL_SCANCODE_DELETE,    jpt::KeyCode::Delete,      "Delete" },
        { SDL_SCANCODE_LSHIFT,    jpt::KeyCode::LeftShift,   "LeftShift" },
        { SDL_SCANCODE_RSHIFT,    jpt::KeyCode::RightShift,  "RightShift" },
        { SDL_SCANCODE_LGUI,      jpt::KeyCode::LeftSuper,   "LeftSuper" },
        { SDL_SCANCODE_GRAVE,     jpt::KeyCode::Grave,       "Grave" },
        { SDL_SCANCODE_SLASH,     jpt::KeyCode::Slash,       "Slash" },
        { SDL_SCANCODE_KP_9,      jpt::KeyCode::Keypad9,     "Keypad9" },
        { SDL_SCANCODE_KP_CLEAR,  jpt::KeyCode::KeypadClear, "KeypadClear" },
        { SDL_SCANCODE_F1,        jpt::KeyCode::F1,          "F1" },
        { SDL_SCANCODE_F20,       jpt::KeyCode::F20,         "F20" },
    };

    SDL_Event event{};
    for (const auto& scancodeCase : kCases)
    {
        received = jpt::KeyCode::Unknown;
        event.type = SDL_EVENT_KEY_DOWN;
        event.key.scancode = scancodeCase.scancode;
        event.key.repeat = false;
        window.OnEvent(event);
        test.Expect(received == scancodeCase.expected, "Scancode {} translated to {}, expected {}",
                    static_cast<int32>(scancodeCase.scancode), jpt::ToString(received), scancodeCase.what);

        event.type = SDL_EVENT_KEY_UP;
        window.OnEvent(event);
    }

    // A scancode with no row must land on Unknown rather than a neighbouring key.
    received = jpt::KeyCode::A;
    event.type = SDL_EVENT_KEY_DOWN;
    event.key.scancode = SDL_SCANCODE_LANG1;
    window.OnEvent(event);
    test.Expect(received == jpt::KeyCode::Unknown, "An unmapped scancode translated to {}", jpt::ToString(received));
    event.type = SDL_EVENT_KEY_UP;
    window.OnEvent(event);

    input.OnKeyDown().Remove(handle);

    // CapsLock is a plain key to SDL, where AppKit reported only a whole modifier mask. Down
    // and up here still mean engaged and disengaged, not physically held.
    event.key.scancode = SDL_SCANCODE_CAPSLOCK;
    event.type = SDL_EVENT_KEY_DOWN;
    window.OnEvent(event);
    test.Expect(input.IsKeyDown(jpt::KeyCode::CapsLock), "CapsLock did not register as engaged");
    event.type = SDL_EVENT_KEY_UP;
    window.OnEvent(event);
    test.Expect(!input.IsKeyDown(jpt::KeyCode::CapsLock), "CapsLock stayed engaged after its key up");

    // SDL numbers left 1, middle 2 and right 3 -- the opposite of AppKit, which put right at 1
    // and middle at 2. Nothing else in the engine says so.
    jpt::MouseButton receivedButton = jpt::MouseButton::Left;
    const auto buttonHandle = input.OnMouseButton().Add([&receivedButton](const jpt::MouseButtonEvent& event) { receivedButton = event.button; });

    // Right is absent, and cannot be pressed here: Camera answers a right press by capturing
    // the cursor, and the matching release warps the *physical* pointer to the anchor. A test
    // that moves the machine's mouse out from under whoever ran it is not one to keep.
    const struct { std::uint8_t number; jpt::MouseButton expected; } kButtons[] =
    {
        { SDL_BUTTON_LEFT,   jpt::MouseButton::Left },
        { SDL_BUTTON_MIDDLE, jpt::MouseButton::Middle },
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
        test.Expect(receivedButton == buttonCase.expected, "Button {} translated to {}, expected {}", buttonCase.number, jpt::ToString(receivedButton), jpt::ToString(buttonCase.expected));
        test.Expect(input.IsMouseButtonDown(buttonCase.expected), "Button {} did not register as down", buttonCase.number);

        event.type = SDL_EVENT_MOUSE_BUTTON_UP;
        window.OnEvent(event);
        test.Expect(!input.IsMouseButtonDown(buttonCase.expected), "Button {} stayed down after release", buttonCase.number);
    }

    // Number 3 is still checked, through a release alone -- the one direction with no side
    // effect, since nothing is captured and SetCursorCaptured(false) then takes its
    // already-false early return. The down/up state above covers what a press adds.
    receivedButton = jpt::MouseButton::Left;
    event.type = SDL_EVENT_MOUSE_BUTTON_UP;
    event.button.button = SDL_BUTTON_RIGHT;
    event.button.which = 0;
    event.button.x = 0.0f;
    event.button.y = 0.0f;
    window.OnEvent(event);
    test.Expect(receivedButton == jpt::MouseButton::Right, "Button {} translated to {}, expected Right",
                static_cast<int32>(SDL_BUTTON_RIGHT), jpt::ToString(receivedButton));
    test.Expect(!input.IsMouseButtonDown(jpt::MouseButton::Right), "Right registered as down after a release alone");

    input.OnMouseButton().Remove(buttonHandle);
}
