// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module UnitTests_Tuple;

import jpt.CoreModules;

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

bool UnitTest_Tuple()
{
	jpt::Tuple<int, char, bool, Foo, int, jpt::String> tuple{3, 'C', false, Foo(10), 4, "Hello"};
	
	// Initializing
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<0>(tuple) != 3, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<1>(tuple) != 'C', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<2>(tuple) != false, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<3>(tuple) != Foo(10), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<4>(tuple) != 4, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<5>(tuple) != "Hello", "");

	// Copy assign
	jpt::Get<0>(tuple) = 42;
	jpt::Get<1>(tuple) = 'A';
	jpt::Get<2>(tuple) = true;
	jpt::Get<3>(tuple) = Foo(42);
	jpt::Get<4>(tuple) = 0;
	jpt::Get<5>(tuple).Append(" World");

	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<0>(tuple) != 42, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<1>(tuple) != 'A', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<2>(tuple) != true, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<3>(tuple) != Foo(42), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<4>(tuple) != 0, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<5>(tuple) != "Hello World", "");

	// Move assign
	Foo foo(420);
	jpt::String str("Jupiter Engine");

	jpt::Get<3>(tuple) = jpt::Move(foo);
	jpt::Get<5>(tuple) = jpt::Move(str);

	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<3>(tuple) != Foo(420), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<5>(tuple) != "Jupiter Engine", "");
	
	// Get Size
	// Warning	C4127	conditional expression is constant :)
	//JPT_RETURN_FALSE_IF_ERROR(jpt::GetSize(tuple) != 6, "");

	return true;
}

bool UnitTest_EmptyTuple()
{
	jpt::Tuple<int, char, bool, Foo, int, jpt::String> tuple;

	// Copy assign
	jpt::Get<0>(tuple) = 42;
	jpt::Get<1>(tuple) = 'A';
	jpt::Get<2>(tuple) = true;
	jpt::Get<3>(tuple) = Foo(42);
	jpt::Get<4>(tuple) = 0;
	jpt::Get<5>(tuple).Append(" World");

	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<0>(tuple) != 42, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<1>(tuple) != 'A', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<2>(tuple) != true, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<3>(tuple) != Foo(42), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<4>(tuple) != 0, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<5>(tuple) != " World", "");

	// Move assign
	char c = 'C';
	Foo foo(420);
	jpt::String str("Jupiter Engine");

	jpt::Get<1>(tuple) = jpt::Move(c);
	jpt::Get<3>(tuple) = jpt::Move(foo);
	jpt::Get<5>(tuple) = jpt::Move(str);

	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<1>(tuple) != 'C', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<3>(tuple) != Foo(420), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<5>(tuple) != "Jupiter Engine", "");

	// Get Size
	//JPT_RETURN_FALSE_IF_ERROR(jpt::GetSize(tuple) != 6, "");

	return true;
}

bool UnitTest_ConstTuple()
{
	const jpt::Tuple<int, char, bool, Foo, jpt::String> tuple{ 3, 'C', false, Foo(10), "Hello" };

	// Initializing
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<0>(tuple) != 3, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<1>(tuple) != 'C', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<2>(tuple) != false, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<3>(tuple) != Foo(10), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<4>(tuple) != "Hello", "");

	// Get Size
	//JPT_RETURN_FALSE_IF_ERROR(jpt::GetSize(tuple) != 5, "");

	return true;
}

bool UnitTest_ConstTDatauple()
{
	jpt::Tuple<int, char, const bool, Foo, const int, const jpt::String> tuple{ 3, 'C', false, Foo(10), 4, "Hello" };

	// Initializing
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<0>(tuple) != 3, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<1>(tuple) != 'C', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<2>(tuple) != false, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<3>(tuple) != Foo(10), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<4>(tuple) != 4, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<5>(tuple) != "Hello", "");

	// Copy assign
	jpt::Get<0>(tuple) = 42;
	jpt::Get<1>(tuple) = 'A';
	jpt::Get<3>(tuple) = Foo(42);

	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<0>(tuple) != 42, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<1>(tuple) != 'A', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<3>(tuple) != Foo(42), "");

	// Move assign
	Foo foo(420);

	jpt::Get<3>(tuple) = jpt::Move(foo);

	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<3>(tuple) != Foo(420), "");

	// Get Size
	//JPT_RETURN_FALSE_IF_ERROR(jpt::GetSize(tuple) != 6, "");

	return true;
}

bool UnitTest_Tie()
{
	auto tuple = jpt::Tie(3, 'C', false, Foo(10), 4, jpt::String("Hello"));

	// Initializing
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<0>(tuple) != 3, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<1>(tuple) != 'C', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<2>(tuple) != false, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<3>(tuple) != Foo(10), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<4>(tuple) != 4, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<5>(tuple) != "Hello", "");

	// Copy assign
	jpt::Get<0>(tuple) = 42;
	jpt::Get<1>(tuple) = 'A';
	jpt::Get<2>(tuple) = true;
	jpt::Get<3>(tuple) = Foo(42);
	jpt::Get<4>(tuple) = 0;
	jpt::Get<5>(tuple).Append(" World");

	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<0>(tuple) != 42, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<1>(tuple) != 'A', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<2>(tuple) != true, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<3>(tuple) != Foo(42), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<4>(tuple) != 0, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<5>(tuple) != "Hello World", "");

	// Move assign
	Foo foo(420);
	jpt::String str("Jupiter Engine");

	jpt::Get<3>(tuple) = jpt::Move(foo);
	jpt::Get<5>(tuple) = jpt::Move(str);

	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<3>(tuple) != Foo(420), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<5>(tuple) != "Jupiter Engine", "");

	// Get Size
	//JPT_RETURN_FALSE_IF_ERROR(jpt::GetSize(tuple) != 6, "");

	return true;
}

bool UnitTest_OneTypeTuple()
{
	jpt::Tuple<int> tuple{42};

	JPT_RETURN_FALSE_IF_ERROR(jpt::Get<0>(tuple) != 42, "");
	//JPT_RETURN_FALSE_IF_ERROR(jpt::GetSize(tuple) != 1, "");

	return true;
}

export bool RunUnitTests_Tuple()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Tuple(), "UnitTest_Tuple Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_EmptyTuple(), "UnitTest_EmptyTuple Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_OneTypeTuple(), "UnitTest_OneTypeTuple Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_ConstTuple(), "UnitTest_ConstTuple Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_ConstTDatauple(), "UnitTest_ConstTDatauple Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Tie(), "UnitTest_Tie Failed");

	return true;
}