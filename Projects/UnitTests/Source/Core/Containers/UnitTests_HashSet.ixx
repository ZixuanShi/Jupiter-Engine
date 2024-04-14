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
    JPT_ENSURE(hashSet.Count() == 3);

    hashSet.Erase(2);

    JPT_ENSURE(hashSet.Contains(1));
    JPT_ENSURE(!hashSet.Contains(2));
    JPT_ENSURE(hashSet.Contains(3));
    JPT_ENSURE(hashSet.Count() == 2);

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
    JPT_ENSURE(hashSet.Count() == 3);

    hashSet.Erase("World");

    JPT_ENSURE(hashSet.Contains("Hello"));
    JPT_ENSURE(!hashSet.Contains("World"));
    JPT_ENSURE(hashSet.Contains("Jupiter"));
    JPT_ENSURE(hashSet.Count() == 2);

    hashSet.Clear();

    JPT_ENSURE(!hashSet.Contains("Hello"));
    JPT_ENSURE(!hashSet.Contains("World"));
    JPT_ENSURE(!hashSet.Contains("Jupiter"));
    JPT_ENSURE(hashSet.IsEmpty());

    return true;
}

bool UnitTest_HashSet_Copy()
{
    jpt::HashSet<int32> hashSet{ 0,1,2,3 };

    hashSet.Add(4);
    hashSet.Add(5);

    jpt::HashSet<int32> copy = hashSet;

    for (int32 i = 0; i < 6; ++i)
	{
		JPT_ENSURE(hashSet.Contains(i));
		JPT_ENSURE(copy.Contains(i));
	}

    copy.Erase(0);
    copy.Erase(1);

    JPT_ENSURE(!copy.Contains(0));
    JPT_ENSURE(!copy.Contains(1));

    for (int32 i = 2; i < 6; ++i)
    {
        JPT_ENSURE(copy.Contains(i));
    }

    for (int32 i = 0; i < 6; ++i)
    {
        JPT_ENSURE(hashSet.Contains(i));
    }

    return true;
}

bool UnitTest_HashSet_Copy_String()
{
    jpt::HashSet<jpt::String> hashSet{ "Hello", "World", "Jupiter" };

	hashSet.Add("Engine");
	hashSet.Add("Game");

	jpt::HashSet<jpt::String> copy = hashSet;

	JPT_ENSURE(hashSet.Contains("Hello"));
	JPT_ENSURE(hashSet.Contains("World"));
	JPT_ENSURE(hashSet.Contains("Jupiter"));
	JPT_ENSURE(hashSet.Contains("Engine"));
	JPT_ENSURE(hashSet.Contains("Game"));

	JPT_ENSURE(copy.Contains("Hello"));
	JPT_ENSURE(copy.Contains("World"));
	JPT_ENSURE(copy.Contains("Jupiter"));
	JPT_ENSURE(copy.Contains("Engine"));
	JPT_ENSURE(copy.Contains("Game"));

	copy.Erase("Hello");
	copy.Erase("World");

	JPT_ENSURE(!copy.Contains("Hello"));
	JPT_ENSURE(!copy.Contains("World"));

	JPT_ENSURE(copy.Contains("Jupiter"));
	JPT_ENSURE(copy.Contains("Engine"));
	JPT_ENSURE(copy.Contains("Game"));

	JPT_ENSURE(hashSet.Contains("Hello"));
	JPT_ENSURE(hashSet.Contains("World"));
	JPT_ENSURE(hashSet.Contains("Jupiter"));
	JPT_ENSURE(hashSet.Contains("Engine"));
	JPT_ENSURE(hashSet.Contains("Game"));

	return true;
}

bool UnitTest_HashSet_CopyAssign()
{
    jpt::HashSet<int32> hashSet{ 0,1,2,3 };
    jpt::HashSet<int32> copy{ 4,5,6,7 };

    copy = hashSet;

    for (int32 i = 0; i < 4; ++i)
    {
        JPT_ENSURE(hashSet.Contains(i));
		JPT_ENSURE(copy.Contains(i));
    }

    return true;
}

bool UnitTest_HashSet_CopyAssign_String()
{
    jpt::HashSet<jpt::String> hashSet{ "Hello", "World", "Jupiter" };
    jpt::HashSet<jpt::String> copy{ "Engine", "Game" };

    copy = hashSet;

    JPT_ENSURE(copy.Contains("Hello"));
    JPT_ENSURE(copy.Contains("World"));
    JPT_ENSURE(copy.Contains("Jupiter"));
    JPT_ENSURE(!copy.Contains("Engine"));
    JPT_ENSURE(!copy.Contains("Game"));

    JPT_ENSURE(hashSet.Contains("Hello"));
    JPT_ENSURE(hashSet.Contains("World"));
    JPT_ENSURE(hashSet.Contains("Jupiter"));
    JPT_ENSURE(!hashSet.Contains("Engine"));
    JPT_ENSURE(!hashSet.Contains("Game"));

    return true;
}

bool UnitTest_HashSet_Move()
{
    jpt::HashSet<int32> hashSet{ 0,1,2,3 };

    hashSet.Add(4);
    hashSet.Add(5);

    jpt::HashSet<int32> moved = jpt::Move(hashSet);

    for (int32 i = 0; i < 6; ++i)
	{
		JPT_ENSURE(moved.Contains(i));
	}

    JPT_ENSURE(hashSet.IsEmpty());

    moved.Erase(0);

    JPT_ENSURE(!moved.Contains(0));

    for (int32 i = 1; i < 6; ++i)
	{
		JPT_ENSURE(moved.Contains(i));
	}

    return true;
}

bool UnitTest_HashSet_Move_String()
{
    jpt::HashSet<jpt::String> hashSet{ "Hello", "World", "Jupiter" };

    hashSet.Add("Engine");
    hashSet.Add("Game");

    jpt::HashSet<jpt::String> moved = jpt::Move(hashSet);

    JPT_ENSURE(moved.Contains("Hello"));
    JPT_ENSURE(moved.Contains("World"));
    JPT_ENSURE(moved.Contains("Jupiter"));
    JPT_ENSURE(moved.Contains("Engine"));
    JPT_ENSURE(moved.Contains("Game"));

    JPT_ENSURE(hashSet.IsEmpty());

    return true;
}

bool UnitTest_HashSet_MoveAssign()
{
    jpt::HashSet<int32> hashSet{ 0,1,2,3 };
    jpt::HashSet<int32> moved{ 4,5,6,7 };

    moved = jpt::Move(hashSet);

    for (int32 i = 0; i < 4; ++i)
    {
        JPT_ENSURE(moved.Contains(i));
    }

    JPT_ENSURE(hashSet.IsEmpty());

    return true;
}

bool UnitTest_HashSet_MoveAssign_String()
{
    jpt::HashSet<jpt::String> hashSet{ "Hello", "World", "Jupiter" };
    jpt::HashSet<jpt::String> moved{ "Engine", "Game" };

    moved = jpt::Move(hashSet);

    JPT_ENSURE(moved.Contains("Hello"));
    JPT_ENSURE(moved.Contains("World"));
    JPT_ENSURE(moved.Contains("Jupiter"));
    JPT_ENSURE(!moved.Contains("Engine"));
    JPT_ENSURE(!moved.Contains("Game"));

    JPT_ENSURE(hashSet.IsEmpty());

    return true;
}

bool UnitTest_HashSet_Grow()
{
    jpt::HashSet<int32> hashSet;

    for (int32 i = 0; i < 1000; ++i)
    {
        hashSet.Add(i);
    }

    for (int32 i = 0; i < 1000; ++i)
    {
        JPT_ENSURE(hashSet.Contains(i));
    }

    return true;
}


export bool RunUnitTests_HashSet()
{
    JPT_ENSURE(UnitTest_HashSet());
    JPT_ENSURE(UnitTest_HashSet_String());

    JPT_ENSURE(UnitTest_HashSet_Copy());
    JPT_ENSURE(UnitTest_HashSet_Copy_String());

    JPT_ENSURE(UnitTest_HashSet_CopyAssign());
    JPT_ENSURE(UnitTest_HashSet_CopyAssign_String());

    JPT_ENSURE(UnitTest_HashSet_Move());
    JPT_ENSURE(UnitTest_HashSet_Move_String());

    JPT_ENSURE(UnitTest_HashSet_MoveAssign());
    JPT_ENSURE(UnitTest_HashSet_MoveAssign_String());

    JPT_ENSURE(UnitTest_HashSet_Grow());

    return true;
}
