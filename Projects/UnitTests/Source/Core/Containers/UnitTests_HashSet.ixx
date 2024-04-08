// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_HashSet;

import jpt.HashSet;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_HashSet()
{
    jpt::HashSet<int32> hashSet;

    hashSet.Add(1);
    hashSet.Add(2);
    hashSet.Add(3);

    JPT_ENSURE(hashSet.Contains(1));
    JPT_ENSURE(hashSet.Contains(2));
    JPT_ENSURE(hashSet.Contains(3));
    JPT_ENSURE(!hashSet.Contains(4));
    JPT_ENSURE(hashSet.Size() == 3);

    hashSet.Erase(2);

    JPT_ENSURE(hashSet.Contains(1));
    JPT_ENSURE(!hashSet.Contains(2));
    JPT_ENSURE(hashSet.Contains(3));
    JPT_ENSURE(hashSet.Size() == 2);

    hashSet.Clear();

    JPT_ENSURE(!hashSet.Contains(1));
    JPT_ENSURE(!hashSet.Contains(2));
    JPT_ENSURE(!hashSet.Contains(3));
    JPT_ENSURE(hashSet.IsEmpty());

    return true;
}

export bool RunUnitTests_HashSet()
{
    JPT_ENSURE(UnitTest_HashSet());

    return true;
}
