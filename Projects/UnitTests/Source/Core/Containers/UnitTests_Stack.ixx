// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Stack;

import jpt.Stack;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_Stack()
{
    jpt::Stack<int32> stack;

    for (int32 i = 0; i < 10; ++i)
	{
		stack.Add(i);
	}

    JPT_ENSURE(stack.Size() == 10);

    for (int32 i = 9; i >= 0; --i)
    {
        JPT_ENSURE(stack.Peek() == i);
        stack.Pop();
    }

    return true;
}

export bool RunUnitTests_Stack()
{
    JPT_ENSURE(UnitTest_Stack());

    return true;
}
