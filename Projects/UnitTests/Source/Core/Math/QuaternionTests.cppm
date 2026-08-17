// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.QuaternionTests;

import jpt.Constants;
import jpt.Math;
import jpt.MathTestUtils;
import jpt.Matrix44;
import jpt.Quaternion;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector3;

namespace jpt::local
{
    /** Must agree with Matrix44, which Matrix44Tests already pins. */
    void QuaternionTest(TestCase& test)
    {
        test.Expect(AreClose(Quat::Identity().Right(),   Vec3::Right()),   "Quaternion identity right");
        test.Expect(AreClose(Quat::Identity().Up(),      Vec3::Up()),      "Quaternion identity up");
        test.Expect(AreClose(Quat::Identity().Forward(), Vec3::Forward()), "Quaternion identity forward");

        // Legacy's Right() and Up() carried the opposite w signs -- the transpose, which is the
        // inverse rotation -- and its Forward() was +Z. This is where that shows.
        const Quat yaw = Quat::FromAxisAngle(Vec3::Up(), kHalfPi<float32>);
        test.Expect(AreClose(yaw.Right(), Vec3::Forward()), "Quaternion right after 90 deg yaw");
        test.Expect(AreClose(yaw.ToMatrix(), Mat44::RotateY(kHalfPi<float32>)), "Quaternion does not match RotateY");

        // Off-axis on all three, so an XYZ-order mismatch between the two paths cannot cancel.
        const Vec3 euler(ToRadians(20.0f), ToRadians(35.0f), ToRadians(50.0f));
        test.Expect(AreClose(Quat::FromEulerAngles(euler).ToMatrix(), Mat44::FromEulerAngles(euler)),
                    "Quaternion and Matrix44 Euler conventions disagree");

        // A sign error in operator* stays invisible until two rotations are chained.
        const Quat a = Quat::FromAxisAngle(Vec3::Up(), ToRadians(40.0f));
        const Quat b = Quat::FromAxisAngle(Vec3::Right(), ToRadians(25.0f));
        test.Expect(AreClose((a * b).ToMatrix(), a.ToMatrix() * b.ToMatrix()), "Quaternion product order is wrong");

        test.Expect(AreClose(a * a.Inverse(), Quat::Identity()), "q * q.Inverse() is not identity");

        test.Expect(AreClose(Quat::Slerp(a, b, 0.0f), a), "Slerp at t=0 is not the start");
        test.Expect(AreClose(Quat::Slerp(a, b, 1.0f), b), "Slerp at t=1 is not the end");
        test.Expect(AreValuesClose(Quat::Slerp(a, b, 0.5f).Length(), 1.0f), "Slerp does not stay unit length");
    }

    static TestCase s_quaternion("Math.Quaternion", &QuaternionTest);
}
