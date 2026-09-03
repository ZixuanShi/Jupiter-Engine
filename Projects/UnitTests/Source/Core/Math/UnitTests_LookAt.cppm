// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_LookAt;

import jpt.Math;
import jpt.MathTestUtils;
import jpt.Matrix44;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector3;

/** Right-handed, so view space looks down -Z. */
export void RunUnitTests_LookAt(jpt::TestCase& test)
{
    const jpt::Vec3 eye(0, 0, 5);
    const jpt::Mat44 view = jpt::Mat44::LookAt(eye, jpt::Vec3(0, 0, 0));

    // The rotation rows are the camera basis expressed in world space.
    const jpt::Vec3 right  (view.m[0].x, view.m[1].x, view.m[2].x);
    const jpt::Vec3 up     (view.m[0].y, view.m[1].y, view.m[2].y);
    const jpt::Vec3 back   (view.m[0].z, view.m[1].z, view.m[2].z);

    // The assertion legacy never had: its Forward() was +Z while LookAt was right-handed,
    // so this came out (-1, 0, 0) and call sites negated to compensate.
    test.Expect(jpt::AreClose(right, jpt::Vec3::Right()), "LookAt right axis is ({}, {}, {})", right.x, right.y, right.z);
    test.Expect(jpt::AreClose(up, jpt::Vec3::Up()), "LookAt up axis is ({}, {}, {})", up.x, up.y, up.z);
    test.Expect(jpt::AreClose(back, jpt::Vec3::Backward()), "LookAt third row is not the backward axis");

    // The target lands in front of the camera, which is negative Z in view space.
    const jpt::Vec3 target = view * jpt::Vec3(0, 0, 0);
    test.Expect(jpt::AreClose(target, jpt::Vec3(0, 0, -5)), "LookAt puts the target at ({}, {}, {})", target.x, target.y, target.z);

    // A point beyond the eye lands behind it, so positive Z.
    test.Expect((view * jpt::Vec3(0, 0, 10)).z > 0.0f, "LookAt does not place points behind the camera at +Z");

    // The camera itself maps to the view-space origin.
    test.Expect(jpt::AreClose(view * eye, jpt::Vec3(0, 0, 0)), "LookAt does not map the eye to the origin");

    // Forward parallel to up: the basis loses rank without going NaN, and the translation
    // column stays correct, so only unit length catches it.
    const jpt::Mat44 topDown = jpt::Mat44::LookAt(jpt::Vec3(0, 3, 0), jpt::Vec3::Zero());

    const jpt::Vec3 downRight(topDown.m[0].x, topDown.m[1].x, topDown.m[2].x);
    const jpt::Vec3 downUp   (topDown.m[0].y, topDown.m[1].y, topDown.m[2].y);
    const jpt::Vec3 downBack (topDown.m[0].z, topDown.m[1].z, topDown.m[2].z);

    test.Expect(jpt::AreValuesClose(downRight.Length(), 1.0f), "LookAt straight down: right axis has length {}", downRight.Length());
    test.Expect(jpt::AreValuesClose(downUp.Length(), 1.0f), "LookAt straight down: up axis has length {}", downUp.Length());
    test.Expect(jpt::AreValuesClose(downBack.Length(), 1.0f), "LookAt straight down: back axis has length {}", downBack.Length());
    test.Expect(jpt::AreValuesClose(downRight.Dot(downUp), 0.0f), "LookAt straight down: basis is not orthogonal");
}
