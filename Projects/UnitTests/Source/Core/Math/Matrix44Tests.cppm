// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Matrix44Tests;

import jpt.Constants;
import jpt.Math;
import jpt.MathTestUtils;
import jpt.Matrix44;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector3;

namespace jpt::local
{
    void TransformTest(TestCase& test)
    {
        const Vec3 translated = Mat44::Translate(Vec3(1, 2, 3)) * Vec3(0, 0, 0);
        test.Expect(AreClose(translated, Vec3(1, 2, 3)), "Translate is wrong");

        const Vec3 scaled = Mat44::Scale(Vec3(2, 3, 4)) * Vec3(1, 1, 1);
        test.Expect(AreClose(scaled, Vec3(2, 3, 4)), "Scale is wrong");

        test.Expect(AreClose(Mat44::Translate(Vec3(1, 2, 3)).GetTranslation(), Vec3(1, 2, 3)),
                    "GetTranslation reads the wrong column");

        const Mat44 m = Mat44::Translate(Vec3(1, 2, 3)) * Mat44::Scale(Vec3(2, 2, 2));
        test.Expect(Mat44::Transpose(Mat44::Transpose(m)) == m, "Transpose is not an involution");
        test.Expect(AreClose(Mat44::Inverse(m) * m, Mat44::Identity()), "Inverse(M) * M is not identity");
    }

    /** Right-handed rotations. A positive angle about an axis carries the next basis vector in
        the cyclic order X -> Y -> Z(backward) -> X. */
    void RotationTest(TestCase& test)
    {
        constexpr float32 quarter = kHalfPi<float32>;

        test.Expect(AreClose(Mat44::RotateX(quarter) * Vec3::Up(), Vec3::Backward()), "RotateX is not right-handed");
        test.Expect(AreClose(Mat44::RotateY(quarter) * Vec3::Backward(), Vec3::Right()), "RotateY is not right-handed");
        test.Expect(AreClose(Mat44::RotateZ(quarter) * Vec3::Right(), Vec3::Up()), "RotateZ is not right-handed");

        // Rotating about an axis leaves that axis fixed.
        test.Expect(AreClose(Mat44::RotateX(quarter) * Vec3::Right(), Vec3::Right()), "RotateX moved the X axis");
        test.Expect(AreClose(Mat44::RotateY(quarter) * Vec3::Up(), Vec3::Up()), "RotateY moved the Y axis");

        // A full turn is the identity, and rotations preserve length.
        test.Expect(AreClose(Mat44::RotateY(kTwoPi<float32>), Mat44::Identity()), "RotateY(2pi) is not identity");
        test.Expect(AreValuesClose((Mat44::RotateZ(0.7f) * Vec3(1, 2, 3)).Length(),
                                    Vec3(1, 2, 3).Length()), "Rotation does not preserve length");

        // Composition order: X * Y * Z means Z is applied to the vector first.
        const Vec3 euler(0.3f, -0.5f, 0.8f);
        const Mat44 composed = Mat44::RotateX(euler.x) * Mat44::RotateY(euler.y) * Mat44::RotateZ(euler.z);
        test.Expect(AreClose(Mat44::FromEulerAngles(euler), composed), "FromEulerAngles order is wrong");

        const Mat44 a = Mat44::RotateX(0.4f);
        const Mat44 b = Mat44::Translate(Vec3(5, 0, 0));
        const Vec3 v(1, 2, 3);
        test.Expect(AreClose((a * b) * v, a * (b * v)), "Matrix product is not associative with M * v");
    }

    static TestCase s_transform("Math.Matrix44Transform", &TransformTest);
    static TestCase s_rotation("Math.Matrix44Rotation", &RotationTest);
}
