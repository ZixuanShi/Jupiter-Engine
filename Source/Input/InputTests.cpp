// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_MACOS
    #include <Carbon/Carbon.h>

    #include "Applications/AppClient.h"
    #include "Applications/Window/Apple/AppleCallbacks.h"
#endif

module jpt.InputTests;

import jpt.Assert;
import jpt.Constants;
import jpt.EventDispatcher;
import jpt.Input;
import jpt.InputCodes;
import jpt.InputEvents;
import jpt.Logger;
import jpt.TypeDefs;
import jpt.Vector2;
import std;

namespace jpt
{
    namespace
    {
        struct TestEvent
        {
            int32 value = 0;
        };

        using TestDispatcher = EventDispatcher<TestEvent>;
    }

    void RunInputTests()
    {
        // EventDispatcher -- registration
        {
            TestDispatcher dispatcher;

            int32 firstCount = 0;
            int32 secondCount = 0;

            const TestDispatcher::Handle first = dispatcher.Add([&firstCount](const TestEvent&) { ++firstCount; });
            const TestDispatcher::Handle second = dispatcher.Add([&secondCount](const TestEvent&) { ++secondCount; });

            Debug::Assert(first != second, "Add returned the same handle twice");
            Debug::Assert(first != kInvalid<TestDispatcher::Handle>, "Add returned the invalid handle");
            Debug::Assert(dispatcher.GetCount() == 2, "Dispatcher holds {} handlers, expected 2", dispatcher.GetCount());

            dispatcher.Dispatch(TestEvent{});
            Debug::Assert(firstCount == 1 && secondCount == 1, "Dispatch reached {} and {}, expected 1 and 1", firstCount, secondCount);

            // Remove must stop exactly one handler.
            dispatcher.Remove(first);
            dispatcher.Dispatch(TestEvent{});
            Debug::Assert(firstCount == 1, "Removed handler still fired, count is {}", firstCount);
            Debug::Assert(secondCount == 2, "Surviving handler stopped firing, count is {}", secondCount);

            // Removing an unknown handle is a no-op, not a crash or a silent extra tombstone.
            dispatcher.Remove(first);
            Debug::Assert(dispatcher.GetCount() == 1, "Repeated Remove changed the count to {}", dispatcher.GetCount());
        }

        // EventDispatcher -- re-entrancy. Legacy's Send was undefined behaviour here.
        {
            TestDispatcher dispatcher;

            // Removing itself destroys the std::function it is executing in, unless Remove defers.
            int32 selfCount = 0;
            TestDispatcher::Handle self = kInvalid<TestDispatcher::Handle>;
            self = dispatcher.Add([&dispatcher, &selfCount, &self](const TestEvent&)
                {
                    ++selfCount;
                    dispatcher.Remove(self);
                });

            dispatcher.Dispatch(TestEvent{});
            dispatcher.Dispatch(TestEvent{});
            Debug::Assert(selfCount == 1, "Self-removing handler fired {} times, expected 1", selfCount);
        }
        {
            TestDispatcher dispatcher;

            // Adding grows the container; the new handler must not see the in-flight event.
            int32 addedCount = 0;
            int32 adderCount = 0;
            dispatcher.Add([&dispatcher, &addedCount, &adderCount](const TestEvent&)
                {
                    ++adderCount;
                    if (adderCount == 1)
                    {
                        dispatcher.Add([&addedCount](const TestEvent&) { ++addedCount; });
                    }
                });

            dispatcher.Dispatch(TestEvent{});
            Debug::Assert(addedCount == 0, "Handler added mid-dispatch received the in-flight event");

            dispatcher.Dispatch(TestEvent{});
            Debug::Assert(addedCount == 1, "Handler added mid-dispatch never fired, count is {}", addedCount);
        }

        // A local Input, so the dev logger is not attached.
        {
            Input input;

            KeyCode receivedKey = KeyCode::Unknown;
            bool receivedRepeat = false;
            input.OnKeyDown().Add([&receivedKey, &receivedRepeat](const KeyEvent& event)
                {
                    receivedKey = event.key;
                    receivedRepeat = event.isRepeat;
                });

            Debug::Assert(!input.IsKeyDown(KeyCode::W), "A fresh Input reports W already down");

            input.PostKeyDown(KeyCode::W, false);
            Debug::Assert(input.IsKeyDown(KeyCode::W), "PostKeyDown did not set the down state");
            Debug::Assert(receivedKey == KeyCode::W, "OnKeyDown delivered {}, expected W", ToString(receivedKey));
            Debug::Assert(!receivedRepeat, "isRepeat was set on a first press");

            input.PostKeyDown(KeyCode::W, true);
            Debug::Assert(receivedRepeat, "isRepeat did not survive the trip through the event");
            Debug::Assert(input.IsKeyDown(KeyCode::W), "A repeat cleared the down state");

            input.PostKeyUp(KeyCode::W);
            Debug::Assert(!input.IsKeyDown(KeyCode::W), "PostKeyUp did not clear the down state");

            // Modifiers are ordinary keys, which is what lets a mouse handler ask about them.
            input.PostKeyDown(KeyCode::LeftShift, false);
            Debug::Assert(input.IsKeyDown(KeyCode::LeftShift), "Modifiers are not tracked in the down-set");
            input.PostKeyUp(KeyCode::LeftShift);

            Vec2 receivedPosition = Vec2::Zero();
            input.OnMouseButton().Add([&receivedPosition](const MouseButtonEvent& event) { receivedPosition = event.position; });

            input.PostMouseButton(MouseButton::Left, true, Vec2(12.0f, 34.0f));
            Debug::Assert(input.IsMouseButtonDown(MouseButton::Left), "PostMouseButton did not set the down state");
            Debug::Assert(receivedPosition == Vec2(12.0f, 34.0f), "Mouse position did not survive the event");
            Debug::Assert(input.GetMousePosition() == Vec2(12.0f, 34.0f), "A button press did not update the cached position");

            input.PostMouseButton(MouseButton::Left, false, Vec2(12.0f, 34.0f));
            Debug::Assert(!input.IsMouseButtonDown(MouseButton::Left), "Releasing did not clear the down state");

            Vec2 receivedDelta = Vec2::Zero();
            input.OnMouseMove().Add([&receivedDelta](const MouseMoveEvent& event) { receivedDelta = event.delta; });

            input.PostMouseMove(Vec2(20.0f, 30.0f));
            Debug::Assert(receivedDelta == Vec2(8.0f, -4.0f), "Mouse delta is ({}, {}), expected (8, -4)", receivedDelta.x, receivedDelta.y);
        }

        // Name tables. Checked at both ends, because a short table only misbehaves at the end.
        {
            Debug::Assert(std::string_view(ToString(KeyCode::A)) == "A", "KeyCode::A is named {}", ToString(KeyCode::A));
            Debug::Assert(std::string_view(ToString(KeyCode::KeypadClear)) == "KeypadClear",
                          "The last KeyCode is named {}", ToString(KeyCode::KeypadClear));
            Debug::Assert(std::string_view(ToString(KeyCode::LeftSuper)) == "LeftSuper",
                          "KeyCode::LeftSuper is named {}", ToString(KeyCode::LeftSuper));
            Debug::Assert(std::string_view(ToString(MouseButton::Middle)) == "Middle",
                          "MouseButton::Middle is named {}", ToString(MouseButton::Middle));
        }

#if IS_PLATFORM_MACOS
        // The kVK_* translation, driven through the real callback the event monitor calls.
        // Only AppKit's delivery is left untested, and that cannot be scripted.
        {
            Input& input = GetApplication().GetInput();

            KeyCode received = KeyCode::Unknown;
            const auto handle = input.OnKeyDown().Add([&received](const KeyEvent& event) { received = event.key; });

            const struct { std::uint16_t platform; KeyCode expected; const char* what; } kCases[] =
            {
                { kVK_ANSI_W,           KeyCode::W,           "W" },
                { kVK_ANSI_A,           KeyCode::A,           "A" },
                { kVK_ANSI_S,           KeyCode::S,           "S" },
                { kVK_ANSI_D,           KeyCode::D,           "D" },
                { kVK_ANSI_R,           KeyCode::R,           "R" },
                { kVK_LeftArrow,        KeyCode::LeftArrow,   "LeftArrow" },
                { kVK_UpArrow,          KeyCode::UpArrow,     "UpArrow" },
                { kVK_Escape,           KeyCode::Escape,      "Escape" },
                { kVK_Space,            KeyCode::Space,       "Space" },
                { kVK_Delete,           KeyCode::Backspace,   "Backspace (kVK_Delete)" },
                { kVK_ForwardDelete,    KeyCode::Delete,      "Delete (kVK_ForwardDelete)" },
                { kVK_Shift,            KeyCode::LeftShift,   "LeftShift" },
                { kVK_RightShift,       KeyCode::RightShift,  "RightShift" },
                { kVK_Command,          KeyCode::LeftSuper,   "LeftSuper" },
                { kVK_ANSI_Grave,       KeyCode::Grave,       "Grave" },
                { kVK_ANSI_Slash,       KeyCode::Slash,       "Slash" },
                { kVK_ANSI_Keypad9,     KeyCode::Keypad9,     "Keypad9" },
                { kVK_ANSI_KeypadClear, KeyCode::KeypadClear, "KeypadClear" },
                { kVK_F1,               KeyCode::F1,          "F1" },
                { kVK_F20,              KeyCode::F20,         "F20" },
            };

            for (const auto& testCase : kCases)
            {
                received = KeyCode::Unknown;
                jpt::OnKeyDown(testCase.platform, false);
                Debug::Assert(received == testCase.expected, "kVK 0x{:X} translated to {}, expected {}",
                              testCase.platform, ToString(received), testCase.what);
                jpt::OnKeyUp(testCase.platform);
            }

            // A code with no row must land on Unknown rather than a neighbouring key.
            received = KeyCode::A;
            jpt::OnKeyDown(kVK_JIS_Kana, false);
            Debug::Assert(received == KeyCode::Unknown, "An unmapped kVK translated to {}", ToString(received));
            jpt::OnKeyUp(kVK_JIS_Kana);

            input.OnKeyDown().Remove(handle);
        }
        {
            // Modifiers arrive as a whole mask, never down/up, so this path translates separately.
            Input& input = GetApplication().GetInput();

            constexpr std::uint32_t kLeftShiftBit = 0x0002;         // Hardware-dependent, has a side.
            constexpr std::uint32_t kCapsLockFlag = 1u << 16;       // NSEventModifierFlagCapsLock.

            jpt::OnModifierChanged(kVK_Shift, kLeftShiftBit);
            Debug::Assert(input.IsKeyDown(KeyCode::LeftShift), "A flags-changed event did not put LeftShift down");
            jpt::OnModifierChanged(kVK_Shift, 0);
            Debug::Assert(!input.IsKeyDown(KeyCode::LeftShift), "A cleared mask did not lift LeftShift");

            // CapsLock has no left/right bit, which is exactly why it was silently dropped before.
            // Down and up here mean engaged and disengaged, not physically held.
            jpt::OnModifierChanged(kVK_CapsLock, kCapsLockFlag);
            Debug::Assert(input.IsKeyDown(KeyCode::CapsLock), "CapsLock was dropped -- ModifierMask has no row for it");
            jpt::OnModifierChanged(kVK_CapsLock, 0);
            Debug::Assert(!input.IsKeyDown(KeyCode::CapsLock), "CapsLock stayed engaged after its flag cleared");
        }
#endif

        Debug::Log("Input tests passed.");
    }
}
