// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_Vector3;

import jpt.Math;
import jpt.MathTestUtils;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector3;

/** The basis must be right-handed, X cross Y = Z. */
export void RunUnitTests_Vector3(jpt::TestCase& test)
{
    test.Expect(jpt::AreClose(jpt::Vec3::Right().Cross(jpt::Vec3::Up()), jpt::Vec3::Backward()), "Right x Up != Backward");
    test.Expect(jpt::AreClose(jpt::Vec3::Up().Cross(jpt::Vec3::Backward()), jpt::Vec3::Right()), "Up x Backward != Right");
    test.Expect(jpt::AreClose(jpt::Vec3::Backward().Cross(jpt::Vec3::Right()), jpt::Vec3::Up()), "Backward x Right != Up");

    // Anti-commutative, so a reversed pair flips sign.
    test.Expect(jpt::AreClose(jpt::Vec3::Up().Cross(jpt::Vec3::Right()), jpt::Vec3::Forward()), "Up x Right != Forward");

    test.Expect(jpt::AreClose(jpt::Vec3::Forward(), -jpt::Vec3::Backward()), "Forward != -Backward");

    // Holds for objects, not only cameras -- and why "+X right, +Y up, +Z forward" cannot
    // be adopted: facing +Z puts right at -X.
    test.Expect(jpt::AreClose(jpt::Vec3::Forward().Cross(jpt::Vec3::Up()), jpt::Vec3::Right()), "Forward x Up != Right");
    test.Expect(jpt::AreClose(jpt::Vec3::Backward().Cross(jpt::Vec3::Up()), jpt::Vec3::Left()), "Backward x Up != Left");
    test.Expect(jpt::AreValuesClose(jpt::Vec3::Right().Dot(jpt::Vec3::Up()), 0.0f), "Basis is not orthogonal");

    const jpt::Vec3 normalized = jpt::Vec3(3.0f, 0.0f, 4.0f).Normalized();
    test.Expect(jpt::AreClose(normalized, jpt::Vec3(0.6f, 0.0f, 0.8f)), "Normalized 3-4-5 is wrong");
    test.Expect(jpt::AreValuesClose(jpt::Vec3(3.0f, 0.0f, 4.0f).Length(), 5.0f), "Length 3-4-5 is wrong");
}
