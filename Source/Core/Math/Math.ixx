// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"

#include <cmath>

export module jpt.Math;

import jpt.Concepts;
import jpt.Constants;
import jpt.TypeDefs;
import jpt.Utilities;

export namespace jpt
{
#pragma region Clamping
    /** @return Clamped value. Ensure it's at least bigger than min and smaller than max. Exclusive */
    template<ComparableTrivial T>
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
    template<ComparableNonTrivial T>
    constexpr T Clamp(const T& value, const T& min, const T& max)
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

    /** Clamps a value. Ensure it's at least bigger than min and smaller than max. Exclusive
        @param outValue:    Will be changed if less than min or bigger than max */
    template<ComparableTrivial T>
    constexpr void ClampTo(T& outValue, T min, T max)
    {
        if (outValue < min)
        {
            outValue = min;
        }
        else if (outValue > max)
        {
            outValue = max;
        }
    }
    template<ComparableNonTrivial T>
    constexpr void ClampTo(T& outValue, const T& min, const T& max)
    {
        if (outValue < min)
        {
            outValue = min;
        }
        else if (outValue > max)
        {
            outValue = max;
        }
    }

#pragma endregion Clamping

#pragma region Lerp
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
    template<NonTrivial T, Floating TFloat = float32>
    constexpr void Lerp(T& value, const T& start, const T& end, TFloat t)
    {
        value = start + t * (end - start);
    }
    template<Trivial T, Floating TFloat = float32>
    constexpr void Lerp(T& value, T start, T end, TFloat t)
    {
        value = start + t * (end - start);
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
#pragma endregion Lerp

#pragma region MinMax

    template <Trivial TFirst, typename... TRest>
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
    template <NonTrivial TFirst, typename... TRest>
    constexpr TFirst Min(const TFirst& first, const TRest&... inputs)
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

    template <Trivial TFirst, typename... TRest>
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
    template <NonTrivial TFirst, typename... TRest>
    constexpr TFirst Max(const TFirst& first, const TRest&... inputs)
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

    template<Numeric T = int32, Floating TFloat = float32>
    constexpr T Round(TFloat value)
    {
        return static_cast<T>(std::round(value));
    }

    template<Numeric T = int32, Floating TFloat = float32>
    constexpr T Floor(TFloat value)
    {
        return static_cast<T>(std::floor(value));
    }

    template<Numeric T = int32, Floating TFloat = float32>
    constexpr T Ceil(TFloat value)
    {
        return static_cast<T>(std::ceil(value));
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