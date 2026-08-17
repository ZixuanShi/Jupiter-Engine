// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Vector2Tests;

import jpt.Math;
import jpt.MathTestUtils;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector2;
import std;

namespace jpt::local
{
    void Vector2Test(TestCase& test)
    {
        test.Expect(AreValuesClose(Vec2::Right().Dot(Vec2::Up()), 0.0f), "Vector2 basis is not orthogonal");

        test.Expect(AreValuesClose(Vec2(3.0f, 4.0f).Length(), 5.0f), "Vector2 length 3-4-5 is wrong");
        test.Expect(AreClose(Vec2(3.0f, 4.0f).Normalized(), Vec2(0.6f, 0.8f)), "Vector2 normalized 3-4-5 is wrong");
        test.Expect(AreClose(Vec2(2.0f), Vec2(2.0f, 2.0f)), "Vector2 scalar ctor does not broadcast");

        // Legacy compared against an exact zero, so this divided by 1e-20 and returned inf.
        const Vec2 tiny = Vec2(1e-20f, 0.0f).Normalized();
        test.Expect(std::isfinite(tiny.x) && std::isfinite(tiny.y), "Normalizing a near-zero Vector2 produced inf");
    }

    static TestCase s_vector2("Math.Vector2", &Vector2Test);
}
