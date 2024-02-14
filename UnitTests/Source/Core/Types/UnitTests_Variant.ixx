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
    jpt::Variant<int32, char, bool, jpt::String> originalVariant;
    originalVariant = jpt::String("Original");

    jpt::Variant<int32, char, bool, jpt::String> copiedVariant = originalVariant;
    copiedVariant.As<jpt::String>() = "Copied";

    JPT_RETURN_FALSE_IF_ERROR(originalVariant.As<jpt::String>() != "Original", "");
    JPT_RETURN_FALSE_IF_ERROR(copiedVariant.As<jpt::String>() != "Copied", "");

    originalVariant = 42;
    copiedVariant = originalVariant;
    JPT_RETURN_FALSE_IF_ERROR(copiedVariant.As<int32>() != 42, "");

    copiedVariant.As<int32>() = 84;

    JPT_RETURN_FALSE_IF_ERROR(originalVariant.As<int32>() != 42, "");
    JPT_RETURN_FALSE_IF_ERROR(copiedVariant.As<int32>() != 84, "");

    copiedVariant = false;
    JPT_RETURN_FALSE_IF_ERROR(!copiedVariant.Is<bool>(), "");
    JPT_RETURN_FALSE_IF_ERROR(copiedVariant.As<bool>() != false, "");
    
    originalVariant = 'a';
    JPT_RETURN_FALSE_IF_ERROR(!originalVariant.Is<char>(), "");
    JPT_RETURN_FALSE_IF_ERROR(originalVariant.As<char>() != 'a', "");

    copiedVariant = originalVariant;
    JPT_RETURN_FALSE_IF_ERROR(!originalVariant.Is<char>(), "");
    JPT_RETURN_FALSE_IF_ERROR(originalVariant.As<char>() != 'a', "");
    JPT_RETURN_FALSE_IF_ERROR(!copiedVariant.Is<char>(), "");
    JPT_RETURN_FALSE_IF_ERROR(copiedVariant.As<char>() != 'a', "");

    copiedVariant.As<char>() = 'b';
    JPT_RETURN_FALSE_IF_ERROR(originalVariant.As<char>() != 'a', "");
    JPT_RETURN_FALSE_IF_ERROR(copiedVariant.As<char>() != 'b', "");

    jpt::String str = "Hello World";
    jpt::Variant<int32, char, bool, jpt::String> anotherCopiedVariant = str;
    JPT_RETURN_FALSE_IF_ERROR(anotherCopiedVariant.As<jpt::String>() != "Hello World", "");
    JPT_RETURN_FALSE_IF_ERROR(str != "Hello World", "");

    return true;
}

bool UnitTest_Variant_Move()
{
    jpt::Variant<int32, char, bool, jpt::String> originalVariant;
    originalVariant = jpt::String("Original");

    jpt::Variant<int32, char, bool, jpt::String> movedVariant = Move(originalVariant);

    JPT_RETURN_FALSE_IF_ERROR(movedVariant.As<jpt::String>() != "Original", "");
    JPT_RETURN_FALSE_IF_ERROR(originalVariant.Is<jpt::String>(), "");

    jpt::String str = "Hello World";
    movedVariant = Move(str);
    JPT_RETURN_FALSE_IF_ERROR(movedVariant.As<jpt::String>() != "Hello World", "");
    JPT_RETURN_FALSE_IF_ERROR(!str.IsEmpty(), "");

    str = "Hello World";
    jpt::Variant<int32, char, bool, jpt::String> anotherMovedVariant = Move(str);
    JPT_RETURN_FALSE_IF_ERROR(anotherMovedVariant.As<jpt::String>() != "Hello World", "");
    JPT_RETURN_FALSE_IF_ERROR(!str.IsEmpty(), "");

    return true;
}

export bool RunUnitTests_Variant()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Variant(), "UnitTest_Variant Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Variant_Copy(), "UnitTest_Variant_Copy Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Variant_Move(), "UnitTest_Variant_Move Failed");

    return true;
}
