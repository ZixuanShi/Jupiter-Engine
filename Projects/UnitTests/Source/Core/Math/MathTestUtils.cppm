// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.MathTestUtils;

import jpt.Math;
import jpt.Matrix44;
import jpt.Quaternion;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;
import jpt.Vector4;

export namespace jpt
{
    // Looser than kEpsilon: these compare against hand-written expectations.
    constexpr float32 kTestTolerance = 1e-4f;

    /** Imports every math module rather than leaving the overload to the caller: AreValuesClose
        is picked by ADL at instantiation, so each overload has to be reachable from here. */
    template<typename T>
    [[nodiscard]] constexpr bool AreClose(const T& a, const T& b) noexcept
    {
        return AreValuesClose(a, b, kTestTolerance);
    }
}
