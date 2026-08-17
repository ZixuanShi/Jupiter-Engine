// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.InputStateTests;

import jpt.Input;
import jpt.InputCodes;
import jpt.InputEvents;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector2;

namespace jpt::local
{
    /** A local Input, so the dev logger is not attached. */
    void KeyAndMouseTest(TestCase& test)
    {
        Input input;

        KeyCode receivedKey = KeyCode::Unknown;
        bool receivedRepeat = false;
        input.OnKeyDown().Add([&receivedKey, &receivedRepeat](const KeyEvent& event)
            {
                receivedKey = event.key;
                receivedRepeat = event.isRepeat;
            });

        test.Expect(!input.IsKeyDown(KeyCode::W), "A fresh Input reports W already down");

        input.PostKeyDown(KeyCode::W, false);
        test.Expect(input.IsKeyDown(KeyCode::W), "PostKeyDown did not set the down state");
        test.Expect(receivedKey == KeyCode::W, "OnKeyDown delivered {}, expected W", ToString(receivedKey));
        test.Expect(!receivedRepeat, "isRepeat was set on a first press");

        input.PostKeyDown(KeyCode::W, true);
        test.Expect(receivedRepeat, "isRepeat did not survive the trip through the event");
        test.Expect(input.IsKeyDown(KeyCode::W), "A repeat cleared the down state");

        input.PostKeyUp(KeyCode::W);
        test.Expect(!input.IsKeyDown(KeyCode::W), "PostKeyUp did not clear the down state");

        // Modifiers are ordinary keys, which is what lets a mouse handler ask about them.
        input.PostKeyDown(KeyCode::LeftShift, false);
        test.Expect(input.IsKeyDown(KeyCode::LeftShift), "Modifiers are not tracked in the down-set");
        input.PostKeyUp(KeyCode::LeftShift);

        Vec2 receivedPosition = Vec2::Zero();
        input.OnMouseButton().Add([&receivedPosition](const MouseButtonEvent& event) { receivedPosition = event.position; });

        input.PostMouseButton(MouseButton::Left, true, Vec2(12.0f, 34.0f));
        test.Expect(input.IsMouseButtonDown(MouseButton::Left), "PostMouseButton did not set the down state");
        test.Expect(receivedPosition == Vec2(12.0f, 34.0f), "Mouse position did not survive the event");
        test.Expect(input.GetMousePosition() == Vec2(12.0f, 34.0f), "A button press did not update the cached position");

        input.PostMouseButton(MouseButton::Left, false, Vec2(12.0f, 34.0f));
        test.Expect(!input.IsMouseButtonDown(MouseButton::Left), "Releasing did not clear the down state");

        Vec2 receivedDelta = Vec2::Zero();
        input.OnMouseMove().Add([&receivedDelta](const MouseMoveEvent& event) { receivedDelta = event.delta; });

        input.PostMouseMove(Vec2(20.0f, 30.0f));
        test.Expect(receivedDelta == Vec2(8.0f, -4.0f), "Mouse delta is ({}, {}), expected (8, -4)", receivedDelta.x, receivedDelta.y);
    }

    /** The platform seam reports everything now, so capture is the only gate, and the case that
        matters is focus changing *between* a press and its release. */
    void CaptureTest(TestCase& test)
    {
        Input input;

        uint32 downCount = 0;
        uint32 upCount = 0;
        input.OnKeyDown().Add([&downCount](const KeyEvent&) { ++downCount; });
        input.OnKeyUp().Add([&upCount](const KeyEvent&) { ++upCount; });

        input.PostKeyDown(KeyCode::W, false);
        test.Expect(input.IsKeyDown(KeyCode::W) && downCount == 1, "An uncaptured press did not reach the game");

        // Focus moves to a panel mid-press: the game must stop seeing the key as held.
        input.SetCaptured(true, false);
        test.Expect(!input.IsKeyDown(KeyCode::W), "A captured keyboard still reported W held");

        // Released while captured. Nothing is dispatched, but the state must still track it.
        input.PostKeyUp(KeyCode::W);
        test.Expect(upCount == 0, "A captured release was dispatched to the game");
        input.SetCaptured(false, false);
        test.Expect(!input.IsKeyDown(KeyCode::W), "W stuck down: the release was lost while captured");

        // Typing while captured must not leak in either.
        input.SetCaptured(true, false);
        input.PostKeyDown(KeyCode::A, false);
        test.Expect(downCount == 1, "A captured press was dispatched to the game");
        test.Expect(!input.IsKeyDown(KeyCode::A), "A captured press was readable by the game");

        // Still physically held when focus returns, so it must read as held again.
        input.SetCaptured(false, false);
        test.Expect(input.IsKeyDown(KeyCode::A), "A key held across a capture cycle was forgotten");
        input.PostKeyUp(KeyCode::A);

        // Buttons are the same story, on their own flag.
        input.PostMouseButton(MouseButton::Left, true, Vec2(10.0f, 10.0f));
        input.SetCaptured(false, true);
        test.Expect(!input.IsMouseButtonDown(MouseButton::Left), "A captured mouse still reported Left held");
        input.PostMouseButton(MouseButton::Left, false, Vec2(90.0f, 90.0f));
        input.SetCaptured(false, false);
        test.Expect(!input.IsMouseButtonDown(MouseButton::Left), "Left stuck down: the release was lost while captured");

        // Position tracks through capture, or crossing the panel and back arrives as one jump.
        Vec2 movedDelta = Vec2::Zero();
        input.OnMouseMove().Add([&movedDelta](const MouseMoveEvent& event) { movedDelta = event.delta; });
        input.SetCaptured(false, true);
        input.PostMouseMove(Vec2(500.0f, 90.0f));
        input.SetCaptured(false, false);
        input.PostMouseMove(Vec2(510.0f, 90.0f));
        test.Expect(movedDelta == Vec2(10.0f, 0.0f), "Crossing a panel delivered a ({}, {}) jump", movedDelta.x, movedDelta.y);
    }

    static TestCase s_keyAndMouse("Input.KeyAndMouse", &KeyAndMouseTest);
    static TestCase s_capture("Input.Capture", &CaptureTest);
}
