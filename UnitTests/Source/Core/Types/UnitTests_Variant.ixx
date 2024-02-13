// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Variant;

import jpt.Variant;
import jpt.Utilities;
import jpt.TypeDefs;
import jpt.String;

bool UnitTest_Variant()
{
    jpt::Variant<int32, char, bool, jpt::String> variant = 1;

    // int32
    JPT_RETURN_FALSE_IF_ERROR(!variant.Is<int32>(), "");
    JPT_RETURN_FALSE_IF_ERROR(variant.As<int32>() != 1, "");

    variant = 5;
    JPT_RETURN_FALSE_IF_ERROR(variant.As<int32>() != 5, "");

    variant.As<int32>() = 42;
    JPT_RETURN_FALSE_IF_ERROR(variant.As<int32>() != 42, "");

    // char
    variant = 'a';
    JPT_RETURN_FALSE_IF_ERROR(variant.Is<int32>(), "");
    JPT_RETURN_FALSE_IF_ERROR(!variant.Is<char>(), "");
    JPT_RETURN_FALSE_IF_ERROR(variant.As<char>() != 'a', "");

    // String
    variant = jpt::String("Hello");
    JPT_RETURN_FALSE_IF_ERROR(variant.Is<char>(), "");
    JPT_RETURN_FALSE_IF_ERROR(!variant.Is<jpt::String>(), "");
    JPT_RETURN_FALSE_IF_ERROR(variant.As<jpt::String>() != "Hello", "");
    variant.As<jpt::String>().Append(" World");
    JPT_RETURN_FALSE_IF_ERROR(variant.As<jpt::String>() != "Hello World", "");

    return true;
}

bool UnitTest_Variant_Copy()
{
    //jpt::Variant<int32, char, bool, jpt::String> variant;

    return true;
}

bool UnitTest_Variant_Move()
{
    //jpt::Variant<int32, char, bool, jpt::String> variant;

    return true;
}

export bool RunUnitTests_Variant()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Variant(), "UnitTest_Variant Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Variant_Copy(), "UnitTest_Variant_Copy Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Variant_Move(), "UnitTest_Variant_Move Failed");

    return true;
}
