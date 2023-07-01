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
}