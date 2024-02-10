// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Function;

import jpt.Function;
import jpt.TypeDefs;

bool TestFunction1(int32& n)
{
    if (n < 10)
    {
        return true;
    }

    n = 100;
    return false;
}

bool UnitTest_Function()
{
    // Functions
    jpt::Function<bool(int32&)> function = TestFunction1;
    int32 n = 5;
    JPT_RETURN_FALSE_IF_ERROR(!function(n), "");
    JPT_RETURN_FALSE_IF_ERROR(n != 5, "");

    n = 10;
    JPT_RETURN_FALSE_IF_ERROR(function(n), "");
	JPT_RETURN_FALSE_IF_ERROR(n != 100, "");

    // Lambda
    auto lambda = [](int32& n) -> bool
        { 
            if (n < 10)
            {
                return true;
            }

            n = 100;
            return false;
        };
    function = lambda;
    n = 5;
    JPT_RETURN_FALSE_IF_ERROR(!function(n), "");
    JPT_RETURN_FALSE_IF_ERROR(n != 5, "");

    n = 10;
    JPT_RETURN_FALSE_IF_ERROR(function(n), "");
    JPT_RETURN_FALSE_IF_ERROR(n != 100, "");

    return true;
}

export bool RunUnitTests_Function()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Function(), "UnitTest_Function Failed");

    return true;
}
