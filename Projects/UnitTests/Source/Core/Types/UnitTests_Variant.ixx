// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Variant;

import jpt.Variant;
import jpt.Utilities;
import jpt.TypeDefs;
import jpt.String;

bool UnitTests_Variant()
{
    jpt::Variant<int32, char, bool, jpt::String> variant = 1;

    // int32
    JPT_ENSURE(variant.Is<int32>());
    JPT_ENSURE(variant.As<int32>() == 1);

    variant = 5;
    JPT_ENSURE(variant.As<int32>() == 5);

    variant.As<int32>() = 42;
    JPT_ENSURE(variant.As<int32>() == 42);

    // char
    variant = 'a';
    JPT_ENSURE(!variant.Is<int32>());
    JPT_ENSURE(variant.Is<char>());
    JPT_ENSURE(variant.As<char>() == 'a');

    // String
    variant = jpt::String("Hello");
    JPT_ENSURE(!variant.Is<char>());
    JPT_ENSURE(variant.Is<jpt::String>());
    JPT_ENSURE(variant.As<jpt::String>() == "Hello");
    variant.As<jpt::String>().Append(" World");
    JPT_ENSURE(variant.As<jpt::String>() == "Hello World");

    return true;
}

bool UnitTests_Variant_Copy()
{
    jpt::Variant<int32, char, bool, jpt::String> originalVariant;
    originalVariant = jpt::String("Original");

    jpt::Variant<int32, char, bool, jpt::String> copiedVariant = originalVariant;
    copiedVariant.As<jpt::String>() = "Copied";

    JPT_ENSURE(originalVariant.As<jpt::String>() == "Original");
    JPT_ENSURE(copiedVariant.As<jpt::String>() == "Copied");

    originalVariant = 42;
    copiedVariant = originalVariant;
    JPT_ENSURE(copiedVariant.As<int32>() == 42);

    copiedVariant.As<int32>() = 84;

    JPT_ENSURE(originalVariant.As<int32>() == 42);
    JPT_ENSURE(copiedVariant.As<int32>() == 84);

    bool b = false;
    copiedVariant = b;
    JPT_ENSURE(copiedVariant.Is<bool>());
    JPT_ENSURE(copiedVariant.As<bool>() == false);
    
    char c = 'a';
    originalVariant = c;
    JPT_ENSURE(originalVariant.Is<char>());
    JPT_ENSURE(originalVariant.As<char>() == 'a');

    copiedVariant = originalVariant;
    JPT_ENSURE(originalVariant.Is<char>());
    JPT_ENSURE(originalVariant.As<char>() == 'a');
    JPT_ENSURE(copiedVariant.Is<char>());
    JPT_ENSURE(copiedVariant.As<char>() == 'a');

    copiedVariant.As<char>() = 'b';
    JPT_ENSURE(originalVariant.As<char>() == 'a');
    JPT_ENSURE(copiedVariant.As<char>() == 'b');

    jpt::String str = "Hello World";
    jpt::Variant<int32, char, bool, jpt::String> anotherCopiedVariant = str;
    JPT_ENSURE(anotherCopiedVariant.As<jpt::String>() == "Hello World");
    JPT_ENSURE(str == "Hello World");

    return true;
}

bool UnitTests_Variant_Move()
{
    jpt::Variant<int32, char, bool, jpt::String> originalVariant;
    originalVariant = jpt::String("Original");

    jpt::Variant<int32, char, bool, jpt::String> movedVariant = Move(originalVariant);

    JPT_ENSURE(movedVariant.As<jpt::String>() == "Original");
    JPT_ENSURE(!originalVariant.Is<jpt::String>());

    jpt::String str = "Hello World";
    movedVariant = Move(str);
    JPT_ENSURE(movedVariant.As<jpt::String>() == "Hello World");
    JPT_ENSURE(str.IsEmpty());

    str = "Hello World";
    jpt::Variant<int32, char, bool, jpt::String> anotherMovedVariant = Move(str);
    JPT_ENSURE(anotherMovedVariant.As<jpt::String>() == "Hello World");
    JPT_ENSURE(str.IsEmpty());

    return true;
}

export bool RunUnitTests_Variant()
{
    JPT_ENSURE(UnitTests_Variant());
    JPT_ENSURE(UnitTests_Variant_Copy());
    JPT_ENSURE(UnitTests_Variant_Move());

    return true;
}
