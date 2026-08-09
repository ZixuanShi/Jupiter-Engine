// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_MACOS
    #include <Carbon/Carbon.h>

    #include "Applications/GetApp.h"
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
import jpt.Matrix44;
import jpt.Quaternion;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;
import std;

namespace jpt::local
{
    void TestListener::OnEvent(const TestEvent&)
    {
        ++count;
    }
}

namespace jpt
{
    void RunInputTests()
    {
        // EventDispatcher -- registration
        {
            local::TestDispatcher dispatcher;

            int32 firstCount = 0;
            int32 secondCount = 0;

            const local::TestDispatcher::Handle first = dispatcher.Add([&firstCount](const local::TestEvent&) { ++firstCount; });
            const local::TestDispatcher::Handle second = dispatcher.Add([&secondCount](const local::TestEvent&) { ++secondCount; });

            Debug::Assert(first != second, "Add returned the same handle twice");
            Debug::Assert(first != kInvalid<local::TestDispatcher::Handle>, "Add returned the invalid handle");
            Debug::Assert(dispatcher.GetCount() == 2, "Dispatcher holds {} handlers, expected 2", dispatcher.GetCount());

            dispatcher.Dispatch(local::TestEvent{});
            Debug::Assert(firstCount == 1 && secondCount == 1, "Dispatch reached {} and {}, expected 1 and 1", firstCount, secondCount);

            // Remove must stop exactly one handler.
            dispatcher.Remove(first);
            dispatcher.Dispatch(local::TestEvent{});
            Debug::Assert(firstCount == 1, "Removed handler still fired, count is {}", firstCount);
            Debug::Assert(secondCount == 2, "Surviving handler stopped firing, count is {}", secondCount);

            // Removing an unknown handle is a no-op, not a crash or a silent extra tombstone.
            dispatcher.Remove(first);
            Debug::Assert(dispatcher.GetCount() == 1, "Repeated Remove changed the count to {}", dispatcher.GetCount());
        }

        // EventDispatcher -- member function binding
        {
            local::TestDispatcher dispatcher;
            local::TestListener listener;

            const local::TestDispatcher::Handle handle = dispatcher.Add(&listener, &local::TestListener::OnEvent);
            dispatcher.Dispatch(local::TestEvent{});
            Debug::Assert(listener.count == 1, "Member handler fired {} times, expected 1", listener.count);

            dispatcher.Remove(handle);
            dispatcher.Dispatch(local::TestEvent{});
            Debug::Assert(listener.count == 1, "Removed member handler still fired, count is {}", listener.count);
        }

        // EventDispatcher -- re-entrancy. Legacy's Send was undefined behaviour here.
        {
            local::TestDispatcher dispatcher;

            // Removing itself destroys the std::function it is executing in, unless Remove defers.
            int32 selfCount = 0;
            local::TestDispatcher::Handle self = kInvalid<local::TestDispatcher::Handle>;
            self = dispatcher.Add([&dispatcher, &selfCount, &self](const local::TestEvent&)
                {
                    ++selfCount;
                    dispatcher.Remove(self);
                });

            dispatcher.Dispatch(local::TestEvent{});
            dispatcher.Dispatch(local::TestEvent{});
            Debug::Assert(selfCount == 1, "Self-removing handler fired {} times, expected 1", selfCount);
        }
        {
            local::TestDispatcher dispatcher;

            // Adding grows the container; the new handler must not see the in-flight event.
            int32 addedCount = 0;
            int32 adderCount = 0;
            dispatcher.Add([&dispatcher, &addedCount, &adderCount](const local::TestEvent&)
                {
                    ++adderCount;
                    if (adderCount == 1)
                    {
                        dispatcher.Add([&addedCount](const local::TestEvent&) { ++addedCount; });
                    }
                });

            dispatcher.Dispatch(local::TestEvent{});
            Debug::Assert(addedCount == 0, "Handler added mid-dispatch received the in-flight event");

            dispatcher.Dispatch(local::TestEvent{});
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

        // Twist. Rotating about the pair's own midpoint holds the centroid and the spread, so a
        // pure twist must reach exactly one of the three recognisers.
        {
            Input input;

            uint32 panCount = 0;
            uint32 pinchCount = 0;
            uint32 twistCount = 0;
            float32 twistRadians = 0.0f;
            input.OnPan().Add([&panCount](const PanEvent&) { ++panCount; });
            input.OnPinch().Add([&pinchCount](const PinchEvent&) { ++pinchCount; });
            input.OnTwist().Add([&](const TwistEvent& event)
                {
                    twistRadians = event.radians;
                    ++twistCount;
                });

            input.PostTouch(TouchPhase::Began, 1, Vec2(100.0f, 100.0f), 0.0);
            input.PostTouch(TouchPhase::Began, 2, Vec2(300.0f, 100.0f), 0.0);
            input.Update();

            // A quarter turn clockwise about the midpoint (200, 100). Pixels are y-down, so
            // clockwise on screen is the positive direction.
            input.PostTouch(TouchPhase::Moved, 1, Vec2(200.0f,   0.0f), 0.016);
            input.PostTouch(TouchPhase::Moved, 2, Vec2(200.0f, 200.0f), 0.016);
            input.Update();
            Debug::Assert(twistCount == 1, "A quarter turn emitted {} twists, expected 1", twistCount);
            Debug::Assert(AreValuesClose(twistRadians, kHalfPi<float32>, 1e-4f), "A quarter turn reported {} rad", twistRadians);
            Debug::Assert(panCount == 0, "A twist about the midpoint moved the centroid");
            Debug::Assert(pinchCount == 0, "A twist about the midpoint changed the spread");

            // Park the pair just short of atan2's +/-pi branch cut, then step 20 degrees across it.
            input.PostTouch(TouchPhase::Moved, 1, Vec2(298.481f,  82.635f), 0.032);
            input.PostTouch(TouchPhase::Moved, 2, Vec2(101.519f, 117.365f), 0.032);
            input.Update();

            twistCount = 0;
            input.PostTouch(TouchPhase::Moved, 1, Vec2(298.481f, 117.365f), 0.048);
            input.PostTouch(TouchPhase::Moved, 2, Vec2(101.519f,  82.635f), 0.048);
            input.Update();
            Debug::Assert(twistCount == 1, "The step across the branch cut emitted {} twists, expected 1", twistCount);
            Debug::Assert(AreValuesClose(twistRadians, ToRadians(20.0f), 1e-3f), "Crossing +/-pi reported {} rad, expected 0.349 -- the delta was not wrapped", twistRadians);

            // Trading one finger for another inside a frame holds the count at 2 while swinging
            // the pair's axis. Rebasing on the count alone would report that swing as a twist.
            panCount = 0;
            pinchCount = 0;
            twistCount = 0;
            input.PostTouch(TouchPhase::Ended, 1, Vec2(298.481f, 117.365f), 0.064);
            input.PostTouch(TouchPhase::Began, 3, Vec2(101.519f, 500.0f), 0.064);
            input.Update();
            Debug::Assert(twistCount == 0, "Swapping a finger mid-gesture emitted a twist of {} rad", twistRadians);
            Debug::Assert(panCount == 0, "Swapping a finger mid-gesture emitted a pan");
            Debug::Assert(pinchCount == 0, "Swapping a finger mid-gesture emitted a pinch");
        }

        // Camera, as input drives it.
        {
            Camera camera;
            camera.SetPosition(Vec3(0.0f, 0.0f, 5.0f));
            camera.SetDirection(Vec3::Forward());
            camera.SetDistance(5.0f);
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
            camera.SetDistance(10.0f);
            const Vec3 farther = camera.ScreenDeltaToWorld(Vec2(100.0f, 0.0f), 1000.0f);
            Debug::Assert(AreValuesClose(farther.x, right.x * 2.0f, 1e-4f), "Doubling the distance scaled the drag by {}", farther.x / right.x);

            // Zoom moves the camera, not what it orbits. Position and distance are separate fields
            // now, so nothing else here would notice the position half of Zoom going wrong.
            const Vec3 pivotBefore = camera.GetPosition() + camera.Forward() * camera.GetDistance();
            camera.Zoom(0.4f);
            const Vec3 pivotAfter = camera.GetPosition() + camera.Forward() * camera.GetDistance();
            Debug::Assert(AreValuesClose(pivotBefore, pivotAfter, 1e-4f), "Zoom dragged the orbit point from ({}, {}, {}) to ({}, {}, {})", pivotBefore.x, pivotBefore.y, pivotBefore.z, pivotAfter.x, pivotAfter.y, pivotAfter.z);
            camera.Zoom(1.0f / 0.4f);

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
            camera.SetDirection(-Vec3(1.6f, 2.0f, 2.4f));

            const Vec3 right = camera.Right();
            const Vec3 up    = camera.Up();

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
            // passes the single-drag case above and then rolls to 0.92 by the sixth -- the model
            // lying on its side -- because two rotations about different axes compose to a twist.
            float32 yaw = 0.0f;
            float32 pitch = 0.0f;
            for (int32 i = 0; i < 6; ++i)
            {
                ((i % 2) == 0 ? pitch : yaw) += ToRadians(30.0f);

                const Quat rotation = Quat::FromAxisAngle(right, pitch) * Quat::FromAxisAngle(Vec3::Up(), yaw);
                const float32 rolled = rotation.Up().Dot(right);
                Debug::Assert(AreValuesClose(rolled, 0.0f, 1e-4f), "The model rolled {} sideways after {} drags", rolled, i + 1);
            }

            // A two-finger twist rolls about the axis the camera looks along, so it cannot change
            // how far anything sits along that axis. World +Z is 47 degrees off it here and shifts
            // this by 0.36 -- the same class of mistake as pitching about world X above.
            const Vec3 forward = camera.Forward();
            Debug::Assert(!AreValuesClose(forward.Dot(Vec3::Forward()), 1.0f, 1e-3f), "This camera looks down -Z, so it cannot tell world Z from the view axis");

            const float32 depthBefore = Vec3::Up().Dot(forward);
            const float32 depthAfter  = Quat::FromAxisAngle(forward, ToRadians(35.0f)).Up().Dot(forward);
            Debug::Assert(AreValuesClose(depthAfter, depthBefore, 1e-4f), "A twist moved the model {} along the view axis", depthAfter - depthBefore);
        }

        // Capture. The platform seam reports everything now, so this is the only gate, and the
        // case that matters is focus changing *between* a press and its release.
        {
            Input input;

            uint32 downCount = 0;
            uint32 upCount = 0;
            input.OnKeyDown().Add([&downCount](const KeyEvent&) { ++downCount; });
            input.OnKeyUp().Add([&upCount](const KeyEvent&) { ++upCount; });

            input.PostKeyDown(KeyCode::W, false);
            Debug::Assert(input.IsKeyDown(KeyCode::W) && downCount == 1, "An uncaptured press did not reach the game");

            // Focus moves to a panel mid-press: the game must stop seeing the key as held.
            input.SetCaptured(true, false);
            Debug::Assert(!input.IsKeyDown(KeyCode::W), "A captured keyboard still reported W held");

            // Released while captured. Nothing is dispatched, but the state must still track it.
            input.PostKeyUp(KeyCode::W);
            Debug::Assert(upCount == 0, "A captured release was dispatched to the game");
            input.SetCaptured(false, false);
            Debug::Assert(!input.IsKeyDown(KeyCode::W), "W stuck down: the release was lost while captured");

            // Typing while captured must not leak in either.
            input.SetCaptured(true, false);
            input.PostKeyDown(KeyCode::A, false);
            Debug::Assert(downCount == 1, "A captured press was dispatched to the game");
            Debug::Assert(!input.IsKeyDown(KeyCode::A), "A captured press was readable by the game");

            // Still physically held when focus returns, so it must read as held again.
            input.SetCaptured(false, false);
            Debug::Assert(input.IsKeyDown(KeyCode::A), "A key held across a capture cycle was forgotten");
            input.PostKeyUp(KeyCode::A);

            // Buttons are the same story, on their own flag.
            input.PostMouseButton(MouseButton::Left, true, Vec2(10.0f, 10.0f));
            input.SetCaptured(false, true);
            Debug::Assert(!input.IsMouseButtonDown(MouseButton::Left), "A captured mouse still reported Left held");
            input.PostMouseButton(MouseButton::Left, false, Vec2(90.0f, 90.0f));
            input.SetCaptured(false, false);
            Debug::Assert(!input.IsMouseButtonDown(MouseButton::Left), "Left stuck down: the release was lost while captured");

            // Position tracks through capture, or crossing the panel and back arrives as one jump.
            Vec2 movedDelta = Vec2::Zero();
            input.OnMouseMove().Add([&movedDelta](const MouseMoveEvent& event) { movedDelta = event.delta; });
            input.SetCaptured(false, true);
            input.PostMouseMove(Vec2(500.0f, 90.0f));
            input.SetCaptured(false, false);
            input.PostMouseMove(Vec2(510.0f, 90.0f));
            Debug::Assert(movedDelta == Vec2(10.0f, 0.0f), "Crossing a panel delivered a ({}, {}) jump", movedDelta.x, movedDelta.y);
        }

        // Local move and rotate, which the Dev Menu drives.
        {
            Camera camera;
            camera.SetPosition(Vec3::Zero());
            camera.SetDirection(Vec3::Forward());

            // Yaw 90 deg about world up leaves the camera facing -X, so its own right is -Z.
            camera.RotateLocal(0.0f, ToRadians(90.0f));
            Debug::Assert(AreValuesClose(camera.Forward(), Vec3(-1.0f, 0.0f, 0.0f), 1e-4f), "Yaw 90 aimed at ({}, {}, {})", camera.Forward().x, camera.Forward().y, camera.Forward().z);

            // Move is along the camera's axes, not the world's: +x is its right, which is now -Z.
            camera.MoveLocal(Vec3(2.0f, 0.0f, 0.0f));
            Debug::Assert(AreValuesClose(camera.GetPosition(), Vec3(0.0f, 0.0f, -2.0f), 1e-4f), "Local +x moved to ({}, {}, {}), expected (0, 0, -2)", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

            // The horizon must stay level through any mix of pitch and yaw. Composing about two
            // local axes instead would roll, exactly as the model's rotation did.
            for (int32 i = 0; i < 8; ++i)
            {
                camera.RotateLocal(ToRadians(17.0f), ToRadians(23.0f));
                Debug::Assert(AreValuesClose(camera.Right().y, 0.0f, 1e-4f), "The camera rolled: right.y is {} after {} rotations", camera.Right().y, i + 1);
            }

            // And pitch stops short of vertical rather than tipping over onto its head.
            Debug::Assert(camera.Up().y > 0.0f, "Pitch passed vertical: up.y is {}", camera.Up().y);

            // LookAt sets aim and orbit distance together, which is the point of it: setting the
            // direction alone would leave Zoom and the drag projection on a stale depth.
            camera.SetPosition(Vec3(0.0f, 0.0f, 10.0f));
            camera.LookAt(Vec3(0.0f, 0.0f, 2.0f));
            Debug::Assert(AreValuesClose(camera.Forward(), Vec3::Forward(), 1e-4f), "LookAt aimed at ({}, {}, {})", camera.Forward().x, camera.Forward().y, camera.Forward().z);
            Debug::Assert(AreValuesClose(camera.GetDistance(), 8.0f, 1e-4f), "LookAt left the orbit distance at {}, expected 8", camera.GetDistance());

            // Which is to say the orbit point lands exactly on what it was aimed at.
            const Vec3 orbit = camera.GetPosition() + camera.Forward() * camera.GetDistance();
            Debug::Assert(AreValuesClose(orbit, Vec3(0.0f, 0.0f, 2.0f), 1e-4f), "The orbit point landed at ({}, {}, {})", orbit.x, orbit.y, orbit.z);

            // The view matrix now comes from the rotation's conjugate instead of LookAt. The two
            // must agree wherever LookAt is defined, which is what makes this a refactor.
            camera.SetPosition(Vec3(3.0f, 4.0f, 5.0f));
            camera.SetDirection(Vec3(-0.3f, -0.5f, -0.8f));
            const Mat44 viaLookAt = Mat44::Perspective(camera.GetFovY(), 1.5f, camera.GetNear(), camera.GetFar()) * Mat44::LookAt(camera.GetPosition(), camera.GetPosition() + camera.Forward());
            Debug::Assert(AreValuesClose(camera.GetViewProjection(1.5f), viaLookAt, 1e-3f), "The quaternion view matrix disagrees with LookAt");
        }

        // Keyboard movement. Synthetic keystrokes are blocked on this machine, so the mapping is
        // driven through Input directly -- which covers everything but AppKit's delivery.
        {
            Input input;
            Debug::Assert(GetMoveAxis(input) == Vec3::Zero(), "An idle keyboard asked the camera to move");

            input.PostKeyDown(KeyCode::D, false);
            Debug::Assert(GetMoveAxis(input) == Vec3(1.0f, 0.0f, 0.0f), "D did not map to the camera's right");
            input.PostKeyUp(KeyCode::D);

            // Local up, not world up: MoveLocal scales these by the camera's own axes.
            input.PostKeyDown(KeyCode::E, false);
            Debug::Assert(GetMoveAxis(input) == Vec3(0.0f, 1.0f, 0.0f), "E did not map to the camera's up");
            input.PostKeyUp(KeyCode::E);

            input.PostKeyDown(KeyCode::Q, false);
            Debug::Assert(GetMoveAxis(input) == Vec3(0.0f, -1.0f, 0.0f), "Q did not map to the camera's down");
            input.PostKeyUp(KeyCode::Q);

            input.PostKeyDown(KeyCode::W, false);
            Debug::Assert(GetMoveAxis(input) == Vec3(0.0f, 0.0f, -1.0f), "W did not map to the camera's forward");

            // Arrows are the same axis, so a held W and a held UpArrow must not stack.
            input.PostKeyDown(KeyCode::UpArrow, false);
            Debug::Assert(GetMoveAxis(input) == Vec3(0.0f, 0.0f, -1.0f), "W and UpArrow stacked into a double-speed press");
            input.PostKeyUp(KeyCode::UpArrow);

            // Opposing keys cancel rather than fighting over the last one pressed.
            input.PostKeyDown(KeyCode::S, false);
            Debug::Assert(GetMoveAxis(input) == Vec3::Zero(), "W and S did not cancel");
            input.PostKeyUp(KeyCode::S);

            // A diagonal is normalized, or holding two keys is 1.41x faster than holding one.
            input.PostKeyDown(KeyCode::A, false);
            const Vec3 diagonal = GetMoveAxis(input);
            Debug::Assert(AreValuesClose(diagonal.Length(), 1.0f, 1e-4f), "A diagonal press moves at {}, expected 1", diagonal.Length());
            Debug::Assert(diagonal.x < 0.0f && diagonal.z < 0.0f, "A+W pointed ({}, {}, {}), expected forward and left", diagonal.x, diagonal.y, diagonal.z);
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
            Input& input = GetApp().GetInput();

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

            // Button numbers through the same real callback. Middle is the one that drives
            // translation, and AppKit numbers it 2 -- nothing else in the engine says so.
            MouseButton receivedButton = MouseButton::Left;
            const auto buttonHandle = input.OnMouseButton().Add([&receivedButton](const MouseButtonEvent& event) { receivedButton = event.button; });

            const struct { std::int32_t number; MouseButton expected; } kButtons[] =
            {
                { 0, MouseButton::Left },
                { 1, MouseButton::Right },
                { 2, MouseButton::Middle },
            };

            for (const auto& testCase : kButtons)
            {
                // Position stays at the origin it already holds, so this leaves no phantom delta
                // behind for the first real mouse move.
                jpt::OnMouseButton(testCase.number, true, 0.0f, 0.0f);
                Debug::Assert(receivedButton == testCase.expected, "Button {} translated to {}, expected {}", testCase.number, ToString(receivedButton), ToString(testCase.expected));
                Debug::Assert(input.IsMouseButtonDown(testCase.expected), "Button {} did not register as down", testCase.number);

                jpt::OnMouseButton(testCase.number, false, 0.0f, 0.0f);
                Debug::Assert(!input.IsMouseButtonDown(testCase.expected), "Button {} stayed down after release", testCase.number);
            }

            input.OnMouseButton().Remove(buttonHandle);
        }
        {
            // Modifiers arrive as a whole mask, never down/up, so this path translates separately.
            const Input& input = GetApp().GetInput();

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
