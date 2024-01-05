// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Math;

import jpt.Concepts;
import jpt.Utilities;

export namespace jpt
{
	/** @return Clamped value. Ensure it's at least bigger than min and smaller than max. Exclusive */
	template<Comparable Type>
	Type GetClamped(const Type& value, const Type& min, const Type& max)
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
	template<Comparable Type>
	void ClampTo(Type& outValue, const Type& min, const Type& max)
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
	template<Comparable Type>
	void ClampTo(Type& outValue, Type&& min, Type&& max)
	{
		if (outValue < min)
		{
			outValue = jpt::move(min);
		}
		else if (outValue > max)
		{
			outValue = jpt::move(max);
		}
	}

	/** @return The absolute value of input arithmetic parameter */
	template<Numeric Type>
	constexpr Type GetAbs(Type value)
	{
		return (value >= static_cast<Type>(0) ? value : -value);
	}

	/** @return true if two values are close enough */
	template<Integral Type>
	bool AreValuesClose(Type A, Type B)
	{
		return A == B;
	}

	template<Floating Type>
	bool AreValuesClose(Type A, Type B, Type tolerance = 0.000001)
	{
		return jpt::GetAbs(A - B) < tolerance;
	}

	template<ComparableTrivial Type>
	Type min(Type a, Type b)
	{
		return (a < b) ? a : b;
	}

	template<ComparableNonTrivial Type>
	const Type& min(const Type& a, const Type& b)
	{
		return (a < b) ? a : b;
	}

	template<ComparableTrivial Type>
	Type max(Type a, Type b)
	{
		return (a > b) ? a : b;
	}

	template<ComparableNonTrivial Type>
	const Type& max(const Type& a, const Type& b)
	{
		return (a > b) ? a : b;
	}
}