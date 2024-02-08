// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_HashMap;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.HashMap;

bool UnitTest_HashMap_Trivial()
{
    jpt::HashMap<char, int32> map;

    map.Insert('a', 0);
    map.Insert('b', 1);
    map.Insert('c', 2);
    map.Insert('a', 3);
    map.Insert('d', 4);
    map.Insert('e', 5);

    for (auto itr = map.begin(); itr != map.end(); ++itr)
    {
        JPT_LOG(*itr);
    }

    return true;
}

bool UnitTest_HashMap_NonTrivial()
{
    jpt::HashMap<jpt::String, jpt::String> map;

    map.Insert("Champ: A", "Aatrox");
    map.Insert("Champ: B", "Darius");
    map.Insert("Champ: C", "Jax");
    map.Insert("Champ: A", "Morde");
    map.Insert("Champ: D", "Nasus");
    map.Insert("Champ: D", "Zac");

    for (auto itr = map.begin(); itr != map.end(); ++itr)
    {
        JPT_LOG(*itr);
    }

    return true;
}

export bool RunUnitTests_HashMap()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_HashMap_Trivial(), "UnitTest_HashMap_Trivial Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_HashMap_NonTrivial(), "UnitTest_HashMap_Trivial Failed");

    return true;
}
