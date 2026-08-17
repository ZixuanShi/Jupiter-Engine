// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Vector4Tests;

import jpt.Math;
import jpt.MathTestUtils;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector3;
import jpt.Vector4;

namespace jpt::local
{
    void Vector4Test(TestCase& test)
    {
        // Legacy dropped w here, which silently broke any 4-component dot product.
        test.Expect(AreValuesClose(Vec4(1, 2, 3, 4).Dot(Vec4(1, 1, 1, 1)), 10.0f), "Vector4::Dot ignores w");
        test.Expect(AreClose(Vec4(2.0f), Vec4(2, 2, 2, 2)), "Vector4 scalar ctor does not broadcast");
        test.Expect(AreClose(Vec4(Vec3(1, 2, 3), 4.0f).XYZ(), Vec3(1, 2, 3)), "Vector4::XYZ is wrong");
    }

    static TestCase s_vector4("Math.Vector4", &Vector4Test);
}
