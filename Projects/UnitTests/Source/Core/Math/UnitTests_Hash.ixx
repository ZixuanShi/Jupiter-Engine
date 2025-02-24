// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Hash;

import jpt.Hash;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTests_Hash_Primitive()
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
};

namespace jpt
{
    template<>
    constexpr uint64 Hash<TrivialStruct>(const TrivialStruct& object)
    {
        // Combine the hash values of the struct members
        return jpt::Hash(object.a) ^ jpt::Hash(object.b);
    }
}

//template<>
//constexpr bool jpt::IsTrivial<TrivialStruct> = false;

bool UnitTests_Hash_TrivialStruct()
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
};

namespace jpt
{
    template<>
    constexpr uint64 Hash<NonTrivialStruct>(const NonTrivialStruct& object)
    {
        // Combine the hash values of the struct members
        return jpt::Hash(object.a) ^ jpt::Hash(object.b) ^ jpt::Hash(object.c) ^ jpt::Hash(object.d) ^ jpt::Hash(object.e);
    }
}

//template<> constexpr bool jpt::IsTrivial<NonTrivialStruct> = true;
//template<> constexpr bool jpt::IsSmall<NonTrivialStruct> = true;
bool UnitTests_Hash_NonTrivialStruct()
{
	NonTrivialStruct testStruct = { 42, 41, 42.0415f, 42.0415, "Hello" };

	jpt::Hash(testStruct);

	return true;
}

export bool RunUnitTests_Hash()
{
    JPT_ENSURE(UnitTests_Hash_Primitive());
    JPT_ENSURE(UnitTests_Hash_TrivialStruct());
    JPT_ENSURE(UnitTests_Hash_NonTrivialStruct());

    return true;
}
