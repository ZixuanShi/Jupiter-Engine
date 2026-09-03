// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_InputState;

import jpt.Input;
import jpt.InputCodes;
import jpt.InputEvents;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector2;

/** A local Input, so the dev logger is not attached. */
export void RunUnitTests_KeyAndMouse(jpt::TestCase& test)
{
    jpt::Input input;

    jpt::KeyCode receivedKey = jpt::KeyCode::Unknown;
    bool receivedRepeat = false;
    input.OnKeyDown().Add([&receivedKey, &receivedRepeat](const jpt::KeyEvent& event)
        {
            receivedKey = event.key;
            receivedRepeat = event.isRepeat;
        });

    test.Expect(!input.IsKeyDown(jpt::KeyCode::W), "A fresh Input reports W already down");

    input.PostKeyDown(jpt::KeyCode::W, false);
    test.Expect(input.IsKeyDown(jpt::KeyCode::W), "PostKeyDown did not set the down state");
    test.Expect(receivedKey == jpt::KeyCode::W, "OnKeyDown delivered {}, expected W", jpt::ToString(receivedKey));
    test.Expect(!receivedRepeat, "isRepeat was set on a first press");

    input.PostKeyDown(jpt::KeyCode::W, true);
    test.Expect(receivedRepeat, "isRepeat did not survive the trip through the event");
    test.Expect(input.IsKeyDown(jpt::KeyCode::W), "A repeat cleared the down state");

    input.PostKeyUp(jpt::KeyCode::W);
    test.Expect(!input.IsKeyDown(jpt::KeyCode::W), "PostKeyUp did not clear the down state");

    // Modifiers are ordinary keys, which is what lets a mouse handler ask about them.
    input.PostKeyDown(jpt::KeyCode::LeftShift, false);
    test.Expect(input.IsKeyDown(jpt::KeyCode::LeftShift), "Modifiers are not tracked in the down-set");
    input.PostKeyUp(jpt::KeyCode::LeftShift);

    jpt::Vec2 receivedPosition = jpt::Vec2::Zero();
    input.OnMouseButton().Add([&receivedPosition](const jpt::MouseButtonEvent& event) { receivedPosition = event.position; });

    input.PostMouseButton(jpt::MouseButton::Left, true, jpt::Vec2(12.0f, 34.0f));
    test.Expect(input.IsMouseButtonDown(jpt::MouseButton::Left), "PostMouseButton did not set the down state");
    test.Expect(receivedPosition == jpt::Vec2(12.0f, 34.0f), "Mouse position did not survive the event");
    test.Expect(input.GetMousePosition() == jpt::Vec2(12.0f, 34.0f), "A button press did not update the cached position");

    input.PostMouseButton(jpt::MouseButton::Left, false, jpt::Vec2(12.0f, 34.0f));
    test.Expect(!input.IsMouseButtonDown(jpt::MouseButton::Left), "Releasing did not clear the down state");

    jpt::Vec2 receivedDelta = jpt::Vec2::Zero();
    input.OnMouseMove().Add([&receivedDelta](const jpt::MouseMoveEvent& event) { receivedDelta = event.delta; });

    input.PostMouseMove(jpt::Vec2(20.0f, 30.0f));
    test.Expect(receivedDelta == jpt::Vec2(8.0f, -4.0f), "Mouse delta is ({}, {}), expected (8, -4)", receivedDelta.x, receivedDelta.y);
}

/** The platform seam reports everything now, so capture is the only gate, and the case that
    matters is focus changing *between* a press and its release. */
export void RunUnitTests_Capture(jpt::TestCase& test)
{
    jpt::Input input;

    uint32 downCount = 0;
    uint32 upCount = 0;
    input.OnKeyDown().Add([&downCount](const jpt::KeyEvent&) { ++downCount; });
    input.OnKeyUp().Add([&upCount](const jpt::KeyEvent&) { ++upCount; });

    input.PostKeyDown(jpt::KeyCode::W, false);
    test.Expect(input.IsKeyDown(jpt::KeyCode::W) && downCount == 1, "An uncaptured press did not reach the game");

    // Focus moves to a panel mid-press: the game must stop seeing the key as held.
    input.SetCaptured(true, false);
    test.Expect(!input.IsKeyDown(jpt::KeyCode::W), "A captured keyboard still reported W held");

    // Released while captured. Nothing is dispatched, but the state must still track it.
    input.PostKeyUp(jpt::KeyCode::W);
    test.Expect(upCount == 0, "A captured release was dispatched to the game");
    input.SetCaptured(false, false);
    test.Expect(!input.IsKeyDown(jpt::KeyCode::W), "W stuck down: the release was lost while captured");

    // Typing while captured must not leak in either.
    input.SetCaptured(true, false);
    input.PostKeyDown(jpt::KeyCode::A, false);
    test.Expect(downCount == 1, "A captured press was dispatched to the game");
    test.Expect(!input.IsKeyDown(jpt::KeyCode::A), "A captured press was readable by the game");

    // Still physically held when focus returns, so it must read as held again.
    input.SetCaptured(false, false);
    test.Expect(input.IsKeyDown(jpt::KeyCode::A), "A key held across a capture cycle was forgotten");
    input.PostKeyUp(jpt::KeyCode::A);

    // Buttons are the same story, on their own flag.
    input.PostMouseButton(jpt::MouseButton::Left, true, jpt::Vec2(10.0f, 10.0f));
    input.SetCaptured(false, true);
    test.Expect(!input.IsMouseButtonDown(jpt::MouseButton::Left), "A captured mouse still reported Left held");
    input.PostMouseButton(jpt::MouseButton::Left, false, jpt::Vec2(90.0f, 90.0f));
    input.SetCaptured(false, false);
    test.Expect(!input.IsMouseButtonDown(jpt::MouseButton::Left), "Left stuck down: the release was lost while captured");

    // Position tracks through capture, or crossing the panel and back arrives as one jump.
    jpt::Vec2 movedDelta = jpt::Vec2::Zero();
    input.OnMouseMove().Add([&movedDelta](const jpt::MouseMoveEvent& event) { movedDelta = event.delta; });
    input.SetCaptured(false, true);
    input.PostMouseMove(jpt::Vec2(500.0f, 90.0f));
    input.SetCaptured(false, false);
    input.PostMouseMove(jpt::Vec2(510.0f, 90.0f));
    test.Expect(movedDelta == jpt::Vec2(10.0f, 0.0f), "Crossing a panel delivered a ({}, {}) jump", movedDelta.x, movedDelta.y);
}
