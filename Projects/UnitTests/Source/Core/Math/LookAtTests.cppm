// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.LookAtTests;

import jpt.Math;
import jpt.MathTestUtils;
import jpt.Matrix44;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector3;

namespace jpt::local
{
    /** Right-handed, so view space looks down -Z. */
    void LookAtTest(TestCase& test)
    {
        const Vec3 eye(0, 0, 5);
        const Mat44 view = Mat44::LookAt(eye, Vec3(0, 0, 0));

        // The rotation rows are the camera basis expressed in world space.
        const Vec3 right  (view.m[0].x, view.m[1].x, view.m[2].x);
        const Vec3 up     (view.m[0].y, view.m[1].y, view.m[2].y);
        const Vec3 back   (view.m[0].z, view.m[1].z, view.m[2].z);

        // The assertion legacy never had: its Forward() was +Z while LookAt was right-handed,
        // so this came out (-1, 0, 0) and call sites negated to compensate.
        test.Expect(AreClose(right, Vec3::Right()), "LookAt right axis is ({}, {}, {})", right.x, right.y, right.z);
        test.Expect(AreClose(up, Vec3::Up()), "LookAt up axis is ({}, {}, {})", up.x, up.y, up.z);
        test.Expect(AreClose(back, Vec3::Backward()), "LookAt third row is not the backward axis");

        // The target lands in front of the camera, which is negative Z in view space.
        const Vec3 target = view * Vec3(0, 0, 0);
        test.Expect(AreClose(target, Vec3(0, 0, -5)), "LookAt puts the target at ({}, {}, {})", target.x, target.y, target.z);

        // A point beyond the eye lands behind it, so positive Z.
        test.Expect((view * Vec3(0, 0, 10)).z > 0.0f, "LookAt does not place points behind the camera at +Z");

        // The camera itself maps to the view-space origin.
        test.Expect(AreClose(view * eye, Vec3(0, 0, 0)), "LookAt does not map the eye to the origin");

        // Forward parallel to up: the basis loses rank without going NaN, and the translation
        // column stays correct, so only unit length catches it.
        const Mat44 topDown = Mat44::LookAt(Vec3(0, 3, 0), Vec3::Zero());

        const Vec3 downRight(topDown.m[0].x, topDown.m[1].x, topDown.m[2].x);
        const Vec3 downUp   (topDown.m[0].y, topDown.m[1].y, topDown.m[2].y);
        const Vec3 downBack (topDown.m[0].z, topDown.m[1].z, topDown.m[2].z);

        test.Expect(AreValuesClose(downRight.Length(), 1.0f), "LookAt straight down: right axis has length {}", downRight.Length());
        test.Expect(AreValuesClose(downUp.Length(), 1.0f), "LookAt straight down: up axis has length {}", downUp.Length());
        test.Expect(AreValuesClose(downBack.Length(), 1.0f), "LookAt straight down: back axis has length {}", downBack.Length());
        test.Expect(AreValuesClose(downRight.Dot(downUp), 0.0f), "LookAt straight down: basis is not orthogonal");
    }

    static TestCase s_lookAt("Math.LookAt", &LookAtTest);
}
