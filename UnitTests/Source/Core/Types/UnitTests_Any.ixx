// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"
#include <typeinfo>

export module UnitTests_Any;

import jpt.Any;
import jpt.String;
import jpt.Function;
import jpt.TypeDefs;

bool UnitTest_Any()
{
    jpt::Any any;

    // int32
    any = 42;
    JPT_RETURN_FALSE_IF_ERROR(any.As<int32>() != 42, "");
    JPT_RETURN_FALSE_IF_ERROR(!any.Is<int32>(), "");
    JPT_RETURN_FALSE_IF_ERROR(any.Is<char>(), "");

    // String
    any = jpt::String("Hello");
    JPT_RETURN_FALSE_IF_ERROR(any.Is<int32>(), "");
    JPT_RETURN_FALSE_IF_ERROR(any.As<jpt::String>() != "Hello", "");
    JPT_RETURN_FALSE_IF_ERROR(!any.Is<jpt::String>(), "");

    // float
    any = 3.14f;
    JPT_RETURN_FALSE_IF_ERROR(any.As<float>() != 3.14f, "");
    JPT_RETURN_FALSE_IF_ERROR(!any.Is<float>(), "");

    return true;
}

bool UnitTest_Any_Copy()
{
    jpt::Any any;

    // int32
    int32 n = 42;
    any = n;
    JPT_RETURN_FALSE_IF_ERROR(any.As<int32>() != n, "");
    JPT_RETURN_FALSE_IF_ERROR(!any.Is<int32>(), "");
    JPT_RETURN_FALSE_IF_ERROR(any.Is<char>(), "");

    // String
    jpt::String str = "Hello";
    any = str;
    JPT_RETURN_FALSE_IF_ERROR(any.Is<int32>(), "");
    JPT_RETURN_FALSE_IF_ERROR(any.As<jpt::String>() != str, "");
    JPT_RETURN_FALSE_IF_ERROR(!any.Is<jpt::String>(), "");

    // float
    float f = 3.14f;
    any = f;
    JPT_RETURN_FALSE_IF_ERROR(any.As<float>() != f, "");
    JPT_RETURN_FALSE_IF_ERROR(!any.Is<float>(), "");

    return true;
}

export bool RunUnitTests_Any()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Any(), "UnitTest_Any Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Any_Copy(), "UnitTest_Any_Copy Failed");

    return true;
}
