// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Tuple;

import jpt.Utilities;
import jpt.Tuple;
import jpt.TypeDefs;
import jpt.String;

class Foo
{
private:
    int32 m_data = 10;

public:
    Foo() = default;

    Foo(int32 data) :m_data(data) {}

    jpt::String ToString() const { return "I am Foo"; }

    int32 GetData() const { return m_data; }
};

bool operator==(const Foo& a, const Foo& b)
{
    return a.GetData() == b.GetData();
}

bool UnitTests_Tuple()
{
    jpt::Tuple<int, char, bool, Foo, int, jpt::String> tuple{3, 'C', false, Foo(10), 4, "Hello"};
    
    // Initializing
    JPT_ENSURE(jpt::Get<0>(tuple) == 3);
    JPT_ENSURE(jpt::Get<1>(tuple) == 'C');
    JPT_ENSURE(jpt::Get<2>(tuple) == false);
    JPT_ENSURE(jpt::Get<3>(tuple) == Foo(10));
    JPT_ENSURE(jpt::Get<4>(tuple) == 4);
    JPT_ENSURE(jpt::Get<5>(tuple) == "Hello");

    // Copy assign
    jpt::Get<0>(tuple) = 42;
    jpt::Get<1>(tuple) = 'A';
    jpt::Get<2>(tuple) = true;
    jpt::Get<3>(tuple) = Foo(42);
    jpt::Get<4>(tuple) = 0;
    jpt::Get<5>(tuple).Append(" World");

    JPT_ENSURE(jpt::Get<0>(tuple) == 42);
    JPT_ENSURE(jpt::Get<1>(tuple) == 'A');
    JPT_ENSURE(jpt::Get<2>(tuple) == true);
    JPT_ENSURE(jpt::Get<3>(tuple) == Foo(42));
    JPT_ENSURE(jpt::Get<4>(tuple) == 0);
    JPT_ENSURE(jpt::Get<5>(tuple) == "Hello World");

    // Move assign
    Foo foo(420);
    jpt::String str("Jupiter Engine");

    jpt::Get<3>(tuple) = jpt::Move(foo);
    jpt::Get<5>(tuple) = jpt::Move(str);

    JPT_ENSURE(jpt::Get<3>(tuple) == Foo(420));
    JPT_ENSURE(jpt::Get<5>(tuple) == "Jupiter Engine");
    
    // Get Count
    // Warning    C4127    conditional expression is constant :)
    //JPT_ENSURE(jpt::GetSize(tuple) == 6);

    return true;
}

bool UnitTests_EmptyTuple()
{
    jpt::Tuple<int, char, bool, Foo, int, jpt::String> tuple;

    // Copy assign
    jpt::Get<0>(tuple) = 42;
    jpt::Get<1>(tuple) = 'A';
    jpt::Get<2>(tuple) = true;
    jpt::Get<3>(tuple) = Foo(42);
    jpt::Get<4>(tuple) = 0;
    jpt::Get<5>(tuple).Append(" World");

    JPT_ENSURE(jpt::Get<0>(tuple) == 42);
    JPT_ENSURE(jpt::Get<1>(tuple) == 'A');
    JPT_ENSURE(jpt::Get<2>(tuple) == true);
    JPT_ENSURE(jpt::Get<3>(tuple) == Foo(42));
    JPT_ENSURE(jpt::Get<4>(tuple) == 0);
    JPT_ENSURE(jpt::Get<5>(tuple) == " World");

    // Move assign
    char c = 'C';
    Foo foo(420);
    jpt::String str("Jupiter Engine");

    jpt::Get<1>(tuple) = jpt::Move(c);
    jpt::Get<3>(tuple) = jpt::Move(foo);
    jpt::Get<5>(tuple) = jpt::Move(str);

    JPT_ENSURE(jpt::Get<1>(tuple) == 'C');
    JPT_ENSURE(jpt::Get<3>(tuple) == Foo(420));
    JPT_ENSURE(jpt::Get<5>(tuple) == "Jupiter Engine");

    // Get Count
    //JPT_ENSURE(jpt::GetSize(tuple) == 6);

    return true;
}

bool UnitTests_ConstTuple()
{
    const jpt::Tuple<int, char, bool, Foo, jpt::String> tuple{ 3, 'C', false, Foo(10), "Hello" };

    // Initializing
    JPT_ENSURE(jpt::Get<0>(tuple) == 3);
    JPT_ENSURE(jpt::Get<1>(tuple) == 'C');
    JPT_ENSURE(jpt::Get<2>(tuple) == false);
    JPT_ENSURE(jpt::Get<3>(tuple) == Foo(10));
    JPT_ENSURE(jpt::Get<4>(tuple) == "Hello");

    // Get Count
    //JPT_ENSURE(jpt::GetSize(tuple) == 5);

    return true;
}

bool UnitTests_ConstTDatauple()
{
    jpt::Tuple<int, char, const bool, Foo, const int, const jpt::String> tuple{ 3, 'C', false, Foo(10), 4, "Hello" };

    // Initializing
    JPT_ENSURE(jpt::Get<0>(tuple) == 3);
    JPT_ENSURE(jpt::Get<1>(tuple) == 'C');
    JPT_ENSURE(jpt::Get<2>(tuple) == false);
    JPT_ENSURE(jpt::Get<3>(tuple) == Foo(10));
    JPT_ENSURE(jpt::Get<4>(tuple) == 4);
    JPT_ENSURE(jpt::Get<5>(tuple) == "Hello");

    // Copy assign
    jpt::Get<0>(tuple) = 42;
    jpt::Get<1>(tuple) = 'A';
    jpt::Get<3>(tuple) = Foo(42);

    JPT_ENSURE(jpt::Get<0>(tuple) == 42);
    JPT_ENSURE(jpt::Get<1>(tuple) == 'A');
    JPT_ENSURE(jpt::Get<3>(tuple) == Foo(42));

    // Move assign
    Foo foo(420);

    jpt::Get<3>(tuple) = jpt::Move(foo);

    JPT_ENSURE(jpt::Get<3>(tuple) == Foo(420));

    // Get Count
    //JPT_ENSURE(jpt::GetSize(tuple) == 6);

    return true;
}

bool UnitTests_Tie()
{
    auto tuple = jpt::Tie(3, 'C', false, Foo(10), 4, jpt::String("Hello"));

    // Initializing
    JPT_ENSURE(jpt::Get<0>(tuple) == 3);
    JPT_ENSURE(jpt::Get<1>(tuple) == 'C');
    JPT_ENSURE(jpt::Get<2>(tuple) == false);
    JPT_ENSURE(jpt::Get<3>(tuple) == Foo(10));
    JPT_ENSURE(jpt::Get<4>(tuple) == 4);
    JPT_ENSURE(jpt::Get<5>(tuple) == "Hello");

    // Copy assign
    jpt::Get<0>(tuple) = 42;
    jpt::Get<1>(tuple) = 'A';
    jpt::Get<2>(tuple) = true;
    jpt::Get<3>(tuple) = Foo(42);
    jpt::Get<4>(tuple) = 0;
    jpt::Get<5>(tuple).Append(" World");

    JPT_ENSURE(jpt::Get<0>(tuple) == 42);
    JPT_ENSURE(jpt::Get<1>(tuple) == 'A');
    JPT_ENSURE(jpt::Get<2>(tuple) == true);
    JPT_ENSURE(jpt::Get<3>(tuple) == Foo(42));
    JPT_ENSURE(jpt::Get<4>(tuple) == 0);
    JPT_ENSURE(jpt::Get<5>(tuple) == "Hello World");

    // Move assign
    Foo foo(420);
    jpt::String str("Jupiter Engine");

    jpt::Get<3>(tuple) = jpt::Move(foo);
    jpt::Get<5>(tuple) = jpt::Move(str);

    JPT_ENSURE(jpt::Get<3>(tuple) == Foo(420));
    JPT_ENSURE(jpt::Get<5>(tuple) == "Jupiter Engine");

    // Get Count
    //JPT_ENSURE(jpt::GetSize(tuple) == 6);

    return true;
}

bool UnitTests_OneTypeTuple()
{
    jpt::Tuple<int> tuple{42};

    JPT_ENSURE(jpt::Get<0>(tuple) == 42);
    //JPT_ENSURE(jpt::GetSize(tuple) == 1);

    return true;
}

export bool RunUnitTests_Tuple()
{
    JPT_ENSURE(UnitTests_Tuple());
    JPT_ENSURE(UnitTests_EmptyTuple());
    JPT_ENSURE(UnitTests_OneTypeTuple());
    JPT_ENSURE(UnitTests_ConstTuple());
    JPT_ENSURE(UnitTests_ConstTDatauple());
    JPT_ENSURE(UnitTests_Tie());

    return true;
}