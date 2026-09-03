// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_Gesture;

import jpt.Constants;
import jpt.GestureRecognizer;
import jpt.Input;
import jpt.InputCodes;
import jpt.InputEvents;
import jpt.Math;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector2;

/** The only coverage gestures can get: there is no iOS Simulator in this project and
    synthetic touches need assistive access, so UIKit's delivery is untestable here. */
export void RunUnitTests_GesturePan(jpt::TestCase& test)
{
    jpt::Input input;

    uint32 panFingers = 0;
    jpt::Vec2 panDelta = jpt::Vec2::Zero();
    uint32 panCount = 0;
    input.OnPan().Add([&](const jpt::PanEvent& event)
        {
            panFingers = event.fingerCount;
            panDelta = event.delta;
            ++panCount;
        });

    float32 pinchScale = 1.0f;
    uint32 pinchCount = 0;
    input.OnPinch().Add([&](const jpt::PinchEvent& event) { pinchScale = event.scale; ++pinchCount; });

    // One finger down. The first Update only establishes the baseline.
    input.PostTouch(jpt::TouchPhase::Began, 1, jpt::Vec2(100.0f, 100.0f), 0.0, jpt::TouchDevice::Direct);
    input.Update();
    test.Expect(panCount == 0, "A touch landing emitted a pan before it moved");

    input.PostTouch(jpt::TouchPhase::Moved, 1, jpt::Vec2(130.0f, 120.0f), 0.016, jpt::TouchDevice::Direct);
    input.Update();
    test.Expect(panCount == 1, "A one-finger drag emitted {} pans, expected 1", panCount);
    test.Expect(panFingers == 1, "Pan reported {} fingers, expected 1", panFingers);
    test.Expect(panDelta == jpt::Vec2(30.0f, 20.0f), "Pan delta is ({}, {}), expected (30, 20)", panDelta.x, panDelta.y);

    // A second finger landing mid-drag jumps the centroid. It must not emit a pan -- this is
    // the bug that teleports the object the moment you add a finger.
    panCount = 0;
    input.PostTouch(jpt::TouchPhase::Began, 2, jpt::Vec2(330.0f, 120.0f), 0.032, jpt::TouchDevice::Direct);
    input.Update();
    test.Expect(panCount == 0, "A finger landing mid-drag emitted a pan");

    // Two fingers separating: pinch out, and the centroid holds still so no pan.
    panCount = 0;
    input.PostTouch(jpt::TouchPhase::Moved, 1, jpt::Vec2(80.0f, 120.0f), 0.048, jpt::TouchDevice::Direct);
    input.PostTouch(jpt::TouchPhase::Moved, 2, jpt::Vec2(380.0f, 120.0f), 0.048, jpt::TouchDevice::Direct);
    input.Update();
    test.Expect(pinchCount == 1, "Separating two fingers emitted {} pinches, expected 1", pinchCount);
    test.Expect(pinchScale > 1.0f, "Pinch out gave scale {}, expected > 1", pinchScale);
    test.Expect(panCount == 0, "A symmetric pinch moved the centroid");

    // Lifting back to one finger rebases again rather than reporting the centroid jump.
    panCount = 0;
    input.PostTouch(jpt::TouchPhase::Ended, 2, jpt::Vec2(380.0f, 120.0f), 0.064, jpt::TouchDevice::Direct);
    input.Update();
    test.Expect(panCount == 0, "A finger lifting emitted a pan");

    // Cancelled must remove the touch too. A call interrupting a gesture never sends Ended,
    // and a finger left in the table holds the gesture open forever.
    input.PostTouch(jpt::TouchPhase::Cancelled, 1, jpt::Vec2(80.0f, 120.0f), 0.080, jpt::TouchDevice::Direct);
    input.Update();
    input.PostTouch(jpt::TouchPhase::Began, 3, jpt::Vec2(500.0f, 500.0f), 0.096, jpt::TouchDevice::Direct);
    input.Update();
    input.PostTouch(jpt::TouchPhase::Moved, 3, jpt::Vec2(510.0f, 500.0f), 0.112, jpt::TouchDevice::Direct);
    panCount = 0;
    input.Update();
    test.Expect(panFingers == 1, "A cancelled touch stayed active: pan reports {} fingers", panFingers);
}

/** One finger on a trackpad is the cursor, so it must not pan -- that is a thumb resting there
    rotating the model. The same finger on a phone screen must, because there it is the only
    pointer the device has. */
export void RunUnitTests_GestureDirectVsIndirect(jpt::TestCase& test)
{
    {
        jpt::Input input;

        uint32 panCount = 0;
        uint32 panFingers = 0;
        input.OnPan().Add([&](const jpt::PanEvent& event) { ++panCount; panFingers = event.fingerCount; });

        input.PostTouch(jpt::TouchPhase::Began, 1, jpt::Vec2(100.0f, 100.0f), 0.0, jpt::TouchDevice::IndirectAbsolute);
        input.Update();
        input.PostTouch(jpt::TouchPhase::Moved, 1, jpt::Vec2(130.0f, 120.0f), 0.016, jpt::TouchDevice::IndirectAbsolute);
        input.Update();
        test.Expect(panCount == 0, "One finger on an indirect device emitted {} pans, expected 0", panCount);

        // A second finger makes it a gesture, and the pan resumes from there.
        input.PostTouch(jpt::TouchPhase::Began, 2, jpt::Vec2(330.0f, 120.0f), 0.032, jpt::TouchDevice::IndirectAbsolute);
        input.Update();
        input.PostTouch(jpt::TouchPhase::Moved, 1, jpt::Vec2(140.0f, 130.0f), 0.048, jpt::TouchDevice::IndirectAbsolute);
        input.PostTouch(jpt::TouchPhase::Moved, 2, jpt::Vec2(340.0f, 130.0f), 0.048, jpt::TouchDevice::IndirectAbsolute);
        input.Update();
        test.Expect(panCount == 1, "Two fingers on an indirect device emitted {} pans, expected 1", panCount);
        test.Expect(panFingers == 2, "Pan reported {} fingers, expected 2", panFingers);
    }
    {
        jpt::Input input;

        uint32 panCount = 0;
        input.OnPan().Add([&panCount](const jpt::PanEvent&) { ++panCount; });

        input.PostTouch(jpt::TouchPhase::Began, 1, jpt::Vec2(100.0f, 100.0f), 0.0, jpt::TouchDevice::Direct);
        input.Update();
        input.PostTouch(jpt::TouchPhase::Moved, 1, jpt::Vec2(130.0f, 120.0f), 0.016, jpt::TouchDevice::Direct);
        input.Update();
        test.Expect(panCount == 1, "One finger on a direct device emitted {} pans, expected 1", panCount);
    }

    // Both Indirect kinds are cursors, so both need two fingers. Only the coordinate mapping
    // distinguishes them, which UnitTests_InputCodes exercises.
    {
        jpt::GestureRecognizer gestures;

        uint32 panCount = 0;
        gestures.OnPan().Add([&panCount](const jpt::PanEvent&) { ++panCount; });

        gestures.PostTouch(jpt::TouchPhase::Began, 1, jpt::Vec2(100.0f, 100.0f), 0.0, jpt::TouchDevice::IndirectRelative);
        gestures.Update();
        gestures.PostTouch(jpt::TouchPhase::Moved, 1, jpt::Vec2(130.0f, 120.0f), 0.016, jpt::TouchDevice::IndirectRelative);
        gestures.Update();
        test.Expect(panCount == 0, "One finger on IndirectRelative emitted {} pans, expected 0", panCount);
    }
}

/** Touch is the pointer on a phone, so a drag on an ImGui panel must not also reach the scene.
    Driven on the recognizer directly: Input::Update() reads the live ImGui state, which a test
    has no context to set. */
export void RunUnitTests_GestureCapture(jpt::TestCase& test)
{
    jpt::GestureRecognizer gestures;

    uint32 panCount = 0;
    jpt::Vec2 panDelta = jpt::Vec2::Zero();
    gestures.OnPan().Add([&](const jpt::PanEvent& event) { ++panCount; panDelta = event.delta; });

    gestures.PostTouch(jpt::TouchPhase::Began, 1, jpt::Vec2(100.0f, 100.0f), 0.0, jpt::TouchDevice::Direct);
    gestures.SetCaptured(true);
    gestures.Update();

    gestures.PostTouch(jpt::TouchPhase::Moved, 1, jpt::Vec2(130.0f, 120.0f), 0.016, jpt::TouchDevice::Direct);
    gestures.Update();
    test.Expect(panCount == 0, "A drag under capture emitted {} pans, expected 0", panCount);

    // Releasing must not deliver the captured excursion as one jump: the baseline followed the
    // finger, so only the movement after release counts.
    gestures.PostTouch(jpt::TouchPhase::Moved, 1, jpt::Vec2(140.0f, 130.0f), 0.032, jpt::TouchDevice::Direct);
    gestures.SetCaptured(false);
    gestures.Update();
    test.Expect(panCount == 1, "Releasing capture emitted {} pans, expected 1", panCount);
    test.Expect(panDelta == jpt::Vec2(10.0f, 10.0f), "Pan after release is ({}, {}), expected (10, 10) -- the baseline went stale under capture", panDelta.x, panDelta.y);
}

/** Rotating about the pair's own midpoint holds the centroid and the spread, so a pure twist
    must reach exactly one of the three recognisers. */
export void RunUnitTests_GestureTwist(jpt::TestCase& test)
{
    jpt::Input input;

    uint32 panCount = 0;
    uint32 pinchCount = 0;
    uint32 twistCount = 0;
    float32 twistRadians = 0.0f;
    input.OnPan().Add([&panCount](const jpt::PanEvent&) { ++panCount; });
    input.OnPinch().Add([&pinchCount](const jpt::PinchEvent&) { ++pinchCount; });
    input.OnTwist().Add([&](const jpt::TwistEvent& event)
        {
            twistRadians = event.radians;
            ++twistCount;
        });

    input.PostTouch(jpt::TouchPhase::Began, 1, jpt::Vec2(100.0f, 100.0f), 0.0, jpt::TouchDevice::Direct);
    input.PostTouch(jpt::TouchPhase::Began, 2, jpt::Vec2(300.0f, 100.0f), 0.0, jpt::TouchDevice::Direct);
    input.Update();

    // A quarter turn clockwise about the midpoint (200, 100). Pixels are y-down, so clockwise
    // on screen is the positive direction.
    input.PostTouch(jpt::TouchPhase::Moved, 1, jpt::Vec2(200.0f,   0.0f), 0.016, jpt::TouchDevice::Direct);
    input.PostTouch(jpt::TouchPhase::Moved, 2, jpt::Vec2(200.0f, 200.0f), 0.016, jpt::TouchDevice::Direct);
    input.Update();
    test.Expect(twistCount == 1, "A quarter turn emitted {} twists, expected 1", twistCount);
    test.Expect(jpt::AreValuesClose(twistRadians, jpt::kHalfPi<float32>, 1e-4f), "A quarter turn reported {} rad", twistRadians);
    test.Expect(panCount == 0, "A twist about the midpoint moved the centroid");
    test.Expect(pinchCount == 0, "A twist about the midpoint changed the spread");

    // Park the pair just short of atan2's +/-pi branch cut, then step 20 degrees across it.
    input.PostTouch(jpt::TouchPhase::Moved, 1, jpt::Vec2(298.481f,  82.635f), 0.032, jpt::TouchDevice::Direct);
    input.PostTouch(jpt::TouchPhase::Moved, 2, jpt::Vec2(101.519f, 117.365f), 0.032, jpt::TouchDevice::Direct);
    input.Update();

    twistCount = 0;
    input.PostTouch(jpt::TouchPhase::Moved, 1, jpt::Vec2(298.481f, 117.365f), 0.048, jpt::TouchDevice::Direct);
    input.PostTouch(jpt::TouchPhase::Moved, 2, jpt::Vec2(101.519f,  82.635f), 0.048, jpt::TouchDevice::Direct);
    input.Update();
    test.Expect(twistCount == 1, "The step across the branch cut emitted {} twists, expected 1", twistCount);
    test.Expect(jpt::AreValuesClose(twistRadians, jpt::ToRadians(20.0f), 1e-3f), "Crossing +/-pi reported {} rad, expected 0.349 -- the delta was not wrapped", twistRadians);

    // Trading one finger for another inside a frame holds the count at 2 while swinging the
    // pair's axis. Rebasing on the count alone would report that swing as a twist.
    panCount = 0;
    pinchCount = 0;
    twistCount = 0;
    input.PostTouch(jpt::TouchPhase::Ended, 1, jpt::Vec2(298.481f, 117.365f), 0.064, jpt::TouchDevice::Direct);
    input.PostTouch(jpt::TouchPhase::Began, 3, jpt::Vec2(101.519f, 500.0f), 0.064, jpt::TouchDevice::Direct);
    input.Update();
    test.Expect(twistCount == 0, "Swapping a finger mid-gesture emitted a twist of {} rad", twistRadians);
    test.Expect(panCount == 0, "Swapping a finger mid-gesture emitted a pan");
    test.Expect(pinchCount == 0, "Swapping a finger mid-gesture emitted a pinch");
}
