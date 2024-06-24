// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Coding_IsPalindrome;

import jpt.CoreModules;

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

export bool UnitTest_Coding_IsPalindrome()
{
	JPT_ENSURE(IsPalindrome(121) == true);
	JPT_ENSURE(IsPalindrome(-121) == false);
	JPT_ENSURE(IsPalindrome(10) == false);
	JPT_ENSURE(IsPalindrome(-101) == false);
	JPT_ENSURE(IsPalindrome(0) == true);
	JPT_ENSURE(IsPalindrome(100001) == true);

	return true;
}