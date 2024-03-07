// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Function;

import jpt.Function;
import jpt.TypeDefs;
import jpt.Utilities;

int32 Add(int32 a, int32 b)
{
	return a + b;
}

int32 Subtract(int32 a, int32 b)
{
	return a - b;
}

bool UnitTest_Function_Global()
{
    jpt::Function<int32(int32, int32)> func;

    func.Connect(&Add);
    JPT_ENSURE(func(1, 2) == 3);

    func.Connect(&Subtract);
    JPT_ENSURE(func(1, 2) == -1);

    return true;
}

bool UnitTest_Function_Lambda()
{
    // rvalue lambda
    jpt::Function<int32(int32, int32)> func;
    func.Connect([](int32 a, int32 b)
        {
            return a + b;
        });
    JPT_ENSURE(func(1, 2) == 3);

    func.Connect([](int32 a, int32 b)
        {
            return a - b;
        });
    JPT_ENSURE(func(1, 2) == -1);

    // lvalue lambda
    auto lambda1 = [](int32 a, int32 b)
        {
            return a * b;
        };
    auto lambda2 = [](int32 a, int32 b)
        {
            return a / b;
        };

    func.Connect(lambda1);
    JPT_ENSURE(func(2, 3) == 6);

    func.Connect(lambda2);
    JPT_ENSURE(func(6, 3) == 2);

    return true;
}

struct Test
{
    int32 m_value = 0;

    int32 Add(int32 a) { return m_value + a; }
    int32 Subtract(int32 a) { return m_value - a; }
};

bool UnitTest_Function_MemberFunction1()
{
	Test test(10);

    using TSignature = int32(Test::*)(int32);
    TSignature function = &Test::Add;

    JPT_LOG((test.*function)(2));
    JPT_LOG((Test().*function)(2));

    void* ptr = &test;
    JPT_LOG((static_cast<Test*>(ptr)->*function)(2));

	return true;
}

bool UnitTest_Function_MemberFunction()
{
    //jpt::Function<int32(int32)> func;

    //Test test(13);
    //func.Connect(&test, &Test::Add);

    //func.CallMember(1);

    return true;
}

export bool RunUnitTests_Function()
{
    JPT_ENSURE(UnitTest_Function_Global());
    JPT_ENSURE(UnitTest_Function_Lambda());
    JPT_ENSURE(UnitTest_Function_MemberFunction());
    JPT_ENSURE(UnitTest_Function_MemberFunction1());

    return true;
}
