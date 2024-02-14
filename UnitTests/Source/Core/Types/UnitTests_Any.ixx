// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Any;

import jpt.Any;
import jpt.String;

bool UnitTest_Any()
{
    jpt::Any any;

    any = 42;
    JPT_LOG(any.As<int32>());

    // TOFIX
    //any = jpt::String("Hello");
    //JPT_LOG(any.As<jpt::String>());

    return true;
}

export bool RunUnitTests_Any()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Any(), "UnitTest_Any Failed");

    return true;
}
