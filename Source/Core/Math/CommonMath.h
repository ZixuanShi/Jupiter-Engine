// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	/** @return Clamped value. Ensure it's at least bigger than min and smaller than max. Exclusive */
	template<class Type>
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
	template<class Type>
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

	/** @return The absolute value of input arithmetic parameter */
	template<typename Type>
	inline constexpr jpt::enable_if_t<std::is_arithmetic_v<Type>, Type> GetAbs(Type a)
	{
		return (a >= static_cast<Type>(0) ? a : -a);
	}

	/** @return true if two values are close enough */
	template<typename IntegerType>
	inline jpt::enable_if_t<std::is_integral_v<IntegerType>, bool> AreValuesClose(IntegerType left, IntegerType right)
	{
		return left == right;
	}
	template<typename FloatingType>
	inline jpt::enable_if_t<std::is_floating_point_v<FloatingType>, bool> AreValuesClose(FloatingType left, FloatingType right, FloatingType tolerance = 0.000001)
	{
		return jpt::GetAbs(left - right) < tolerance;
	}
}