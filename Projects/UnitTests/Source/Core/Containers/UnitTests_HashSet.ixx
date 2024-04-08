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

    hashSet.Insert(1);
    hashSet.Insert(2);
    hashSet.Insert(3);

    return true;
}

export bool RunUnitTests_HashSet()
{
    JPT_ENSURE(UnitTest_HashSet());

    return true;
}
