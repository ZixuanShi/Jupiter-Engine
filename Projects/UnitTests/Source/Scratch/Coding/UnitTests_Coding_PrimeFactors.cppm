// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Coding_PrimeFactors;

import jpt.Concepts;
import jpt.Utilities;
import jpt.TypeDefs;
import jpt.DynamicArray;
import jpt.Math;

jpt::DynamicArray<int32> GetPrimeFactors(int32 n)
{
    jpt::DynamicArray<int32> factors;
    
    while (n % 2 == 0)
    {
        factors.EmplaceBack(2);
        n /= 2;
    }
    
    for (int32 i = 3; i <= jpt::Sqrt(n); i += 2)
    {
        while (n % i == 0)
        {
            factors.EmplaceBack(i);
            n /= i;
        }
    }

    if (n > 2)
    {
        factors.EmplaceBack(n);
    }

    return factors;
}

export bool UnitTests_Coding_PrimeFactors()
{
    JPT_ENSURE(GetPrimeFactors(5) == jpt::DynamicArray<int32>{ 5 });
    JPT_ENSURE(GetPrimeFactors(11) == jpt::DynamicArray<int32>{ 11 });

    JPT_ENSURE((GetPrimeFactors(4) == jpt::DynamicArray<int32>{ 2, 2 }));
    JPT_ENSURE((GetPrimeFactors(8) == jpt::DynamicArray<int32>{ 2, 2, 2 }));
    JPT_ENSURE((GetPrimeFactors(12) == jpt::DynamicArray<int32>{ 2, 2, 3 }));
    JPT_ENSURE((GetPrimeFactors(16) == jpt::DynamicArray<int32>{ 2, 2, 2, 2 }));
    JPT_ENSURE((GetPrimeFactors(24) == jpt::DynamicArray<int32>{ 2, 2, 2, 3 }));
    JPT_ENSURE((GetPrimeFactors(64) == jpt::DynamicArray<int32>{ 2, 2, 2, 2, 2, 2 }));
    JPT_ENSURE((GetPrimeFactors(128) == jpt::DynamicArray<int32>{ 2, 2, 2, 2, 2, 2, 2 }));

    return true;
}