// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Stack;

import jpt.Stack;
import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_Stack()
{
    jpt::Stack<int32> stack;

    for (int32 i = 0; i < 10; ++i)
	{
		stack.Push(i);
	}

    JPT_ENSURE(stack.Size() == 10);

    for (int32 i = 9; i >= 0; --i)
    {
        JPT_ENSURE(stack.Peek() == i);
        stack.Pop();
    }

    return true;
}

bool UnitTest_Stack_String()
{
    jpt::Stack<jpt::String> stack;

    stack.Emplace("Jupiter");
    stack.Emplace("Engine");
    stack.Emplace("By");
    stack.Emplace("Jupiter");
    stack.Emplace("Technologies");

    JPT_ENSURE(stack.Size() == 5);

    JPT_ENSURE(stack.Peek() == "Technologies"); stack.Pop();
    JPT_ENSURE(stack.Peek() == "Jupiter");      stack.Pop();
    JPT_ENSURE(stack.Peek() == "By");           stack.Pop();
    JPT_ENSURE(stack.Peek() == "Engine");       stack.Pop();
    JPT_ENSURE(stack.Peek() == "Jupiter");      stack.Pop();

    return true;
}

bool UnitTest_Stack_Copy()
{
    jpt::Stack<int32> stack;

    for (int32 i = 0; i < 10; ++i)
    {
        stack.Push(i);
    }

    jpt::Stack<int32> copy = stack;

    JPT_ENSURE(copy.Size() == 10);

    for (int32 i = 9; i >= 0; --i)
    {
        JPT_ENSURE(copy.Peek() == i);
        copy.Pop();
    }

    JPT_ENSURE(stack.Size() == 10);

    for (int32 i = 9; i >= 0; --i)
    {
        JPT_ENSURE(stack.Peek() == i);
        stack.Pop();
    }

    return true;
}

bool UnitTest_Stack_Copy_String()
{
    jpt::Stack<jpt::String> stack;

    stack.Emplace("Jupiter");
    stack.Emplace("Engine");
    stack.Emplace("By");
    stack.Emplace("Jupiter");
    stack.Emplace("Technologies");

    jpt::Stack<jpt::String> copy = stack;

    JPT_ENSURE(stack.Size() == 5);

    JPT_ENSURE(stack.Peek() == "Technologies"); stack.Pop();
    JPT_ENSURE(stack.Peek() == "Jupiter");      stack.Pop();
    JPT_ENSURE(stack.Peek() == "By");           stack.Pop();
    JPT_ENSURE(stack.Peek() == "Engine");       stack.Pop();
    JPT_ENSURE(stack.Peek() == "Jupiter");      stack.Pop();

    JPT_ENSURE(copy.Size() == 5);

    JPT_ENSURE(copy.Peek() == "Technologies"); copy.Pop();
    JPT_ENSURE(copy.Peek() == "Jupiter");      copy.Pop();
    JPT_ENSURE(copy.Peek() == "By");           copy.Pop();
    JPT_ENSURE(copy.Peek() == "Engine");       copy.Pop();
    JPT_ENSURE(copy.Peek() == "Jupiter");      copy.Pop();

    return true;
}

bool UnitTest_Stack_Move()
{
    jpt::Stack<int32> stack;

    for (int32 i = 0; i < 10; ++i)
    {
        stack.Push(i);
    }

    jpt::Stack<int32> moved = jpt::Move(stack);

    JPT_ENSURE(moved.Size() == 10);
    JPT_ENSURE(stack.IsEmpty());

    for (int32 i = 9; i >= 0; --i)
    {
        JPT_ENSURE(moved.Peek() == i);
        moved.Pop();
    }

    JPT_ENSURE(stack.IsEmpty());

    return true;
}

bool UnitTest_Stack_Move_String()
{
    jpt::Stack<jpt::String> stack;

    stack.Emplace("Jupiter");
    stack.Emplace("Engine");
    stack.Emplace("By");
    stack.Emplace("Jupiter");
    stack.Emplace("Technologies");

    jpt::Stack<jpt::String> moved = jpt::Move(stack);

    JPT_ENSURE(moved.Size() == 5);

    JPT_ENSURE(moved.Peek() == "Technologies"); moved.Pop();
    JPT_ENSURE(moved.Peek() == "Jupiter");      moved.Pop();
    JPT_ENSURE(moved.Peek() == "By");           moved.Pop();
    JPT_ENSURE(moved.Peek() == "Engine");       moved.Pop();
    JPT_ENSURE(moved.Peek() == "Jupiter");      moved.Pop();

    JPT_ENSURE(stack.IsEmpty());

    return true;
}

export bool RunUnitTests_Stack()
{
    JPT_ENSURE(UnitTest_Stack());
    JPT_ENSURE(UnitTest_Stack_String());
    
    JPT_ENSURE(UnitTest_Stack_Copy());
    JPT_ENSURE(UnitTest_Stack_Copy_String());
    
    JPT_ENSURE(UnitTest_Stack_Move());
    JPT_ENSURE(UnitTest_Stack_Move_String());

    return true;
}
