// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Coding_RomanToInteger;

import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;

int32 RomanToInteger(const jpt::String& roman)
{
	auto romanToInteger= [roman](size_t i)
		{
			switch (roman[i])
			{
				case 'I': return 1;
				case 'V': return 5;
				case 'X': return 10;
				case 'L': return 50;
				case 'C': return 100;
				case 'D': return 500;
				case 'M': return 1000;
				default:  return 0;
			}
		};

	int32 result = 0;

	for (size_t i = 0; i < roman.Count(); ++i)
	{
		const int32 current = romanToInteger(i);
		const int32 next = romanToInteger(i + 1);

		if (current < next)
		{
			result -= current;
		}
		else
		{
			result += current;
		}
	}

	return result;
}

export bool UnitTest_Coding_RomanToInteger()
{
	JPT_ENSURE(RomanToInteger("III") == 3);
	JPT_ENSURE(RomanToInteger("IV") == 4);
	JPT_ENSURE(RomanToInteger("IX") == 9);
	JPT_ENSURE(RomanToInteger("LVIII") == 58);
	JPT_ENSURE(RomanToInteger("MCMXCIV") == 1994);

	return true;
}