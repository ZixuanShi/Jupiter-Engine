// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_Matrix44;

import jpt.Constants;
import jpt.Math;
import jpt.MathTestUtils;
import jpt.Matrix44;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector3;

export void RunUnitTests_Matrix44Transform(jpt::TestCase& test)
{
    const jpt::Vec3 translated = jpt::Mat44::Translate(jpt::Vec3(1, 2, 3)) * jpt::Vec3(0, 0, 0);
    test.Expect(jpt::AreClose(translated, jpt::Vec3(1, 2, 3)), "Translate is wrong");

    const jpt::Vec3 scaled = jpt::Mat44::Scale(jpt::Vec3(2, 3, 4)) * jpt::Vec3(1, 1, 1);
    test.Expect(jpt::AreClose(scaled, jpt::Vec3(2, 3, 4)), "Scale is wrong");

    test.Expect(jpt::AreClose(jpt::Mat44::Translate(jpt::Vec3(1, 2, 3)).GetTranslation(), jpt::Vec3(1, 2, 3)),
                "GetTranslation reads the wrong column");

    const jpt::Mat44 m = jpt::Mat44::Translate(jpt::Vec3(1, 2, 3)) * jpt::Mat44::Scale(jpt::Vec3(2, 2, 2));
    test.Expect(jpt::Mat44::Transpose(jpt::Mat44::Transpose(m)) == m, "Transpose is not an involution");
    test.Expect(jpt::AreClose(jpt::Mat44::Inverse(m) * m, jpt::Mat44::Identity()), "Inverse(M) * M is not identity");
}

/** Right-handed rotations. A positive angle about an axis carries the next basis vector in
    the cyclic order X -> Y -> Z(backward) -> X. */
export void RunUnitTests_Matrix44Rotation(jpt::TestCase& test)
{
    constexpr float32 quarter = jpt::kHalfPi<float32>;

    test.Expect(jpt::AreClose(jpt::Mat44::RotateX(quarter) * jpt::Vec3::Up(), jpt::Vec3::Backward()), "RotateX is not right-handed");
    test.Expect(jpt::AreClose(jpt::Mat44::RotateY(quarter) * jpt::Vec3::Backward(), jpt::Vec3::Right()), "RotateY is not right-handed");
    test.Expect(jpt::AreClose(jpt::Mat44::RotateZ(quarter) * jpt::Vec3::Right(), jpt::Vec3::Up()), "RotateZ is not right-handed");

    // Rotating about an axis leaves that axis fixed.
    test.Expect(jpt::AreClose(jpt::Mat44::RotateX(quarter) * jpt::Vec3::Right(), jpt::Vec3::Right()), "RotateX moved the X axis");
    test.Expect(jpt::AreClose(jpt::Mat44::RotateY(quarter) * jpt::Vec3::Up(), jpt::Vec3::Up()), "RotateY moved the Y axis");

    // A full turn is the identity, and rotations preserve length.
    test.Expect(jpt::AreClose(jpt::Mat44::RotateY(jpt::kTwoPi<float32>), jpt::Mat44::Identity()), "RotateY(2pi) is not identity");
    test.Expect(jpt::AreValuesClose((jpt::Mat44::RotateZ(0.7f) * jpt::Vec3(1, 2, 3)).Length(),
                                     jpt::Vec3(1, 2, 3).Length()), "Rotation does not preserve length");

    // Composition order: X * Y * Z means Z is applied to the vector first.
    const jpt::Vec3 euler(0.3f, -0.5f, 0.8f);
    const jpt::Mat44 composed = jpt::Mat44::RotateX(euler.x) * jpt::Mat44::RotateY(euler.y) * jpt::Mat44::RotateZ(euler.z);
    test.Expect(jpt::AreClose(jpt::Mat44::FromEulerAngles(euler), composed), "FromEulerAngles order is wrong");

    const jpt::Mat44 a = jpt::Mat44::RotateX(0.4f);
    const jpt::Mat44 b = jpt::Mat44::Translate(jpt::Vec3(5, 0, 0));
    const jpt::Vec3 v(1, 2, 3);
    test.Expect(jpt::AreClose((a * b) * v, a * (b * v)), "Matrix product is not associative with M * v");
}
