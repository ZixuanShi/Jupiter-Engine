// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

#include <memory>

export module UniquePtrUnitTests;

import jpt.CoreModules;

bool UnitTest_UniquePtr_Char()
{
	//jpt::UniquePtr<char> uniquePtrChar = jpt::MakeUnique<char>('C');

	struct Foo
	{
		int32 m_left = 0;
		char m_right = '0';

		Foo() = default;
		Foo(int32 left, char right) : m_left(left), m_right(right) {}
	};

	{
		std::unique_ptr<Foo> pFoo = std::make_unique<Foo>(10, 'H');
		pFoo->m_left;
		pFoo->m_right;
	}

	{
		jpt::UniquePtr<Foo> pFoo = jpt::MakeUnique<Foo>(10, 'H');
	}

	return true;
}

bool UnitTest_UniquePtr_Int()
{
	return true;
}

bool UnitTest_UniquePtr_String()
{
	return true;
}

export bool RunUniquePtrUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_UniquePtr_Char(), "UnitTest_UniquePtr_Char Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_UniquePtr_Int(), "UnitTest_UniquePtr_Int Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_UniquePtr_String(), "UnitTest_UniquePtr_String Failed");

	return true;
}