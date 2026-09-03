// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_Camera;

import jpt.Camera;
import jpt.Input;
import jpt.InputCodes;
import jpt.Math;
import jpt.Matrix44;
import jpt.Quaternion;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;

/** The camera, as input drives it. */
export void RunUnitTests_CameraScreenDelta(jpt::TestCase& test)
{
    jpt::Camera camera;
    camera.SetPosition(jpt::Vec3(0.0f, 0.0f, 5.0f));
    camera.SetDirection(jpt::Vec3::Forward());
    camera.SetDistance(5.0f);
    camera.SetFovY(jpt::ToRadians(60.0f));

    // Looking down -Z from +Z, the basis is the world's: right is +X, up is +Y.
    // worldPerPixel = 2 * 5 * tan(30 deg) / 1000, so 100 px is 0.5774 world units.
    const jpt::Vec3 right = camera.ScreenDeltaToWorld(jpt::Vec2(100.0f, 0.0f), 1000.0f);
    test.Expect(jpt::AreValuesClose(right, jpt::Vec3(0.57735f, 0.0f, 0.0f), 1e-4f), "A rightward drag mapped to ({}, {}, {})", right.x, right.y, right.z);

    // Screen Y is down and world Y is up, so the sign flips.
    const jpt::Vec3 down = camera.ScreenDeltaToWorld(jpt::Vec2(0.0f, 100.0f), 1000.0f);
    test.Expect(jpt::AreValuesClose(down, jpt::Vec3(0.0f, -0.57735f, 0.0f), 1e-4f), "A downward drag mapped to ({}, {}, {})", down.x, down.y, down.z);

    // Twice as far means the same drag covers twice the world, or the object stops tracking
    // the finger as you zoom out.
    camera.SetDistance(10.0f);
    const jpt::Vec3 farther = camera.ScreenDeltaToWorld(jpt::Vec2(100.0f, 0.0f), 1000.0f);
    test.Expect(jpt::AreValuesClose(farther.x, right.x * 2.0f, 1e-4f), "Doubling the distance scaled the drag by {}", farther.x / right.x);

    // Zoom moves the camera, not what it orbits. Position and distance are separate fields
    // now, so nothing else here would notice the position half of Zoom going wrong.
    const jpt::Vec3 pivotBefore = camera.GetPosition() + camera.Forward() * camera.GetDistance();
    camera.Zoom(0.4f);
    const jpt::Vec3 pivotAfter = camera.GetPosition() + camera.Forward() * camera.GetDistance();
    test.Expect(jpt::AreValuesClose(pivotBefore, pivotAfter, 1e-4f), "Zoom dragged the orbit point from ({}, {}, {}) to ({}, {}, {})", pivotBefore.x, pivotBefore.y, pivotBefore.z, pivotAfter.x, pivotAfter.y, pivotAfter.z);
    camera.Zoom(1.0f / 0.4f);

    // Zoom is multiplicative, so opposite factors cancel.
    camera.Zoom(0.5f);
    const jpt::Vec3 halved = camera.ScreenDeltaToWorld(jpt::Vec2(100.0f, 0.0f), 1000.0f);
    test.Expect(jpt::AreValuesClose(halved.x, right.x, 1e-4f), "Zoom(0.5) from 10 units did not land at 5");
    camera.Zoom(2.0f);
    const jpt::Vec3 restored = camera.ScreenDeltaToWorld(jpt::Vec2(100.0f, 0.0f), 1000.0f);
    test.Expect(jpt::AreValuesClose(restored.x, farther.x, 1e-4f), "Zoom(2) did not undo Zoom(0.5)");

    // And it clamps, so a fast pinch cannot bury the camera in the target.
    camera.Zoom(0.0001f);
    const jpt::Vec3 clamped = camera.ScreenDeltaToWorld(jpt::Vec2(100.0f, 0.0f), 1000.0f);
    test.Expect(clamped.x > 0.0f, "Zoom collapsed the camera onto its target");
}

/** The camera basis, which is the axis a one-finger drag rotates about. Off-axis on purpose:
    ScreenDelta looks straight down -Z, where the camera basis equals the world's and a
    world-axis mistake cannot show. */
export void RunUnitTests_CameraBasis(jpt::TestCase& test)
{
    jpt::Camera camera;
    camera.SetPosition(jpt::Vec3(1.6f, 2.0f, 2.4f));
    camera.SetDirection(-jpt::Vec3(1.6f, 2.0f, 2.4f));

    const jpt::Vec3 right = camera.Right();
    const jpt::Vec3 up    = camera.Up();

    test.Expect(jpt::AreValuesClose(right.Length(), 1.0f, 1e-4f), "Camera right is {} long", right.Length());
    test.Expect(jpt::AreValuesClose(up.Length(), 1.0f, 1e-4f), "Camera up is {} long", up.Length());
    test.Expect(jpt::AreValuesClose(right.Dot(up), 0.0f, 1e-4f), "Camera right and up are {} apart", right.Dot(up));

    // The premise of the assertion below, so it cannot pass by being vacuous.
    test.Expect(!jpt::AreValuesClose(right.Dot(jpt::Vec3::Right()), 1.0f, 1e-3f), "This camera is on-axis, so it cannot tell world X from screen right");

    // A vertical drag must not move the object sideways on screen. Screen-X of a world vector
    // is its dot with the camera's right, and a rotation leaves the component along its own
    // axis alone -- so pitching about the camera's right holds it at zero. About world X it
    // reaches -0.39 here, which is the tumble.
    const jpt::Vec3 tipped = jpt::Quat::FromAxisAngle(right, jpt::ToRadians(45.0f)).Up();
    test.Expect(jpt::AreValuesClose(tipped.Dot(right), 0.0f, 1e-4f), "A vertical drag slid the object {} sideways on screen", tipped.Dot(right));

    // And it must still hold after a sequence of drags, which is what rebuilding from two
    // accumulated angles buys. Composing each drag onto the previous rotation instead passes
    // the single-drag case above and then rolls to 0.92 by the sixth -- the model lying on its
    // side -- because two rotations about different axes compose to a twist.
    float32 yaw = 0.0f;
    float32 pitch = 0.0f;
    for (int32 i = 0; i < 6; ++i)
    {
        ((i % 2) == 0 ? pitch : yaw) += jpt::ToRadians(30.0f);

        const jpt::Quat rotation = jpt::Quat::FromAxisAngle(right, pitch) * jpt::Quat::FromAxisAngle(jpt::Vec3::Up(), yaw);
        const float32 rolled = rotation.Up().Dot(right);
        test.Expect(jpt::AreValuesClose(rolled, 0.0f, 1e-4f), "The model rolled {} sideways after {} drags", rolled, i + 1);
    }

    // A two-finger twist rolls about the axis the camera looks along, so it cannot change how
    // far anything sits along that axis. World +Z is 47 degrees off it here and shifts this by
    // 0.36 -- the same class of mistake as pitching about world X above.
    const jpt::Vec3 forward = camera.Forward();
    test.Expect(!jpt::AreValuesClose(forward.Dot(jpt::Vec3::Forward()), 1.0f, 1e-3f), "This camera looks down -Z, so it cannot tell world Z from the view axis");

    const float32 depthBefore = jpt::Vec3::Up().Dot(forward);
    const float32 depthAfter  = jpt::Quat::FromAxisAngle(forward, jpt::ToRadians(35.0f)).Up().Dot(forward);
    test.Expect(jpt::AreValuesClose(depthAfter, depthBefore, 1e-4f), "A twist moved the model {} along the view axis", depthAfter - depthBefore);
}

/** Local move and rotate, which the Dev Menu drives. */
export void RunUnitTests_CameraLocalMotion(jpt::TestCase& test)
{
    jpt::Camera camera;
    camera.SetPosition(jpt::Vec3::Zero());
    camera.SetDirection(jpt::Vec3::Forward());

    // Yaw 90 deg about world up leaves the camera facing -X, so its own right is -Z.
    camera.RotateLocal(0.0f, jpt::ToRadians(90.0f));
    test.Expect(jpt::AreValuesClose(camera.Forward(), jpt::Vec3(-1.0f, 0.0f, 0.0f), 1e-4f), "Yaw 90 aimed at ({}, {}, {})", camera.Forward().x, camera.Forward().y, camera.Forward().z);

    // Move is along the camera's axes, not the world's: +x is its right, which is now -Z.
    camera.MoveLocal(jpt::Vec3(2.0f, 0.0f, 0.0f));
    test.Expect(jpt::AreValuesClose(camera.GetPosition(), jpt::Vec3(0.0f, 0.0f, -2.0f), 1e-4f), "Local +x moved to ({}, {}, {}), expected (0, 0, -2)", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

    // The horizon must stay level through any mix of pitch and yaw. Composing about two local
    // axes instead would roll, exactly as the model's rotation did.
    for (int32 i = 0; i < 8; ++i)
    {
        camera.RotateLocal(jpt::ToRadians(17.0f), jpt::ToRadians(23.0f));
        test.Expect(jpt::AreValuesClose(camera.Right().y, 0.0f, 1e-4f), "The camera rolled: right.y is {} after {} rotations", camera.Right().y, i + 1);
    }

    // And pitch stops short of vertical rather than tipping over onto its head.
    test.Expect(camera.Up().y > 0.0f, "Pitch passed vertical: up.y is {}", camera.Up().y);

    // LookAt sets aim and orbit distance together, which is the point of it: setting the
    // direction alone would leave Zoom and the drag projection on a stale depth.
    camera.SetPosition(jpt::Vec3(0.0f, 0.0f, 10.0f));
    camera.LookAt(jpt::Vec3(0.0f, 0.0f, 2.0f));
    test.Expect(jpt::AreValuesClose(camera.Forward(), jpt::Vec3::Forward(), 1e-4f), "LookAt aimed at ({}, {}, {})", camera.Forward().x, camera.Forward().y, camera.Forward().z);
    test.Expect(jpt::AreValuesClose(camera.GetDistance(), 8.0f, 1e-4f), "LookAt left the orbit distance at {}, expected 8", camera.GetDistance());

    // Which is to say the orbit point lands exactly on what it was aimed at.
    const jpt::Vec3 orbit = camera.GetPosition() + camera.Forward() * camera.GetDistance();
    test.Expect(jpt::AreValuesClose(orbit, jpt::Vec3(0.0f, 0.0f, 2.0f), 1e-4f), "The orbit point landed at ({}, {}, {})", orbit.x, orbit.y, orbit.z);

    // The view matrix now comes from the rotation's conjugate instead of LookAt. The two must
    // agree wherever LookAt is defined, which is what makes this a refactor.
    camera.SetPosition(jpt::Vec3(3.0f, 4.0f, 5.0f));
    camera.SetDirection(jpt::Vec3(-0.3f, -0.5f, -0.8f));
    const jpt::Mat44 viaLookAt = jpt::Mat44::Perspective(camera.GetFovY(), 1.5f, camera.GetNear(), camera.GetFar()) * jpt::Mat44::LookAt(camera.GetPosition(), camera.GetPosition() + camera.Forward());
    test.Expect(jpt::AreValuesClose(camera.GetViewProjection(1.5f), viaLookAt, 1e-3f), "The quaternion view matrix disagrees with LookAt");
}

/** Synthetic keystrokes are blocked on this machine, so the mapping is driven through Input
    directly -- which covers everything but AppKit's delivery. */
export void RunUnitTests_CameraMoveAxis(jpt::TestCase& test)
{
    jpt::Input input;
    test.Expect(jpt::GetMoveAxis(input) == jpt::Vec3::Zero(), "An idle keyboard asked the camera to move");

    input.PostKeyDown(jpt::KeyCode::D, false);
    test.Expect(jpt::GetMoveAxis(input) == jpt::Vec3(1.0f, 0.0f, 0.0f), "D did not map to the camera's right");
    input.PostKeyUp(jpt::KeyCode::D);

    // Local up, not world up: MoveLocal scales these by the camera's own axes.
    input.PostKeyDown(jpt::KeyCode::E, false);
    test.Expect(jpt::GetMoveAxis(input) == jpt::Vec3(0.0f, 1.0f, 0.0f), "E did not map to the camera's up");
    input.PostKeyUp(jpt::KeyCode::E);

    input.PostKeyDown(jpt::KeyCode::Q, false);
    test.Expect(jpt::GetMoveAxis(input) == jpt::Vec3(0.0f, -1.0f, 0.0f), "Q did not map to the camera's down");
    input.PostKeyUp(jpt::KeyCode::Q);

    input.PostKeyDown(jpt::KeyCode::W, false);
    test.Expect(jpt::GetMoveAxis(input) == jpt::Vec3(0.0f, 0.0f, -1.0f), "W did not map to the camera's forward");

    // Arrows are the same axis, so a held W and a held UpArrow must not stack.
    input.PostKeyDown(jpt::KeyCode::UpArrow, false);
    test.Expect(jpt::GetMoveAxis(input) == jpt::Vec3(0.0f, 0.0f, -1.0f), "W and UpArrow stacked into a double-speed press");
    input.PostKeyUp(jpt::KeyCode::UpArrow);

    // Opposing keys cancel rather than fighting over the last one pressed.
    input.PostKeyDown(jpt::KeyCode::S, false);
    test.Expect(jpt::GetMoveAxis(input) == jpt::Vec3::Zero(), "W and S did not cancel");
    input.PostKeyUp(jpt::KeyCode::S);

    // A diagonal is normalized, or holding two keys is 1.41x faster than holding one.
    input.PostKeyDown(jpt::KeyCode::A, false);
    const jpt::Vec3 diagonal = jpt::GetMoveAxis(input);
    test.Expect(jpt::AreValuesClose(diagonal.Length(), 1.0f, 1e-4f), "A diagonal press moves at {}, expected 1", diagonal.Length());
    test.Expect(diagonal.x < 0.0f && diagonal.z < 0.0f, "A+W pointed ({}, {}, {}), expected forward and left", diagonal.x, diagonal.y, diagonal.z);
}
