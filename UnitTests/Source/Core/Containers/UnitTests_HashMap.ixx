// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <unordered_map>

export module UnitTests_HashMap;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Function;
import jpt.HashMap;

bool UnitTest_HashMap_Trivial()
{
    jpt::HashMap<char, int32> map;

    // Inserting
    map.Insert('a', 0);
    map.Insert('b', 1);
    map.Insert('c', 2);
    map['a'] = 3;
    map['d'] = 4;
    map['e'] = 5;

    JPT_RETURN_FALSE_IF_ERROR(map['a'] != 3, "");
    JPT_RETURN_FALSE_IF_ERROR(map['b'] != 1, "");
    JPT_RETURN_FALSE_IF_ERROR(map['c'] != 2, "");
    JPT_RETURN_FALSE_IF_ERROR(map['d'] != 4, "");
    JPT_RETURN_FALSE_IF_ERROR(map['e'] != 5, "");

    // Erasing
    map.Erase('e');

    // Searching
    JPT_RETURN_FALSE_IF_ERROR(!map.Contains('a'), "");
    JPT_RETURN_FALSE_IF_ERROR(map.Contains('e'), "");
    JPT_RETURN_FALSE_IF_ERROR(map.Find('a')->second != 3, "");
    JPT_RETURN_FALSE_IF_ERROR(map.Find('e') != map.end(), "");

    // Modifiying
    //JPT_LOG(map);
    map.Clear();
    //JPT_LOG(map);

    // Capacity
    JPT_RETURN_FALSE_IF_ERROR(!map.IsEmpty(), "");

    return true;
}

bool UnitTest_HashMap_Trivial_Copy()
{
    jpt::HashMap<char, int32> map{ {'a', 1},{'b', 2},{'c', 3} };
    JPT_RETURN_FALSE_IF_ERROR(map['a'] != 1, "");
    JPT_RETURN_FALSE_IF_ERROR(map['b'] != 2, "");
    JPT_RETURN_FALSE_IF_ERROR(map['c'] != 3, "");
    JPT_RETURN_FALSE_IF_ERROR(map.Size() != 3, "");

    jpt::HashMap<char, int32> otherMap(map);
    JPT_RETURN_FALSE_IF_ERROR(otherMap['a'] != 1, "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap['b'] != 2, "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap['c'] != 3, "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap.Size() != 3, "");

    jpt::HashMap<char, int32> otherMap2;
    otherMap2 = otherMap;
    JPT_RETURN_FALSE_IF_ERROR(otherMap2['a'] != 1, "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap2['b'] != 2, "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap2['c'] != 3, "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap2.Size() != 3, "");

    //JPT_LOG(map);
    //JPT_LOG(otherMap);
    //JPT_LOG(otherMap2);

    return true;
}

bool UnitTest_HashMap_Trivial_Move()
{
    jpt::HashMap<char, int32> map{ {'a', 1},{'b', 2},{'c', 3} };
    JPT_RETURN_FALSE_IF_ERROR(map['a'] != 1, "");
    JPT_RETURN_FALSE_IF_ERROR(map['b'] != 2, "");
    JPT_RETURN_FALSE_IF_ERROR(map['c'] != 3, "");
    JPT_RETURN_FALSE_IF_ERROR(map.Size() != 3, "");

    jpt::HashMap<char, int32> otherMap(jpt::Move(map));
    JPT_RETURN_FALSE_IF_ERROR(otherMap['a'] != 1, "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap['b'] != 2, "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap['c'] != 3, "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap.Size() != 3, "");
    JPT_RETURN_FALSE_IF_ERROR(!map.IsEmpty(), "");

    jpt::HashMap<char, int32> otherMap2;
    otherMap2 = jpt::Move(otherMap);
    JPT_RETURN_FALSE_IF_ERROR(otherMap2['a'] != 1, "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap2['b'] != 2, "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap2['c'] != 3, "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap2.Size() != 3, "");
    JPT_RETURN_FALSE_IF_ERROR(!otherMap.IsEmpty(), "");

    //JPT_LOG(map);
    //JPT_LOG(otherMap);
    //JPT_LOG(otherMap2);

    return true;
}

bool UnitTest_HashMap_NonTrivial()
{
    jpt::HashMap<jpt::String, jpt::String> map;

    // Inserting
    map.Insert("Champ A", "Aatrox");
    map.Insert("Champ B", "Darius");
    map.Insert("Champ C", "Jax");
    map["Champ A"] =  "Morde";
    map["Champ D"] =  "Nasus";
    map["Champ E"] =  "Zac";

    // Inserting
    JPT_RETURN_FALSE_IF_ERROR(map["Champ A"] != "Morde", "");
    JPT_RETURN_FALSE_IF_ERROR(map["Champ B"] != "Darius", "");
    JPT_RETURN_FALSE_IF_ERROR(map["Champ C"] != "Jax", "");
    JPT_RETURN_FALSE_IF_ERROR(map["Champ D"] != "Nasus", "");
    JPT_RETURN_FALSE_IF_ERROR(map["Champ E"] != "Zac", "");

    // Erasing
    map.Erase("Champ E");

    // Searching
    JPT_RETURN_FALSE_IF_ERROR(!map.Contains("Champ A"), "");
    JPT_RETURN_FALSE_IF_ERROR(map.Contains("Champ E"), "");
    JPT_RETURN_FALSE_IF_ERROR(map.Find("Champ A")->second != "Morde", "");
    JPT_RETURN_FALSE_IF_ERROR(map.Find("Champ E") != map.end(), "");

    // Modifiying
    //JPT_LOG(map);
    map.Clear();
    //JPT_LOG(map);

    // Capacity
    JPT_RETURN_FALSE_IF_ERROR(!map.IsEmpty(), "");

    return true;
}

bool UnitTest_HashMap_NonTrivial_Copy()
{
    jpt::HashMap<jpt::String, jpt::String> map{ {"Champ A", "Aatrox"},{"Champ B","Jax"},{"Champ C", "Darius"} };
    JPT_RETURN_FALSE_IF_ERROR(map["Champ A"] != "Aatrox", "");
    JPT_RETURN_FALSE_IF_ERROR(map["Champ B"] != "Jax"   , "");
    JPT_RETURN_FALSE_IF_ERROR(map["Champ C"] != "Darius", "");
    JPT_RETURN_FALSE_IF_ERROR(map.Size() != 3, "");

    jpt::HashMap<jpt::String, jpt::String> otherMap(map);
    JPT_RETURN_FALSE_IF_ERROR(otherMap["Champ A"] != "Aatrox", "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap["Champ B"] != "Jax"   , "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap["Champ C"] != "Darius", "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap.Size() != 3, "");

    jpt::HashMap<jpt::String, jpt::String> otherMap2;
    otherMap2 = otherMap;
    JPT_RETURN_FALSE_IF_ERROR(otherMap2["Champ A"] != "Aatrox", "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap2["Champ B"] != "Jax"   , "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap2["Champ C"] != "Darius", "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap2.Size() != 3, "");

    //JPT_LOG(map);
    //JPT_LOG(otherMap);
    //JPT_LOG(otherMap2);

    return true;
}

bool UnitTest_HashMap_NonTrivial_Move()
{
    jpt::HashMap<jpt::String, jpt::String> map{ {"Champ A", "Aatrox"},{"Champ B","Jax"},{"Champ C", "Darius"} };
    JPT_RETURN_FALSE_IF_ERROR(map["Champ A"] != "Aatrox", "");
    JPT_RETURN_FALSE_IF_ERROR(map["Champ B"] != "Jax", "");
    JPT_RETURN_FALSE_IF_ERROR(map["Champ C"] != "Darius", "");
    JPT_RETURN_FALSE_IF_ERROR(map.Size() != 3, "");

    jpt::HashMap<jpt::String, jpt::String> otherMap(jpt::Move(map));
    JPT_RETURN_FALSE_IF_ERROR(otherMap["Champ A"] != "Aatrox", "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap["Champ B"] != "Jax"   , "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap["Champ C"] != "Darius", "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap.Size() != 3, "");

    jpt::HashMap<jpt::String, jpt::String> otherMap2;
    otherMap2 = jpt::Move(otherMap);
    JPT_RETURN_FALSE_IF_ERROR(otherMap2["Champ A"] != "Aatrox", "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap2["Champ B"] != "Jax"   , "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap2["Champ C"] != "Darius", "");
    JPT_RETURN_FALSE_IF_ERROR(otherMap2.Size() != 3, "");

    //JPT_LOG(map);
    //JPT_LOG(otherMap);
    //JPT_LOG(otherMap2);

    return true;
}

void Func(int32& num)
{
    ++num;
}

bool UnitTest_HashMap_Function()
{
    auto lambda = [](int32& num)->void
        {
            num += 2;
        };
    
    jpt::HashMap<jpt::String, jpt::Function<void(int32&)>> map;
    map["Func1"] = Func;
    map["Func2"] = lambda;

    int32 i = 0;

    map["Func1"](i);
    JPT_RETURN_FALSE_IF_ERROR(i != 1, "");
    
    map["Func2"](i);
    JPT_RETURN_FALSE_IF_ERROR(i != 3, "");

    return true;
}

export bool RunUnitTests_HashMap()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_HashMap_Trivial(), "UnitTest_HashMap_Trivial Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_HashMap_Trivial_Copy(), "UnitTest_HashMap_Trivial_Copy Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_HashMap_Trivial_Move(), "UnitTest_HashMap_Trivial_Move Failed");

    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_HashMap_NonTrivial(), "UnitTest_HashMap_Trivial Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_HashMap_NonTrivial_Copy(), "UnitTest_HashMap_NonTrivial_Copy Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_HashMap_NonTrivial_Move(), "UnitTest_HashMap_NonTrivial_Move Failed");
    
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_HashMap_Function(), "UnitTest_HashMap_Function Failed");

    return true;
}
