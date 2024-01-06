// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Math;

import jpt.Concepts;
import jpt.Utilities;

export namespace jpt
{
	/** @return Clamped value. Ensure it's at least bigger than min and smaller than max. Exclusive */
	template<ComparableTrivial Type>
	Type GetClamped(Type value, Type min, Type max)
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
	template<ComparableNonTrivial Type>
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
	template<ComparableNonTrivial Type>
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
	template<ComparableTrivial Type>
	void ClampTo(Type& outValue, Type min, Type max)
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
	bool AreValuesClose(Type A, Type B, Type tolerance = static_cast<Type>(0.000001))
	{
		return jpt::GetAbs(A - B) < tolerance;
	}

	template<ComparableTrivial Type>
	Type Min(Type a, Type b)
	{
		return (a < b) ? a : b;
	}

	template<ComparableTrivial Type>
	Type Min(Type a, Type b, Type c)
	{
		Type temp = Min(a, b);
		return Min(temp, c);
	}

	template<ComparableTrivial Type>
	Type Min(Type a, Type b, Type c, Type d)
	{
		Type temp1 = Min(a, b);
		Type temp2 = Min(c, d);
		return Min(temp1, temp2);
	}

	template<ComparableNonTrivial Type>
	Type Min(const Type& a, const Type& b)
	{
		return (a < b) ? a : b;
	}

	template<ComparableNonTrivial Type>
	Type Min(const Type& a, const Type& b, const Type& c)
	{
		Type temp = Min(a, b);
		return Min(temp, c);
	}

	template<ComparableNonTrivial Type>
	Type Min(const Type& a, const Type& b, const Type& c, const Type& d)
	{
		Type temp1 = Min(a, b);
		Type temp2 = Min(c, d);
		return Min(temp1, temp2);
	}

	template<ComparableTrivial Type>
	Type Max(Type a, Type b)
	{
		return (a > b) ? a : b;
	}

	template<ComparableTrivial Type>
	Type Max(Type a, Type b, Type c)
	{
		Type temp = Max(a, b);
		return Max(temp, c);
	}

	template<ComparableTrivial Type>
	Type Max(Type a, Type b, Type c, Type d)
	{
		Type temp1 = Max(a, b);
		Type temp2 = Max(c, d);
		return Max(temp1, temp2);
	}

	template<ComparableNonTrivial Type>
	Type Max(const Type& a, const Type& b)
	{
		return (a > b) ? a : b;
	}

	template<ComparableNonTrivial Type>
	Type Max(const Type& a, const Type& b, const Type& c)
	{
		Type temp = Max(a, b);
		return Max(temp, c);
	}

	template<ComparableNonTrivial Type>
	Type Max(const Type& a, const Type& b, const Type& c, const Type& d)
	{
		Type temp1 = Max(a, b);
		Type temp2 = Max(c, d);
		return Max(temp1, temp2);
	}
}