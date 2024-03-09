// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <typeinfo>

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

    func = &Add;
    JPT_ENSURE(func(2, 3) == 5);

    func = &Subtract;
    JPT_ENSURE(func(2, 3) == -1);

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

    func = [](int32 a, int32 b)
        {
			return a * b;
		};
    JPT_ENSURE(func(2, 3) == 6);

    func = [](int32 a, int32 b)
        {
			return a / b;
		};
    JPT_ENSURE(func(6, 3) == 2);

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

    func = lambda1;
    JPT_ENSURE(func(2, 3) == 6);

    func = lambda2;
    JPT_ENSURE(func(6, 3) == 2);

    return true;
}

struct Test
{
    jpt::Function<bool(int32)> m_func;
    int32 m_value = 0;

    Test(int32 value) 
        : m_value(value) 
    {
        m_func.Connect([this](int32 a) { return IsEven(a); });
    }

    int32 Add(int32 a) { return m_value + a; }
    int32 Subtract(int32 a) { return m_value - a; }
    bool IsEven(int32 a) { return a % 2 == 0; }
};

bool UnitTest_Function_MemberFunction()
{
    jpt::Function<int32(int32)> func;
    Test test(13);

    func.Connect(&test, &Test::Add);
    JPT_ENSURE(func(2) == 15);

    func.Connect(&test, &Test::Subtract);
    JPT_ENSURE(func(2) == 11);

    JPT_ENSURE(test.m_func(2) == true);
    JPT_ENSURE(test.m_func(3) == false);
    JPT_ENSURE(test.m_func(4) == true);

    test.m_func.Connect(&test, &Test::IsEven);
    JPT_ENSURE(test.m_func(2) == true);
    JPT_ENSURE(test.m_func(3) == false);
    JPT_ENSURE(test.m_func(4) == true);

    return true;
}

void VoidFunction()
{
	JPT_LOG("Hello Void Func");
}

void SetFunc(jpt::Function<void()>& foo)
{
    int32 i = 42;
    auto lambda = [&i]()
        {
            JPT_LOG("Hello %d", i);
            i *= 2;
        };

    foo.Connect(lambda);
}

bool RunUnitTests_Function_Void()
{
    jpt::Function<void()> foo;
    //SetFunc(foo); // TODO: Fix this

    {
        int32 i = 42;
        auto lambda = [&i]()
		{
			//JPT_LOG("Hello %d", i);
			i *= 2;
		};

        foo.Connect(lambda);
    }

    foo();  // Still prints "Hello 42"
    foo();  // Still prints "Hello 84"

    return true;
}

bool RunUnitTests_Function_Functor()
{
    struct Functor
	{
		int32 operator()(int32 a, int32 b) { return a + b; }
	};

    jpt::Function<int32(int32, int32)> func;
    Functor functor;

    func.Connect(&functor, &Functor::operator());
    JPT_ENSURE(func(1, 2) == 3);

    return true;
}

export bool RunUnitTests_Function()
{
    JPT_ENSURE(UnitTest_Function_Global());
    JPT_ENSURE(UnitTest_Function_Lambda());
    JPT_ENSURE(UnitTest_Function_MemberFunction());
    JPT_ENSURE(RunUnitTests_Function_Void());
    JPT_ENSURE(RunUnitTests_Function_Functor());

    return true;
}
