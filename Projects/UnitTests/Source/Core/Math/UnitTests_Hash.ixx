// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Hash;

import jpt.Hash;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_Hash_Primitive()
{
    (jpt::Hash(true));            // bool
    (jpt::Hash(false));           // bool
    (jpt::Hash('A'));             // char
    (jpt::Hash('a'));             // char
    (jpt::Hash('B'));             // char
    (jpt::Hash(42));              // int32
    (jpt::Hash(41));              // int32
    (jpt::Hash(42U));             // uint32
    (jpt::Hash(42.0415f));        // float
    (jpt::Hash(42.0415));         // double
    (jpt::Hash("Hello"));         // const char*
    (jpt::Hash("Hello World"));   // const char*

    return true;
}
struct TrivialStruct
{
    int32 a;
    int32 b;

    uint64 Hash() const
    {
        return jpt::Hash(a) ^ jpt::Hash(b);
    }
};
//template<>
//constexpr bool jpt::IsTrivial<TrivialStruct> = false;

bool UnitTest_Hash_TrivialStruct()
{
    TrivialStruct testStruct = { 42, 41 };

    jpt::Hash(testStruct);

    return true;
}

struct NonTrivialStruct
{
    int32 a;
    int32 b;
    float c;
    double d;
    const char* e;

    uint64 Hash() const
    {
        // Combine the hash values of the struct members
        return jpt::Hash(a) ^ jpt::Hash(b) ^ jpt::Hash(c) ^ jpt::Hash(d) ^ jpt::Hash(e);
    }
};
//template<> constexpr bool jpt::IsTrivial<NonTrivialStruct> = true;
//template<> constexpr bool jpt::IsSmall<NonTrivialStruct> = true;
bool UnitTest_Hash_NonTrivialStruct()
{
	NonTrivialStruct testStruct = { 42, 41, 42.0415f, 42.0415, "Hello" };

	jpt::Hash(testStruct);

	return true;
}

export bool RunUnitTests_Hash()
{
    JPT_ENSURE(UnitTest_Hash_Primitive());
    JPT_ENSURE(UnitTest_Hash_TrivialStruct());
    JPT_ENSURE(UnitTest_Hash_NonTrivialStruct());

    return true;
}
