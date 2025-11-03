// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Utilities.h"

#include <cmath>

export module jpt.Math;

import jpt.Concepts;
import jpt.Constants;
import jpt.TypeDefs;
import jpt.Utilities;

export namespace jpt
{
#pragma region MinMax

    template<typename TFirst, typename... TRest>
    constexpr TFirst Min(TFirst first, TRest... inputs)
    {
        TFirst smallestVal = first;

        ([&]
            {
                if (smallestVal > inputs)
                {
                    smallestVal = inputs;
                }
            } (), ...);

        return smallestVal;
    }

    template <typename TFirst, typename... TRest>
    constexpr TFirst Max(TFirst first, TRest... inputs)
    {
        TFirst largestVal = first;

        ([&]
            {
                if (largestVal < inputs)
                {
                    largestVal = inputs;
                }
            } (), ...);

        return largestVal;
    }

#pragma endregion MinMax

#pragma region Geometry
    template<Floating TFloat = float32>
    constexpr TFloat ToDegrees(TFloat radians)
    {
        return radians * static_cast<TFloat>(180) / kPi<TFloat>;
    }

    template<typename T> requires (!Floating<T>)
    constexpr T ToDegrees(const T& obj)
    {
        return obj * static_cast<T::NumericType>(180) / kPi<T::NumericType>;
    }

    template<Floating TFloat = float32>
    constexpr TFloat ToRadians(TFloat degrees)
    {
        return degrees * kPi<TFloat> / static_cast<TFloat>(180);
    }

    template<typename T> requires (!Floating<T>)
    constexpr T ToRadians(const T& obj)
    {
        return obj * kPi<T::NumericType> / static_cast<T::NumericType>(180);
    }

    template<Floating TFloat = float32>
    constexpr TFloat Sin(TFloat value)
    {
        return std::sin(value);
    }

    template<Floating TFloat = float32>
    constexpr TFloat Cos(TFloat value)
    {
        return std::cos(value);
    }

    template<Floating TFloat = float32>
    constexpr TFloat Tan(TFloat value)
    {
        return std::tan(value);
    }

    template<Floating TFloat = float32>
    constexpr TFloat Atan2(TFloat lhs, TFloat rhs)
    {
        return std::atan2(lhs, rhs);
    }

    template<Floating TFloat = float32>
    constexpr TFloat Asin(TFloat value)
    {
        return std::asin(value);
    }

    template<Floating TFloat = float32>
    constexpr TFloat Acos(TFloat value)
    {
        return std::acos(value);
    }

#pragma endregion Geometry

#pragma region Clamping
    /** @return Clamped value. Ensure it's at least bigger than min and smaller than max. Inclusive */
    template<typename T>
    constexpr T Clamp(T value, T min, T max)
    {
        if (value < min)
        {
            return min;
        }
        else if (value > max)
        {
            return max;
        }
        return value;
    }

    template<Floating T = float32>
    constexpr T Saturate(T value)
    {
        return Clamp(value, static_cast<T>(0), static_cast<T>(1));
    }

#pragma endregion Clamping

#pragma region Interpolation
    template<NonTrivial T, Floating TFloat = float32>
    constexpr T Lerp(const T& start, const T& end, TFloat t)
    {
        return start + t * (end - start);
    }

    template<Trivial T, Floating TFloat = float32>
    constexpr T Lerp(T start, T end, TFloat t)
    {
        return start + t * (end - start);
    }

    template<NonTrivial T>
    constexpr T InvLerp(const T& start, const T& end, const T& value)
    {
        return (value - start) / (end - start);
    }

    template<Trivial T>
    constexpr T InvLerp(T start, T end, T value)
    {
        return (value - start) / (end - start);
    }

    template<Floating T = float32>
    constexpr T SmoothStep(T edge0, T edge1, T x)
    {
        x = Saturate((x - edge0) / (edge1 - edge0));
        return x * x * (static_cast<T>(3) - static_cast<T>(2) * x);
    }

    template<Floating T = float32>
    constexpr T SmootherStep(T edge0, T edge1, T x)
    {
        x = Saturate((x - edge0) / (edge1 - edge0));
        return x * x * x * (x * (x * static_cast<T>(6) - static_cast<T>(15)) + static_cast<T>(10));
    }

#pragma endregion Interpolation

    /** @return The absolute value of input arithmetic parameter */
    template<Numeric TNum>
    constexpr TNum Abs(TNum value)
    {
        return (value >= static_cast<TNum>(0) ? value : -value);
    }

    template<Numeric TNum>
    constexpr TNum Sqrt(TNum value)
    {
        return static_cast<TNum>(std::sqrt(value));
    }

    template<Numeric TNum1, Numeric TNum2>
    constexpr bool AreValuesClose(TNum1 A, TNum2 B, TNum1 tolerance = static_cast<TNum1>(0.000001))
    {
        return Abs(A - static_cast<TNum1>(B)) <= tolerance;
    }

    template<Integral TInt>
    constexpr bool IsPowerOfTwo(TInt value)
    {
        return (value & (value - 1)) == 0;
    }

    template<Integral TInt>
    constexpr bool IsEven(TInt value)
    {
        return (value & 1) == 0;
    }

    /** @return Rount down integer */
    template<Numeric T = int32, Floating TFloat = float32>
    constexpr T Floor(TFloat value)
    {
        const T integer = static_cast<T>(value);
        if (value >= integer)
        {
            return integer;
        }
        else
        {
            return integer - 1;
        }
    }

    template<Numeric T = int32, Floating TFloat = float32>
    constexpr T Ceil(TFloat value)
    {
        const T integer = static_cast<T>(value);
        if (value <= integer)
        {
            return integer;
        }
        else
        {
            return integer + 1;
        }
    }

    /** @return The nearest integer */
    template<Numeric T = int32, Floating TFloat = float32>
    constexpr T Round(TFloat value)
    {
        const TFloat fraction = value - Floor<T>(value);
        if (fraction >= static_cast<TFloat>(0.5))
        {
            return Ceil<T>(value);
        }
        else
        {
            return Floor<T>(value);
        }
    }

    /** @return The floor of the value if it's negative, otherwise the ceil */
    template<Numeric T = int32, Floating TFloat = float32>
    constexpr T FloorCeil(TFloat value)
    {
        if (value < static_cast<TFloat>(0))
        {
            return Floor<T>(value);
        }
        else
        {
            return Ceil<T>(value);
        }
    }

    constexpr auto Log2(auto value)
    {
        return std::log2(value);
    }

    template<Floating TFloat = float32>
    constexpr TFloat Modf(TFloat value, TFloat mod)
    {
        return std::fmod(value, mod);
    }

    template<Integral TInt>
    constexpr bool IsPrime(TInt n)
    {
        if (n <= 1)
        {
            return false;
        }

        if (n <= 3)
        {
            return true;
        }

        if (n % 2 == 0 || 
            n % 3 == 0)
        {
            return false;
        }

        for (TInt i = 5; (i * i) <= n; i += 6)
        {
            if (n % i == 0 ||
                n % (i + 2) == 0)
            {
                return false;
            }
        }

        return true;
    }
}