// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"
#include "Profiling/TimingProfiler.h"

#include <typeinfo>

export module UnitTests_Any;

import jpt.Any;
import jpt.String;
import jpt.Function;
import jpt.TypeDefs;
import jpt.Variant;

bool UnitTest_Any()
{
    jpt::Any any;

    // int32
    any = 42;
    JPT_ENSURE(any.As<int32>() == 42);
    JPT_ENSURE(any.Is<int32>());
    JPT_ENSURE(!any.Is<char>());

    any = 103;
    JPT_ENSURE(any.As<int32>() == 103);

    // String
    any = jpt::String("Hello");
    JPT_ENSURE(!any.Is<int32>());
    JPT_ENSURE(any.As<jpt::String>() == "Hello");
    JPT_ENSURE(any.Is<jpt::String>());

    // float
    any = 3.14f;
    JPT_ENSURE(any.As<float>() == 3.14f);
    JPT_ENSURE(any.Is<float>());

    return true;
}

bool UnitTest_Any_Copy()
{
    jpt::Any any;

    // int32
    int32 n = 42;
    any = n;
    JPT_ENSURE(any.As<int32>() == n);
    JPT_ENSURE(any.Is<int32>());
    JPT_ENSURE(!any.Is<char>());

    // float
    float f = 3.14f;
    any = f;
    JPT_ENSURE(any.As<float>() == f);
    JPT_ENSURE(any.Is<float>());

    // String
    jpt::String str = "Hello";
    any = str;
    JPT_ENSURE(!any.Is<int32>());
    JPT_ENSURE(any.As<jpt::String>() == str);
    JPT_ENSURE(any.Is<jpt::String>());

    any = str;

    // CopyAny
    //jpt::Any any2;
    //any2 = any;

    // Copy variant
    using TVariant = jpt::Variant<int32, char, bool, jpt::String>;
    TVariant variant = jpt::String("Variant");
    
    any = variant;
    const TVariant& variant2 = any.As<TVariant>();
    JPT_ENSURE(variant2.Is<jpt::String>());
    JPT_ENSURE(variant2.As<jpt::String>() == "Variant");
    JPT_ENSURE(variant.Is<jpt::String>());
    JPT_ENSURE(variant.As<jpt::String>() == "Variant");
    JPT_ENSURE(&variant2 != &variant);

    return true;
}

bool UnitTest_Any_Move()
{
    jpt::String str1 = "Hello";
    jpt::String str2 = "World";

    // Move constructor
    jpt::Any any = Move(str1);
    JPT_ENSURE(any.As<jpt::String>() == "Hello");
    JPT_ENSURE(str1.IsEmpty());

    // Move assignment
    any = Move(str2);
    JPT_ENSURE(any.As<jpt::String>() == "World");
    JPT_ENSURE(str2.IsEmpty());

    // MoveAny
    jpt::Any any2;
    JPT_IGNORE(any2);

    // Move variant
    using TVariant = jpt::Variant<int32, char, bool, jpt::String>;
    TVariant variant = jpt::String("Variant");
    
    any = Move(variant);
    const TVariant& variant2 = any.As<TVariant>();
    JPT_ENSURE(variant2.Is<jpt::String>());
    JPT_ENSURE(variant2.As<jpt::String>() == "Variant");
    JPT_ENSURE(!variant.Is<jpt::String>());
    JPT_ENSURE(&variant2 != &variant);

    return true;
}

export bool RunUnitTests_Any()
{
    // Before: [Log] TimingProfiler: UnitTest_Any took 163.653300 ms
    // After:  [Log] TimingProfiler: UnitTest_Any took 125.973000 ms
    {
        JPT_SCOPED_TIMING_PROFILER(UnitTest_Any);
        for (size_t i = 0; i < 100000; ++i)
        {
            JPT_ENSURE(UnitTest_Any());
            JPT_ENSURE(UnitTest_Any_Copy());
            JPT_ENSURE(UnitTest_Any_Move());
        }
    }

    return true;
}
