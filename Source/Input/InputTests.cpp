// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_MACOS
    #include <Carbon/Carbon.h>

    #include "Applications/AppClient.h"
    #include "Applications/Window/Apple/AppleCallbacks.h"
#endif

module jpt.InputTests;

import jpt.Assert;
import jpt.Camera;
import jpt.Math;
import jpt.Constants;
import jpt.EventDispatcher;
import jpt.GestureRecognizer;
import jpt.Input;
import jpt.InputCodes;
import jpt.InputEvents;
import jpt.Logger;
import jpt.Quaternion;
import jpt.Scene;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;
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

        // Gestures. The only coverage they can get: there is no iOS Simulator in this project
        // and synthetic touches need assistive access, so UIKit's delivery is untestable here.
        {
            Input input;

            uint32 panFingers = 0;
            Vec2 panDelta = Vec2::Zero();
            uint32 panCount = 0;
            input.OnPan().Add([&](const PanEvent& event)
                {
                    panFingers = event.fingerCount;
                    panDelta = event.delta;
                    ++panCount;
                });

            float32 pinchScale = 1.0f;
            uint32 pinchCount = 0;
            input.OnPinch().Add([&](const PinchEvent& event) { pinchScale = event.scale; ++pinchCount; });

            // One finger down. The first Update only establishes the baseline.
            input.PostTouch(TouchPhase::Began, 1, Vec2(100.0f, 100.0f), 0.0);
            input.Update();
            Debug::Assert(panCount == 0, "A touch landing emitted a pan before it moved");

            input.PostTouch(TouchPhase::Moved, 1, Vec2(130.0f, 120.0f), 0.016);
            input.Update();
            Debug::Assert(panCount == 1, "A one-finger drag emitted {} pans, expected 1", panCount);
            Debug::Assert(panFingers == 1, "Pan reported {} fingers, expected 1", panFingers);
            Debug::Assert(panDelta == Vec2(30.0f, 20.0f), "Pan delta is ({}, {}), expected (30, 20)", panDelta.x, panDelta.y);

            // A second finger landing mid-drag jumps the centroid. It must not emit a pan --
            // this is the bug that teleports the object the moment you add a finger.
            panCount = 0;
            input.PostTouch(TouchPhase::Began, 2, Vec2(330.0f, 120.0f), 0.032);
            input.Update();
            Debug::Assert(panCount == 0, "A finger landing mid-drag emitted a pan");

            // Two fingers separating: pinch out, and the centroid holds still so no pan.
            panCount = 0;
            input.PostTouch(TouchPhase::Moved, 1, Vec2(80.0f, 120.0f), 0.048);
            input.PostTouch(TouchPhase::Moved, 2, Vec2(380.0f, 120.0f), 0.048);
            input.Update();
            Debug::Assert(pinchCount == 1, "Separating two fingers emitted {} pinches, expected 1", pinchCount);
            Debug::Assert(pinchScale > 1.0f, "Pinch out gave scale {}, expected > 1", pinchScale);
            Debug::Assert(panCount == 0, "A symmetric pinch moved the centroid");

            // Lifting back to one finger rebases again rather than reporting the centroid jump.
            panCount = 0;
            input.PostTouch(TouchPhase::Ended, 2, Vec2(380.0f, 120.0f), 0.064);
            input.Update();
            Debug::Assert(panCount == 0, "A finger lifting emitted a pan");

            // Cancelled must remove the touch too. A call interrupting a gesture never sends
            // Ended, and a finger left in the table holds the gesture open forever.
            input.PostTouch(TouchPhase::Cancelled, 1, Vec2(80.0f, 120.0f), 0.080);
            input.Update();
            input.PostTouch(TouchPhase::Began, 3, Vec2(500.0f, 500.0f), 0.096);
            input.Update();
            input.PostTouch(TouchPhase::Moved, 3, Vec2(510.0f, 500.0f), 0.112);
            panCount = 0;
            input.Update();
            Debug::Assert(panFingers == 1, "A cancelled touch stayed active: pan reports {} fingers", panFingers);
        }

        // Camera, as input drives it.
        {
            Camera camera;
            camera.SetPosition(Vec3(0.0f, 0.0f, 5.0f));
            camera.SetTarget(Vec3::Zero());
            camera.SetFovY(ToRadians(60.0f));

            // Looking down -Z from +Z, the basis is the world's: right is +X, up is +Y.
            // worldPerPixel = 2 * 5 * tan(30 deg) / 1000, so 100 px is 0.5774 world units.
            const Vec3 right = camera.ScreenDeltaToWorld(Vec2(100.0f, 0.0f), 1000.0f);
            Debug::Assert(AreValuesClose(right, Vec3(0.57735f, 0.0f, 0.0f), 1e-4f), "A rightward drag mapped to ({}, {}, {})", right.x, right.y, right.z);

            // Screen Y is down and world Y is up, so the sign flips.
            const Vec3 down = camera.ScreenDeltaToWorld(Vec2(0.0f, 100.0f), 1000.0f);
            Debug::Assert(AreValuesClose(down, Vec3(0.0f, -0.57735f, 0.0f), 1e-4f), "A downward drag mapped to ({}, {}, {})", down.x, down.y, down.z);

            // Twice as far means the same drag covers twice the world, or the object stops
            // tracking the finger as you zoom out.
            camera.SetPosition(Vec3(0.0f, 0.0f, 10.0f));
            const Vec3 farther = camera.ScreenDeltaToWorld(Vec2(100.0f, 0.0f), 1000.0f);
            Debug::Assert(AreValuesClose(farther.x, right.x * 2.0f, 1e-4f), "Doubling the distance scaled the drag by {}", farther.x / right.x);

            // Zoom is multiplicative, so opposite factors cancel.
            camera.Zoom(0.5f);
            const Vec3 halved = camera.ScreenDeltaToWorld(Vec2(100.0f, 0.0f), 1000.0f);
            Debug::Assert(AreValuesClose(halved.x, right.x, 1e-4f), "Zoom(0.5) from 10 units did not land at 5");
            camera.Zoom(2.0f);
            const Vec3 restored = camera.ScreenDeltaToWorld(Vec2(100.0f, 0.0f), 1000.0f);
            Debug::Assert(AreValuesClose(restored.x, farther.x, 1e-4f), "Zoom(2) did not undo Zoom(0.5)");

            // And it clamps, so a fast pinch cannot bury the camera in the target.
            camera.Zoom(0.0001f);
            const Vec3 clamped = camera.ScreenDeltaToWorld(Vec2(100.0f, 0.0f), 1000.0f);
            Debug::Assert(clamped.x > 0.0f, "Zoom collapsed the camera onto its target");
        }

        // The camera basis, which is the axis a one-finger drag rotates about. Off-axis on
        // purpose: the block above looks straight down -Z, where the camera basis equals the
        // world's and a world-axis mistake cannot show.
        {
            Camera camera;
            camera.SetPosition(Vec3(1.6f, 2.0f, 2.4f));
            camera.SetTarget(Vec3::Zero());

            const Vec3 right = camera.GetRight();
            const Vec3 up    = camera.GetUp();

            Debug::Assert(AreValuesClose(right.Length(), 1.0f, 1e-4f), "Camera right is {} long", right.Length());
            Debug::Assert(AreValuesClose(up.Length(), 1.0f, 1e-4f), "Camera up is {} long", up.Length());
            Debug::Assert(AreValuesClose(right.Dot(up), 0.0f, 1e-4f), "Camera right and up are {} apart", right.Dot(up));

            // The premise of the assertion below, so it cannot pass by being vacuous.
            Debug::Assert(!AreValuesClose(right.Dot(Vec3::Right()), 1.0f, 1e-3f), "This camera is on-axis, so it cannot tell world X from screen right");

            // A vertical drag must not move the object sideways on screen. Screen-X of a world
            // vector is its dot with the camera's right, and a rotation leaves the component along
            // its own axis alone -- so pitching about the camera's right holds it at zero. About
            // world X it reaches -0.39 here, which is the tumble.
            const Vec3 tipped = Quat::FromAxisAngle(right, ToRadians(45.0f)).Up();
            Debug::Assert(AreValuesClose(tipped.Dot(right), 0.0f, 1e-4f), "A vertical drag slid the object {} sideways on screen", tipped.Dot(right));

            // And it must still hold after a sequence of drags, which is what rebuilding from two
            // accumulated angles buys. Composing each drag onto the previous rotation instead
            // passes the single-drag case above and then rolls to 0.92 by the sixth -- the pyramid
            // lying on its side -- because two rotations about different axes compose to a twist.
            float32 yaw = 0.0f;
            float32 pitch = 0.0f;
            for (int32 i = 0; i < 6; ++i)
            {
                ((i % 2) == 0 ? pitch : yaw) += ToRadians(30.0f);

                const Quat rotation = Quat::FromAxisAngle(right, pitch) * Quat::FromAxisAngle(Vec3::Up(), yaw);
                const float32 rolled = rotation.Up().Dot(right);
                Debug::Assert(AreValuesClose(rolled, 0.0f, 1e-4f), "The pyramid rolled {} sideways after {} drags", rolled, i + 1);
            }
        }

        // Keyboard movement. Synthetic keystrokes are blocked on this machine, so the mapping is
        // driven through Input directly -- which covers everything but AppKit's delivery.
        {
            Input input;
            Debug::Assert(GetMoveAxis(input) == Vec2::Zero(), "An idle keyboard asked the pyramid to move");

            input.PostKeyDown(KeyCode::D, false);
            Debug::Assert(GetMoveAxis(input) == Vec2(1.0f, 0.0f), "D did not map to +X");
            input.PostKeyUp(KeyCode::D);

            input.PostKeyDown(KeyCode::W, false);
            Debug::Assert(GetMoveAxis(input) == Vec2(0.0f, 1.0f), "W did not map to +Y");

            // Arrows are the same axis, so a held W and a held UpArrow must not stack.
            input.PostKeyDown(KeyCode::UpArrow, false);
            Debug::Assert(GetMoveAxis(input) == Vec2(0.0f, 1.0f), "W and UpArrow stacked into a double-speed press");
            input.PostKeyUp(KeyCode::UpArrow);

            // Opposing keys cancel rather than fighting over the last one pressed.
            input.PostKeyDown(KeyCode::S, false);
            Debug::Assert(GetMoveAxis(input) == Vec2::Zero(), "W and S did not cancel");
            input.PostKeyUp(KeyCode::S);

            // A diagonal is normalized, or holding two keys is 1.41x faster than holding one.
            input.PostKeyDown(KeyCode::A, false);
            const Vec2 diagonal = GetMoveAxis(input);
            Debug::Assert(AreValuesClose(diagonal.Length(), 1.0f, 1e-4f), "A diagonal press moves at {}, expected 1", diagonal.Length());
            Debug::Assert(diagonal.x < 0.0f && diagonal.y > 0.0f, "A+W pointed ({}, {}), expected up and left", diagonal.x, diagonal.y);
        }

        // Name tables. Checked at both ends, because a short table only misbehaves at the end.
        {
            Debug::Assert(std::string_view(ToString(KeyCode::A)) == "A", "KeyCode::A is named {}", ToString(KeyCode::A));
            Debug::Assert(std::string_view(ToString(KeyCode::KeypadClear)) == "KeypadClear", "The last KeyCode is named {}", ToString(KeyCode::KeypadClear));
            Debug::Assert(std::string_view(ToString(KeyCode::LeftSuper)) == "LeftSuper", "KeyCode::LeftSuper is named {}", ToString(KeyCode::LeftSuper));
            Debug::Assert(std::string_view(ToString(MouseButton::Middle)) == "Middle", "MouseButton::Middle is named {}", ToString(MouseButton::Middle));
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
