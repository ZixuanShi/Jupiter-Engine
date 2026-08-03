// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Math;

import jpt.Concepts;
import jpt.Constants;
import std;

export namespace jpt
{
    // Deliberately not constexpr: the std counterparts only become constexpr in C++26. Legacy
    // marked these constexpr anyway, which compiles solely because a template body is never
    // forced into constant evaluation.
    template<Floating T> T Sin(T radians)  { return std::sin(radians);  }
    template<Floating T> T Cos(T radians)  { return std::cos(radians);  }
    template<Floating T> T Tan(T radians)  { return std::tan(radians);  }
    template<Floating T> T Sqrt(T value)   { return std::sqrt(value);   }

    template<Floating T> constexpr T ToRadians(T degrees) { return degrees * (kPi<T> / static_cast<T>(180)); }
    template<Floating T> constexpr T ToDegrees(T radians) { return radians * (static_cast<T>(180) / kPi<T>); }

    template<Numeric T>
    constexpr T Abs(T value)
    {
        if constexpr (std::is_unsigned_v<T>)
        {
            return value;
        }
        else
        {
            return value < static_cast<T>(0) ? -value : value;
        }
    }

    template<Numeric T> constexpr T Min(T a, T b) { return a < b ? a : b; }
    template<Numeric T> constexpr T Max(T a, T b) { return a > b ? a : b; }

    template<Numeric T> constexpr T Clamp(T value, T low, T high) { return Min(Max(value, low), high); }
    template<Numeric T> constexpr T Lerp(T from, T to, T ratio)   { return from + (to - from) * ratio; }

    template<Floating T>
    constexpr bool AreValuesClose(T a, T b, T tolerance = kEpsilon<T>)
    {
        return Abs(a - b) <= tolerance;
    }
}
