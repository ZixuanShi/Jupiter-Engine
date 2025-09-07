// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"
#include "Core/Types/Enum.h"

export module UnitTests_StaticHashMap;

import jpt.StaticHashMap;
import jpt.String;
import jpt.HashMap;
import jpt.StaticArray;
import jpt.TypeDefs;

//--------------------------------------------------------------------------------------------------------------
// Data
//--------------------------------------------------------------------------------------------------------------
static const jpt::StaticArray<int32, 16> kKeys = 
{
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 9, 10,11,12,13,14,15
};

static const jpt::StaticArray<jpt::String, 16> kValues = 
{
    "Zero", "One", "Two", "Three",
    "Four", "Five", "Six", "Seven",
    "Eight", "Nine", "Ten", "Eleven",
    "Twelve", "Thirteen", "Fourteen", "Fifteen"
};

//--------------------------------------------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------------------------------------------
bool Common()
{
    jpt::StaticHashMap<int32, jpt::String, 16> map;

    // Adding
    for (int32 i = 0; i < 16; ++i)
    {
        map.Add(kKeys[i], kValues[i]);
    }

    JPT_ENSURE(map.Count() == 16);
    JPT_ENSURE(!map.IsEmpty());
    JPT_ENSURE(map.IsFull());

    // Searching
    for (int32 i = 0; i < 16; ++i)
    {
        JPT_ENSURE(map.Has(kKeys[i]));
        JPT_ENSURE(map[kKeys[i]] == kValues[i]);
    }

    JPT_ENSURE(!map.Has(16));

    // Erasing
    map.Erase(5);
    map.Erase(11);
    JPT_ENSURE(map.Count() == 14);
    for (int32 i = 0; i < 16; ++i)
    {
        if (i == 5 || i == 11)
        {
            JPT_ENSURE(!map.Has(i));
        }
        else
        {
            JPT_ENSURE(map.Has(kKeys[i]));
            JPT_ENSURE(map[kKeys[i]] == kValues[i]);
        }
    }

    // Re-add
    map.Add(5, "Five");
    map.Add(11, "Eleven");
    JPT_ENSURE(map.Count() == 16);
    JPT_ENSURE(map.IsFull());
    for (int32 i = 0; i < 16; ++i)
    {
        JPT_ENSURE(map.Has(kKeys[i]));
        JPT_ENSURE(map[kKeys[i]] == kValues[i]);
    }

    // Clear
    map.Clear();
    JPT_ENSURE(map.IsEmpty());
    JPT_ENSURE(map.Count() == 0);
    for (int32 i = 0; i < 16; ++i)
    {
        JPT_ENSURE(!map.Has(kKeys[i]));
    }

    return true;
}

JPT_ENUM_UINT8(Color,
    Red,
    Green,
    Blue,
    Yellow,
    Orange,
    Cyan,

    Num
);
JPT_ENUM_HASH(Color);
JPT_ENUM_TO_STRING(Color);

bool Enum()
{
    jpt::StaticHashMap<Color, jpt::String, Color::Num> map;

    // Adding
    for (Color c = 0; c < Color::Num; ++c)
    {
        map.Add(c, jpt::ToString(c));
    }
    JPT_ENSURE(map.Count() == 6);
    JPT_ENSURE(!map.IsEmpty());
    JPT_ENSURE(map.IsFull());

    // Searching
    for (Color c = 0; c < Color::Num; ++c)
    {
        JPT_ENSURE(map.Has(c));
        JPT_ENSURE(map[c] == jpt::ToString(c));
    }

    // Erasing
    map.Erase(Color::Green);
    map.Erase(Color::Cyan);
    JPT_ENSURE(map.Count() == 4);
    for (Color c = 0; c < Color::Num; ++c)
    {
        if (c == Color::Green || c == Color::Cyan)
        {
            JPT_ENSURE(!map.Has(c));
        }
        else
        {
            JPT_ENSURE(map.Has(c));
            JPT_ENSURE(map[c] == jpt::ToString(c));
        }
    }

    // Re-add
    map.Add(Color::Green, Color::Name(Color::Green));
    map.Add(Color::Cyan, Color::Name(Color::Cyan));

    JPT_ENSURE(map.Count() == 6);
    JPT_ENSURE(map.IsFull());
    for (Color c = 0; c < Color::Num; ++c)
    {
        JPT_ENSURE(map.Has(c));
        JPT_ENSURE(map[c] == jpt::ToString(c));
    }

    // Clear
    map.Clear();
    JPT_ENSURE(map.IsEmpty());
    JPT_ENSURE(map.Count() == 0);
    for (Color c = 0; c < Color::Num; ++c)
    {
        JPT_ENSURE(!map.Has(c));
    }

    return true;
}

export bool RunUnitTests_StaticHashMap()
{
    JPT_ENSURE(Common());
    JPT_ENSURE(Enum());

    return true;
}
