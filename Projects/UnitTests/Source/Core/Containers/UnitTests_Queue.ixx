// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Queue;

import jpt.Queue;
import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_Queue()
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

export bool RunUnitTests_Queue()
{
    JPT_ENSURE(UnitTest_Queue());

    return true;
}
