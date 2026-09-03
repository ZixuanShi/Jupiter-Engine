// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_Vector2;

import jpt.Math;
import jpt.MathTestUtils;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector2;
import std;

export void RunUnitTests_Vector2(jpt::TestCase& test)
{
    test.Expect(jpt::AreValuesClose(jpt::Vec2::Right().Dot(jpt::Vec2::Up()), 0.0f), "Vector2 basis is not orthogonal");

    test.Expect(jpt::AreValuesClose(jpt::Vec2(3.0f, 4.0f).Length(), 5.0f), "Vector2 length 3-4-5 is wrong");
    test.Expect(jpt::AreClose(jpt::Vec2(3.0f, 4.0f).Normalized(), jpt::Vec2(0.6f, 0.8f)), "Vector2 normalized 3-4-5 is wrong");
    test.Expect(jpt::AreClose(jpt::Vec2(2.0f), jpt::Vec2(2.0f, 2.0f)), "Vector2 scalar ctor does not broadcast");

    // Legacy compared against an exact zero, so this divided by 1e-20 and returned inf.
    const jpt::Vec2 tiny = jpt::Vec2(1e-20f, 0.0f).Normalized();
    test.Expect(std::isfinite(tiny.x) && std::isfinite(tiny.y), "Normalizing a near-zero Vector2 produced inf");
}
