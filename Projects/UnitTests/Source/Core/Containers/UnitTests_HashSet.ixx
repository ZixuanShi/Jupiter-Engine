// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_HashSet;

import jpt.HashSet;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.String;

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

bool UnitTest_HashSet_String()
{
    jpt::HashSet<jpt::String> hashSet;

    hashSet.Add("Hello");
    hashSet.Add("World");
    hashSet.Add("Jupiter");

    JPT_ENSURE(hashSet.Contains("Hello"));
    JPT_ENSURE(hashSet.Contains("World"));
    JPT_ENSURE(hashSet.Contains("Jupiter"));
    JPT_ENSURE(!hashSet.Contains("Engine"));
    JPT_ENSURE(hashSet.Size() == 3);

    hashSet.Erase("World");

    JPT_ENSURE(hashSet.Contains("Hello"));
    JPT_ENSURE(!hashSet.Contains("World"));
    JPT_ENSURE(hashSet.Contains("Jupiter"));
    JPT_ENSURE(hashSet.Size() == 2);

    hashSet.Clear();

    JPT_ENSURE(!hashSet.Contains("Hello"));
    JPT_ENSURE(!hashSet.Contains("World"));
    JPT_ENSURE(!hashSet.Contains("Jupiter"));
    JPT_ENSURE(hashSet.IsEmpty());

    return true;
}

export bool RunUnitTests_HashSet()
{
    JPT_ENSURE(UnitTest_HashSet());
    JPT_ENSURE(UnitTest_HashSet_String());

    return true;
}
