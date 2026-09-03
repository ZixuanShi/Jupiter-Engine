// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_Quaternion;

import jpt.Constants;
import jpt.Math;
import jpt.MathTestUtils;
import jpt.Matrix44;
import jpt.Quaternion;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector3;

/** Must agree with Matrix44, which UnitTests_Matrix44 already pins. */
export void RunUnitTests_Quaternion(jpt::TestCase& test)
{
    test.Expect(jpt::AreClose(jpt::Quat::Identity().Right(),   jpt::Vec3::Right()),   "Quaternion identity right");
    test.Expect(jpt::AreClose(jpt::Quat::Identity().Up(),      jpt::Vec3::Up()),      "Quaternion identity up");
    test.Expect(jpt::AreClose(jpt::Quat::Identity().Forward(), jpt::Vec3::Forward()), "Quaternion identity forward");

    // Legacy's Right() and Up() carried the opposite w signs -- the transpose, which is the
    // inverse rotation -- and its Forward() was +Z. This is where that shows.
    const jpt::Quat yaw = jpt::Quat::FromAxisAngle(jpt::Vec3::Up(), jpt::kHalfPi<float32>);
    test.Expect(jpt::AreClose(yaw.Right(), jpt::Vec3::Forward()), "Quaternion right after 90 deg yaw");
    test.Expect(jpt::AreClose(yaw.ToMatrix(), jpt::Mat44::RotateY(jpt::kHalfPi<float32>)), "Quaternion does not match RotateY");

    // Off-axis on all three, so an XYZ-order mismatch between the two paths cannot cancel.
    const jpt::Vec3 euler(jpt::ToRadians(20.0f), jpt::ToRadians(35.0f), jpt::ToRadians(50.0f));
    test.Expect(jpt::AreClose(jpt::Quat::FromEulerAngles(euler).ToMatrix(), jpt::Mat44::FromEulerAngles(euler)),
                "Quaternion and Matrix44 Euler conventions disagree");

    // A sign error in operator* stays invisible until two rotations are chained.
    const jpt::Quat a = jpt::Quat::FromAxisAngle(jpt::Vec3::Up(), jpt::ToRadians(40.0f));
    const jpt::Quat b = jpt::Quat::FromAxisAngle(jpt::Vec3::Right(), jpt::ToRadians(25.0f));
    test.Expect(jpt::AreClose((a * b).ToMatrix(), a.ToMatrix() * b.ToMatrix()), "Quaternion product order is wrong");

    test.Expect(jpt::AreClose(a * a.Inverse(), jpt::Quat::Identity()), "q * q.Inverse() is not identity");

    test.Expect(jpt::AreClose(jpt::Quat::Slerp(a, b, 0.0f), a), "Slerp at t=0 is not the start");
    test.Expect(jpt::AreClose(jpt::Quat::Slerp(a, b, 1.0f), b), "Slerp at t=1 is not the end");
    test.Expect(jpt::AreValuesClose(jpt::Quat::Slerp(a, b, 0.5f).Length(), 1.0f), "Slerp does not stay unit length");
}
