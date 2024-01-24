// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Math;

import jpt.Concepts;
import jpt.Utilities;

export namespace jpt
{
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

	/** @return The absolute value of input arithmetic parameter */
	template<Numeric NumT>
	constexpr NumT GetAbs(NumT value)
	{
		return (value >= static_cast<NumT>(0) ? value : -value);
	}

	/** @return true if two values are close enough */
	template<Integral IntT>
	constexpr bool AreValuesClose(IntT A, IntT B)
	{
		return A == B;
	}

	template<Floating FloatT>
	constexpr bool AreValuesClose(FloatT A, FloatT B, FloatT tolerance = static_cast<FloatT>(0.000001))
	{
		return GetAbs(A - B) < tolerance;
	}

	template <Trivial FirstT, typename... RestT>
	constexpr FirstT Min(FirstT first, RestT... inputs)
	{
		FirstT smallestVal = first;

		([&]
			{
				if (smallestVal > inputs)
				{
					smallestVal = inputs;
				}
			} (), ...);

		return smallestVal;
	}
	template <NonTrivial FirstT, typename... RestT>
	constexpr FirstT Min(const FirstT& first, const RestT&... inputs)
	{
		FirstT smallestVal = first;

		([&]
			{
				if (smallestVal > inputs)
				{
					smallestVal = inputs;
				}
			} (), ...);

		return smallestVal;
	}

	template <Trivial FirstT, typename... RestT>
	constexpr FirstT Max(FirstT first, RestT... inputs)
	{
		FirstT largestVal = first;

		([&]
			{
				if (largestVal < inputs)
				{
					largestVal = inputs;
				}
			} (), ...);

		return largestVal;
	}
	template <NonTrivial FirstT, typename... RestT>
	constexpr FirstT Max(const FirstT& first, const RestT&... inputs)
	{
		FirstT largestVal = first;

		([&]
			{
				if (largestVal < inputs)
				{
					largestVal = inputs;
				}
			} (), ...);

		return largestVal;
	}
}