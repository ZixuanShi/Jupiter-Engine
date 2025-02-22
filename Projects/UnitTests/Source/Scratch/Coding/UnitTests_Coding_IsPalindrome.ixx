// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Coding_IsPalindrome;

import jpt.Utilities;
import jpt.TypeDefs;

bool IsPalindrome(int32 x)
{
	if (x < 0)
	{
		return false;
	}

	int32 reversed = 0;
	int32 temp = x;

	while (temp != 0)
	{
		const int32 digit = temp % 10;
		reversed = reversed * 10 + digit;
		temp /= 10;
	}

	return reversed == x;
}

bool IsPalindrome_V2(int32 x)
{
	if (x < 0)
	{
		return false;
	}

	// Get digit count
	int32 copy = x;
	int32 digitCount = 0;
	while (copy != 0)
	{
		copy /= 10;
		++digitCount;
	}

	// Compare left and right digits
	copy = x;
	for (int32 i = 0; i < digitCount / 2; ++i)
	{
		const int32 factor = static_cast<int32>(pow(10, digitCount - (i * 2) - 1));

		// Left
		const int32 left = copy / factor;

		// Right
		const int32 right = copy % 10;

		if (left != right)
		{
			return false;
		}

		// Remove left and right digits
		copy = copy % factor;
		copy /= 10;
	}

	return true;
}

export bool UnitTests_Coding_IsPalindrome()
{
	JPT_ENSURE(IsPalindrome(121) == true);
	JPT_ENSURE(IsPalindrome(-121) == false);
	JPT_ENSURE(IsPalindrome(10) == false);
	JPT_ENSURE(IsPalindrome(-101) == false);
	JPT_ENSURE(IsPalindrome(0) == true);
	JPT_ENSURE(IsPalindrome(100001) == true);
	JPT_ENSURE(IsPalindrome(432234) == true);

	JPT_ENSURE(IsPalindrome_V2(121) == true);
	JPT_ENSURE(IsPalindrome_V2(-121) == false);
	JPT_ENSURE(IsPalindrome_V2(10) == false);
	JPT_ENSURE(IsPalindrome_V2(-101) == false);
	JPT_ENSURE(IsPalindrome_V2(0) == true);
	JPT_ENSURE(IsPalindrome_V2(100001) == true);
	JPT_ENSURE(IsPalindrome_V2(432234) == true);

	return true;
}