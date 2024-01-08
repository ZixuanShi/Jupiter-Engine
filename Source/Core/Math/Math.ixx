// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Math;

import jpt.Concepts;
import jpt.Utilities;

export namespace jpt
{
	/** @return Clamped value. Ensure it's at least bigger than min and smaller than max. Exclusive */
	template<ComparableTrivial T>
	T GetClamped(T value, T min, T max)
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
	T GetClamped(const T& value, const T& min, const T& max)
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
	bool AreValuesClose(IntT A, IntT B)
	{
		return A == B;
	}

	template<Floating FloatT>
	bool AreValuesClose(FloatT A, FloatT B, FloatT tolerance = static_cast<FloatT>(0.000001))
	{
		return jpt::GetAbs(A - B) < tolerance;
	}

	template<ComparableTrivial T>
	T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template<ComparableTrivial T>
	T Min(T a, T b, T c)
	{
		T temp = Min(a, b);
		return Min(temp, c);
	}

	template<ComparableTrivial T>
	T Min(T a, T b, T c, T d)
	{
		T temp1 = Min(a, b);
		T temp2 = Min(c, d);
		return Min(temp1, temp2);
	}

	template<ComparableNonTrivial T>
	T Min(const T& a, const T& b)
	{
		return (a < b) ? a : b;
	}

	template<ComparableNonTrivial T>
	T Min(const T& a, const T& b, const T& c)
	{
		T temp = Min(a, b);
		return Min(temp, c);
	}

	template<ComparableNonTrivial T>
	T Min(const T& a, const T& b, const T& c, const T& d)
	{
		T temp1 = Min(a, b);
		T temp2 = Min(c, d);
		return Min(temp1, temp2);
	}

	template<ComparableTrivial T>
	T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	template<ComparableTrivial T>
	T Max(T a, T b, T c)
	{
		T temp = Max(a, b);
		return Max(temp, c);
	}

	template<ComparableTrivial T>
	T Max(T a, T b, T c, T d)
	{
		T temp1 = Max(a, b);
		T temp2 = Max(c, d);
		return Max(temp1, temp2);
	}

	template<ComparableNonTrivial T>
	T Max(const T& a, const T& b)
	{
		return (a > b) ? a : b;
	}

	template<ComparableNonTrivial T>
	T Max(const T& a, const T& b, const T& c)
	{
		T temp = Max(a, b);
		return Max(temp, c);
	}

	template<ComparableNonTrivial T>
	T Max(const T& a, const T& b, const T& c, const T& d)
	{
		T temp1 = Max(a, b);
		T temp2 = Max(c, d);
		return Max(temp1, temp2);
	}
}