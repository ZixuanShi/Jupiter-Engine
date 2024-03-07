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

export bool RunUnitTests_Function()
{
    JPT_ENSURE(UnitTest_Function_Global());
    JPT_ENSURE(UnitTest_Function_Lambda());

    return true;
}
