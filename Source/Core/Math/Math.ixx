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
	template<typename T>
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
		@param outValue:	Will be changed if less than min or bigger than max */
	template<typename T>
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
	template<typename T, Floating TFloat = float32>
	constexpr T Lerp(const T& start, const T& end, TFloat t)
	{
		return start + t * (end - start);
	}
	template<typename T, Floating TFloat = float32>
	constexpr void Lerp(T& value, const T& start, const T& end, TFloat t)
	{
		value = start + t * (end - start);
	}
	template<typename T>
	constexpr T InvLerp(const T& start, const T& end, const T& value)
	{
		return (value - start) / (end - start);
	}
#pragma endregion Lerp

#pragma region MinMax
	template <typename TFirst, typename... TRest>
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

	template <typename TFirst, typename... TRest>
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

	template<Floating TFloat = float32>
	constexpr TFloat ToDegrees(TFloat radians)
	{
		return radians * static_cast<TFloat>(180) / kPi<TFloat>;
	}
	template<typename T>
	constexpr T ToDegrees(const T& obj)
	{
		return obj * static_cast<T::NumericType>(180) / kPi<T::NumericType>;
	}
	template<Floating TFloat = float32>
	constexpr TFloat ToRadians(TFloat degrees)
	{
		return degrees * kPi<TFloat> / static_cast<TFloat>(180);
	}
	template<typename T>
	constexpr T ToRadians(const T& obj)
	{
		return obj * kPi<T::NumericType> / static_cast<T::NumericType>(180);
	}
}