// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Queue;

import jpt.Queue;
import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTests_Queue()
{
    jpt::Queue<int32> queue;

    for (int32 i = 0; i < 10; ++i)
    {
        queue.Enqueue(i);
    }

    JPT_ENSURE(queue.Count() == 10);

    for (int32 i = 0; i < 10; ++i)
    {
        JPT_ENSURE(queue.Front() == i); queue.Dequeue();
    }

    return true;
}

bool UnitTests_Queue_String()
{
    jpt::Queue<jpt::String> queue;

    queue.Enqueue("Jupiter");
    queue.Enqueue("Engine");
    queue.Enqueue("By");
    queue.Enqueue("Jupiter");
    queue.Enqueue("Technologies");

    JPT_ENSURE(queue.Count() == 5);

    JPT_ENSURE(queue.Front() == "Jupiter");      queue.Dequeue();
    JPT_ENSURE(queue.Front() == "Engine");       queue.Dequeue();
    JPT_ENSURE(queue.Front() == "By");           queue.Dequeue();
    JPT_ENSURE(queue.Front() == "Jupiter");      queue.Dequeue();
    JPT_ENSURE(queue.Front() == "Technologies"); queue.Dequeue();

    return true;
}

bool UnitTests_Queue_Copy()
{
    jpt::Queue<int32> queue;

    for (int32 i = 0; i < 10; ++i)
    {
        queue.Enqueue(i);
    }

    jpt::Queue<int32> copy = queue;

    JPT_ENSURE(copy.Count() == 10);

    for (int32 i = 0; i < 10; ++i)
    {
        JPT_ENSURE(copy.Front() == i); copy.Dequeue();
    }

    JPT_ENSURE(queue.Count() == 10);

    for (int32 i = 0; i < 10; ++i)
    {
        JPT_ENSURE(queue.Front() == i); queue.Dequeue();
    }

    return true;
}

bool UnitTests_Queue_Copy_String()
{
    jpt::Queue<jpt::String> queue;

    queue.Enqueue("Jupiter");
    queue.Enqueue("Engine");
    queue.Enqueue("By");
    queue.Enqueue("Jupiter");
    queue.Enqueue("Technologies");

    jpt::Queue<jpt::String> copy = queue;

    JPT_ENSURE(queue.Count() == 5);

    JPT_ENSURE(queue.Front() == "Jupiter");      queue.Dequeue();
    JPT_ENSURE(queue.Front() == "Engine");       queue.Dequeue();
    JPT_ENSURE(queue.Front() == "By");           queue.Dequeue();
    JPT_ENSURE(queue.Front() == "Jupiter");      queue.Dequeue();
    JPT_ENSURE(queue.Front() == "Technologies"); queue.Dequeue();

    JPT_ENSURE(copy.Count() == 5);

    JPT_ENSURE(copy.Front() == "Jupiter");      copy.Dequeue();
    JPT_ENSURE(copy.Front() == "Engine");       copy.Dequeue();
    JPT_ENSURE(copy.Front() == "By");           copy.Dequeue();
    JPT_ENSURE(copy.Front() == "Jupiter");      copy.Dequeue();
    JPT_ENSURE(copy.Front() == "Technologies"); copy.Dequeue();

    return true;
}

bool UnitTests_Queue_Move()
{
    jpt::Queue<int32> queue;

    for (int32 i = 0; i < 10; ++i)
    {
        queue.Enqueue(i);
    }

    jpt::Queue<int32> moved = jpt::Move(queue);

    JPT_ENSURE(moved.Count() == 10);

    for (int32 i = 0; i < 10; ++i)
    {
        JPT_ENSURE(moved.Front() == i); moved.Dequeue();
    }

    JPT_ENSURE(queue.IsEmpty());

    return true;
}

bool UnitTests_Queue_Move_String()
{
    jpt::Queue<jpt::String> queue;

    queue.Enqueue("Jupiter");
    queue.Enqueue("Engine");
    queue.Enqueue("By");
    queue.Enqueue("Jupiter");
    queue.Enqueue("Technologies");

    jpt::Queue<jpt::String> moved = jpt::Move(queue);

    JPT_ENSURE(moved.Count() == 5);

    JPT_ENSURE(moved.Front() == "Jupiter");      moved.Dequeue();
    JPT_ENSURE(moved.Front() == "Engine");       moved.Dequeue();
    JPT_ENSURE(moved.Front() == "By");           moved.Dequeue();
    JPT_ENSURE(moved.Front() == "Jupiter");      moved.Dequeue();
    JPT_ENSURE(moved.Front() == "Technologies"); moved.Dequeue();

    JPT_ENSURE(queue.IsEmpty());

    return true;
}

export bool RunUnitTests_Queue()
{
    JPT_ENSURE(UnitTests_Queue());
    JPT_ENSURE(UnitTests_Queue_String());

    JPT_ENSURE(UnitTests_Queue_Copy());
    JPT_ENSURE(UnitTests_Queue_Copy_String());

    JPT_ENSURE(UnitTests_Queue_Move());
    JPT_ENSURE(UnitTests_Queue_Move_String());

    return true;
}
