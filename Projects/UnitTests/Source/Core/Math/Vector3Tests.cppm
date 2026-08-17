// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Vector3Tests;

import jpt.Math;
import jpt.MathTestUtils;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector3;

namespace jpt::local
{
    /** The basis must be right-handed, X cross Y = Z. */
    void Vector3Test(TestCase& test)
    {
        test.Expect(AreClose(Vec3::Right().Cross(Vec3::Up()), Vec3::Backward()), "Right x Up != Backward");
        test.Expect(AreClose(Vec3::Up().Cross(Vec3::Backward()), Vec3::Right()), "Up x Backward != Right");
        test.Expect(AreClose(Vec3::Backward().Cross(Vec3::Right()), Vec3::Up()), "Backward x Right != Up");

        // Anti-commutative, so a reversed pair flips sign.
        test.Expect(AreClose(Vec3::Up().Cross(Vec3::Right()), Vec3::Forward()), "Up x Right != Forward");

        test.Expect(AreClose(Vec3::Forward(), -Vec3::Backward()), "Forward != -Backward");

        // Holds for objects, not only cameras -- and why "+X right, +Y up, +Z forward" cannot
        // be adopted: facing +Z puts right at -X.
        test.Expect(AreClose(Vec3::Forward().Cross(Vec3::Up()), Vec3::Right()), "Forward x Up != Right");
        test.Expect(AreClose(Vec3::Backward().Cross(Vec3::Up()), Vec3::Left()), "Backward x Up != Left");
        test.Expect(AreValuesClose(Vec3::Right().Dot(Vec3::Up()), 0.0f), "Basis is not orthogonal");

        const Vec3 normalized = Vec3(3.0f, 0.0f, 4.0f).Normalized();
        test.Expect(AreClose(normalized, Vec3(0.6f, 0.0f, 0.8f)), "Normalized 3-4-5 is wrong");
        test.Expect(AreValuesClose(Vec3(3.0f, 0.0f, 4.0f).Length(), 5.0f), "Length 3-4-5 is wrong");
    }

    static TestCase s_vector3("Math.Vector3", &Vector3Test);
}
