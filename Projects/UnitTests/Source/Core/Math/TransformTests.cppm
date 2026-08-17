// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.TransformTests;

import jpt.Constants;
import jpt.MathTestUtils;
import jpt.Matrix44;
import jpt.Quaternion;
import jpt.TestFramework;
import jpt.Transform;
import jpt.TypeDefs;
import jpt.Vector3;

namespace jpt::local
{
    void TransformTest(TestCase& test)
    {
        test.Expect(AreClose(Transform{}.ToMatrix(), Mat44::Identity()), "Transform identity");

        // Pins all three orderings at once: +X scales to (2,0,0), yaws to (0,0,-2), translates
        // to (0,0,3). Swapping any pair moves the result.
        const Transform transform{ .position = Vec3(0.0f, 0.0f, 5.0f),
                                   .rotation = Quat::FromAxisAngle(Vec3::Up(), kHalfPi<float32>),
                                   .scale    = Vec3(2.0f) };
        test.Expect(AreClose(transform.ToMatrix() * Vec3::Right(), Vec3(0.0f, 0.0f, 3.0f)),
                    "Transform does not compose scale, then rotation, then translation");
    }

    static TestCase s_transform("Math.Transform", &TransformTest);
}
