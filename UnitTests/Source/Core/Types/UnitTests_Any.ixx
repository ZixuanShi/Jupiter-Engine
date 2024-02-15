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

    // float
    float f = 3.14f;
    any = f;
    JPT_RETURN_FALSE_IF_ERROR(any.As<float>() != f, "");
    JPT_RETURN_FALSE_IF_ERROR(!any.Is<float>(), "");

    // String
    jpt::String str = "Hello";
    any = str;
    JPT_RETURN_FALSE_IF_ERROR(any.Is<int32>(), "");
    JPT_RETURN_FALSE_IF_ERROR(any.As<jpt::String>() != str, "");
    JPT_RETURN_FALSE_IF_ERROR(!any.Is<jpt::String>(), "");

    // Copy constructor
    //jpt::Any any2 = any;
    //JPT_RETURN_FALSE_IF_ERROR(any.As<jpt::String>() != str, "");
    //JPT_RETURN_FALSE_IF_ERROR(any2.As<jpt::String>() != str, "");
    
    //any = any2;

    return true;
}

bool UnitTest_Any_Move()
{
    jpt::String str1 = "Hello";
    jpt::String str2 = "World";

    // Move constructor
    jpt::Any any = Move(str1);
    JPT_RETURN_FALSE_IF_ERROR(any.As<jpt::String>() != "Hello", "");
    JPT_RETURN_FALSE_IF_ERROR(!str1.IsEmpty(), "");

    // Move assignment
    any = Move(str2);
    JPT_RETURN_FALSE_IF_ERROR(any.As<jpt::String>() != "World", "");
    JPT_RETURN_FALSE_IF_ERROR(!str2.IsEmpty(), "");

    return true;
}

export bool RunUnitTests_Any()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Any(), "UnitTest_Any Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Any_Copy(), "UnitTest_Any_Copy Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Any_Move(), "UnitTest_Any_Move Failed");

    return true;
}
