// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

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

bool UnitTests_Function_Global()
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

struct Test
{
    jpt::Function<bool(int32)> m_func;
    jpt::Function<int32(int32)> m_func2;
    int32 m_value = 0;

    Test(int32 value) 
        : m_value(value) 
    {
        m_func.Connect([this](int32 a) 
            {
                return IsEven(a); 
            });

        m_func2.Connect([this](int32 a)
            {
                m_value += a;
                return m_value;
            });
    }

    int32 Add(int32 a) 
    { 
        m_value += a;
        return m_value;
    }
    int32 Subtract(int32 a) 
    {
        m_value -= a;
        return m_value;
    }
    bool IsEven(int32 a) 
    { 
        return a % 2 == 0; 
    }
};

bool UnitTests_Function_MemberFunction()
{
    jpt::Function<int32(int32)> func;
    Test test(13);

    func.Connect(&test, &Test::Add);
    JPT_ENSURE(func(2) == 15);

    func.Connect(&test, &Test::Subtract);
    JPT_ENSURE(func(10) == 5);

    JPT_ENSURE(test.m_func(2) == true);
    JPT_ENSURE(test.m_func(3) == false);
    JPT_ENSURE(test.m_func(4) == true);

    test.m_func.Connect(&test, &Test::IsEven);
    JPT_ENSURE(test.m_func(2) == true);
    JPT_ENSURE(test.m_func(3) == false);
    JPT_ENSURE(test.m_func(4) == true);

    return true;
}

bool UnitTests_Function_Functor()
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

bool UnitTests_Function_Copy()
{
    jpt::Function<int32(int32, int32)> func1;
    func1.Connect(&Add);

    jpt::Function<int32(int32, int32)> func2(func1);
    JPT_ENSURE(func1(1, 2) == 3);
    JPT_ENSURE(func2(1, 2) == 3);

    Test test(13);
    jpt::Function<int32(int32)> func3;
    func3.Connect(&test, &Test::Add);

    jpt::Function<int32(int32)> func4(func3);
    JPT_ENSURE(func3(2) == 15);
    JPT_ENSURE(func4(2) == 17);

    func3.Connect(&test, &Test::Subtract);
    JPT_ENSURE(func3(2) == 15);
    JPT_ENSURE(func4(2) == 17);

    return true;
}

bool UnitTests_Function_Lambda()
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

void LambdaCapturesLocalVar(jpt::Function<int32()>& outLambda)
{
    int32 x = 10;
    outLambda = [x]() { return x; };
    //outLambda = [&x]() { return x; }; // Error: 'x' cannot be captured because it does not have automatic storage duration
}
jpt::Function<int32()> LambdaCapturesLocalVar()
{
    int32 x = 10;
    return [x]() { return x; };
}
bool UnitTests_Function_LambdaCapturesLocalVar()
{
    jpt::Function<int32()> lambda;
    LambdaCapturesLocalVar(lambda);
    JPT_ENSURE(lambda() == 10);
    JPT_ENSURE(lambda() == 10);
    JPT_ENSURE(LambdaCapturesLocalVar()() == 10);
    return true;
}

bool UnitTests_Function_LambdaCapturesThis()
{
	Test test(13);
	JPT_ENSURE(test.m_func2(2) == 15);
    JPT_ENSURE(test.m_value == 15);

    JPT_ENSURE(test.m_func2(3) == 18);
    JPT_ENSURE(test.m_value == 18);

    JPT_ENSURE(test.m_func2(4) == 22);
    JPT_ENSURE(test.m_value == 22);

	return true;
}

void VoidFunction()
{
    JPT_LOG("Hello Void Func");
}
bool UnitTests_Function_Void()
{
    jpt::Function<void()> func;
    func.Connect(&VoidFunction);
    //func();

    return true;
}

export bool RunUnitTests_Function()
{
    JPT_ENSURE(UnitTests_Function_Global());
    JPT_ENSURE(UnitTests_Function_MemberFunction());
    JPT_ENSURE(UnitTests_Function_Functor());
    JPT_ENSURE(UnitTests_Function_Copy());

    JPT_ENSURE(UnitTests_Function_Lambda());
    JPT_ENSURE(UnitTests_Function_LambdaCapturesLocalVar());
    JPT_ENSURE(UnitTests_Function_LambdaCapturesThis());

    JPT_ENSURE(UnitTests_Function_Void());

    return true;
}
