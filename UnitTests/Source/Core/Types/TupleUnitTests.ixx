// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module TupleUnitTests;

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
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<0>(tuple) != 3, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<1>(tuple) != 'C', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<2>(tuple) != false, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<3>(tuple) != Foo(10), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<4>(tuple) != 4, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<5>(tuple) != "Hello", "");

	// Copy assign
	jpt::GetValueAt<0>(tuple) = 42;
	jpt::GetValueAt<1>(tuple) = 'A';
	jpt::GetValueAt<2>(tuple) = true;
	jpt::GetValueAt<3>(tuple) = Foo(42);
	jpt::GetValueAt<4>(tuple) = 0;
	jpt::GetValueAt<5>(tuple).Append(" World");

	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<0>(tuple) != 42, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<1>(tuple) != 'A', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<2>(tuple) != true, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<3>(tuple) != Foo(42), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<4>(tuple) != 0, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<5>(tuple) != "Hello World", "");

	// Move assign
	Foo foo(420);
	jpt::String str("Jupiter Engine");

	jpt::GetValueAt<3>(tuple) = jpt::Move(foo);
	jpt::GetValueAt<5>(tuple) = jpt::Move(str);

	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<3>(tuple) != Foo(420), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<5>(tuple) != "Jupiter Engine", "");

	// Get Size
	//JPT_RETURN_FALSE_IF_ERROR(jpt::TupleSize<decltype(tuple)>::kValue != 6, "");

	return true;
}

bool UnitTest_EmptyTuple()
{
	jpt::Tuple<int, char, bool, Foo, int, jpt::String> tuple;

	// Copy assign
	jpt::GetValueAt<0>(tuple) = 42;
	jpt::GetValueAt<1>(tuple) = 'A';
	jpt::GetValueAt<2>(tuple) = true;
	jpt::GetValueAt<3>(tuple) = Foo(42);
	jpt::GetValueAt<4>(tuple) = 0;
	jpt::GetValueAt<5>(tuple).Append(" World");

	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<0>(tuple) != 42, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<1>(tuple) != 'A', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<2>(tuple) != true, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<3>(tuple) != Foo(42), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<4>(tuple) != 0, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<5>(tuple) != " World", "");

	// Move assign
	Foo foo(420);
	jpt::String str("Jupiter Engine");

	jpt::GetValueAt<3>(tuple) = jpt::Move(foo);
	jpt::GetValueAt<5>(tuple) = jpt::Move(str);

	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<3>(tuple) != Foo(420), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<5>(tuple) != "Jupiter Engine", "");

	// Get Size
	//JPT_RETURN_FALSE_IF_ERROR(jpt::TupleSize<decltype(tuple)>::kValue != 6, "");

	return true;
}

bool UnitTest_ConstTuple()
{
	const jpt::Tuple<int, char, bool, Foo, jpt::String> tuple{ 3, 'C', false, Foo(10), "Hello" };

	// Initializing
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<0>(tuple) != 3, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<1>(tuple) != 'C', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<2>(tuple) != false, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<3>(tuple) != Foo(10), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<4>(tuple) != "Hello", "");

	// Get Size
	//JPT_RETURN_FALSE_IF_ERROR(jpt::TupleSize<decltype(tuple)>::kValue != 5, "");

	return true;
}

bool UnitTest_ConstDataTuple()
{
	jpt::Tuple<int, char, const bool, Foo, const int, const jpt::String> tuple{ 3, 'C', false, Foo(10), 4, "Hello" };

	// Initializing
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<0>(tuple) != 3, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<1>(tuple) != 'C', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<2>(tuple) != false, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<3>(tuple) != Foo(10), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<4>(tuple) != 4, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<5>(tuple) != "Hello", "");

	// Copy assign
	jpt::GetValueAt<0>(tuple) = 42;
	jpt::GetValueAt<1>(tuple) = 'A';
	jpt::GetValueAt<3>(tuple) = Foo(42);

	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<0>(tuple) != 42, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<1>(tuple) != 'A', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<3>(tuple) != Foo(42), "");

	// Move assign
	Foo foo(420);

	jpt::GetValueAt<3>(tuple) = jpt::Move(foo);

	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<3>(tuple) != Foo(420), "");

	// Get Size
	//JPT_RETURN_FALSE_IF_ERROR(jpt::TupleSize<decltype(tuple)>::kValue != 6, "");

	return true;
}

bool UnitTest_Tie()
{
	auto tuple = jpt::Tie(3, 'C', false, Foo(10), 4, jpt::String("Hello"));

	// Initializing
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<0>(tuple) != 3, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<1>(tuple) != 'C', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<2>(tuple) != false, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<3>(tuple) != Foo(10), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<4>(tuple) != 4, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<5>(tuple) != "Hello", "");

	// Copy assign
	jpt::GetValueAt<0>(tuple) = 42;
	jpt::GetValueAt<1>(tuple) = 'A';
	jpt::GetValueAt<2>(tuple) = true;
	jpt::GetValueAt<3>(tuple) = Foo(42);
	jpt::GetValueAt<4>(tuple) = 0;
	jpt::GetValueAt<5>(tuple).Append(" World");

	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<0>(tuple) != 42, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<1>(tuple) != 'A', "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<2>(tuple) != true, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<3>(tuple) != Foo(42), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<4>(tuple) != 0, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<5>(tuple) != "Hello World", "");

	// Move assign
	Foo foo(420);
	jpt::String str("Jupiter Engine");

	jpt::GetValueAt<3>(tuple) = jpt::Move(foo);
	jpt::GetValueAt<5>(tuple) = jpt::Move(str);

	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<3>(tuple) != Foo(420), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetValueAt<5>(tuple) != "Jupiter Engine", "");

	// Get Size
	//JPT_RETURN_FALSE_IF_ERROR(jpt::TupleSize<decltype(tuple)>::kValue != 6, "");

	return true;
}

export bool RunTupleUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Tuple(), "UnitTest_Tuple Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_EmptyTuple(), "UnitTest_EmptyTuple Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_ConstTuple(), "UnitTest_ConstTuple Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_ConstDataTuple(), "UnitTest_ConstDataTuple Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Tie(), "UnitTest_Tie Failed");

	return true;
}