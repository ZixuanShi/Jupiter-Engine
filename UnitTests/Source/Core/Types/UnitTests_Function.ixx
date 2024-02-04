// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Function;

import jpt.Function;
import jpt.TypeDefs;

bool TestFunction1(int32 n, char c)
{
    //JPT_LOG(n);
    //JPT_LOG(c);

	return true;
}

bool TestFunction2(int32 n, char c)
{
    //JPT_LOG(n * 2);
    //JPT_LOG(c);

    return true;
}

bool UnitTest_Function()
{
    jpt::Function<bool(int32, char)> function = TestFunction1;

    function(1, 'a');
    function = TestFunction2;
    function(1, 'a');

    return true;
}

export bool RunUnitTests_Function()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Function(), "UnitTest_Function Failed");

    return true;
}
