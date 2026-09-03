// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_Vector4;

import jpt.Math;
import jpt.MathTestUtils;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector3;
import jpt.Vector4;

export void RunUnitTests_Vector4(jpt::TestCase& test)
{
    // Legacy dropped w here, which silently broke any 4-component dot product.
    test.Expect(jpt::AreValuesClose(jpt::Vec4(1, 2, 3, 4).Dot(jpt::Vec4(1, 1, 1, 1)), 10.0f), "Vector4::Dot ignores w");
    test.Expect(jpt::AreClose(jpt::Vec4(2.0f), jpt::Vec4(2, 2, 2, 2)), "Vector4 scalar ctor does not broadcast");
    test.Expect(jpt::AreClose(jpt::Vec4(jpt::Vec3(1, 2, 3), 4.0f).XYZ(), jpt::Vec3(1, 2, 3)), "Vector4::XYZ is wrong");
}
