// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_Transform;

import jpt.Constants;
import jpt.MathTestUtils;
import jpt.Matrix44;
import jpt.Quaternion;
import jpt.TestFramework;
import jpt.Transform;
import jpt.TypeDefs;
import jpt.Vector3;

export void RunUnitTests_Transform(jpt::TestCase& test)
{
    test.Expect(jpt::AreClose(jpt::Transform{}.ToMatrix(), jpt::Mat44::Identity()), "Transform identity");

    // Pins all three orderings at once: +X scales to (2,0,0), yaws to (0,0,-2), translates
    // to (0,0,3). Swapping any pair moves the result.
    const jpt::Transform transform{ .position = jpt::Vec3(0.0f, 0.0f, 5.0f),
                                    .rotation = jpt::Quat::FromAxisAngle(jpt::Vec3::Up(), jpt::kHalfPi<float32>),
                                    .scale    = jpt::Vec3(2.0f) };
    test.Expect(jpt::AreClose(transform.ToMatrix() * jpt::Vec3::Right(), jpt::Vec3(0.0f, 0.0f, 3.0f)),
                "Transform does not compose scale, then rotation, then translation");
}
