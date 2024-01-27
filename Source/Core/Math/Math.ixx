// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Math;

import jpt.Concepts;
import jpt.Utilities;

export namespace jpt
{
#pragma region Clamping
	/** @return Clamped value. Ensure it's at least bigger than min and smaller than max. Exclusive */
	template<ComparableTrivial T>
	constexpr T GetClamped(T value, T min, T max)
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
	constexpr T GetClamped(const T& value, const T& min, const T& max)
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
	template<ComparableTrivial T>
	void ClampTo(T& outValue, T min, T max)
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
	void ClampTo(T& outValue, const T& min, const T& max)
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

	/** @return The absolute value of input arithmetic parameter */
	template<Numeric TNum>
	constexpr TNum GetAbs(TNum value)
	{
		return (value >= static_cast<TNum>(0) ? value : -value);
	}

	/** @return true if two values are close enough */
	template<Integral TInt>
	constexpr bool AreValuesClose(TInt A, TInt B)
	{
		return A == B;
	}

	template<Floating TFloat>
	constexpr bool AreValuesClose(TFloat A, TFloat B, TFloat tolerance = static_cast<TFloat>(0.000001))
	{
		return GetAbs(A - B) < tolerance;
	}
}