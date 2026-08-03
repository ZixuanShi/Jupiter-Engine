// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Math;

import jpt.Concepts;
import jpt.Constants;

export namespace jpt
{
    template<Floating T>
    [[nodiscard]] constexpr T ToRadians(T degrees) noexcept
    {
        return degrees * (kPi<T> / static_cast<T>(180));
    }

    template<Floating T>
    [[nodiscard]] constexpr T ToDegrees(T radians) noexcept
    {
        return radians * (static_cast<T>(180) / kPi<T>);
    }

    template<Floating T>
    [[nodiscard]] constexpr bool AreValuesClose(T a, T b, T tolerance = kEpsilon<T>) noexcept
    {
        const T difference = a - b;
        return -tolerance <= difference && difference <= tolerance;
    }
}
