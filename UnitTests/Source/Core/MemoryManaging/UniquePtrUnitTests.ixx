// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

#include <memory>

export module UniquePtrUnitTests;

import jpt.CoreModules;

bool UnitTest_UniquePtr_Char()
{
	auto deleter = [](char* pCharPtr)
		{
			//JPT_LOG("Deleted a char %c", *pCharPtr);
			delete pCharPtr;
		};

	auto deleter2 = [](char* pCharPtr)
		{
			//JPT_LOG("Another Deleted a char %c", *pCharPtr);
			delete pCharPtr;
		};

	jpt::UniquePtr<char> uniqueChar = jpt::MakeUnique<char>('C');
	JPT_RETURN_FALSE_IF_ERROR(!uniqueChar.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!uniqueChar, "");
	JPT_RETURN_FALSE_IF_ERROR(*uniqueChar != 'C', "");

	uniqueChar.Reset(new char('D'));
	JPT_RETURN_FALSE_IF_ERROR(*uniqueChar != 'D', "");

	char* c = uniqueChar.Release();
	JPT_RETURN_FALSE_IF_ERROR(uniqueChar, "");
	JPT_RETURN_FALSE_IF_ERROR(uniqueChar.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(*c != 'D', "");
	delete c;

	jpt::UniquePtr<char, decltype(deleter)> customDeleterUniquePtr = jpt::UniquePtr<char, decltype(deleter)>(new char('C'), deleter);
	JPT_RETURN_FALSE_IF_ERROR(!customDeleterUniquePtr.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!customDeleterUniquePtr, "");
	JPT_RETURN_FALSE_IF_ERROR(*customDeleterUniquePtr != 'C', "");

	customDeleterUniquePtr.Reset(new char('D'));
	JPT_RETURN_FALSE_IF_ERROR(*customDeleterUniquePtr != 'D', "");

	jpt::UniquePtr<char, decltype(deleter2)> otherCustomDeleterUniquePtr = jpt::UniquePtr<char, decltype(deleter2)>(new char('D'), deleter2);

	otherCustomDeleterUniquePtr.GetDeleter()(customDeleterUniquePtr.Release());
	JPT_RETURN_FALSE_IF_ERROR(customDeleterUniquePtr, "");
	JPT_RETURN_FALSE_IF_ERROR(customDeleterUniquePtr.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(*otherCustomDeleterUniquePtr != 'D', "");

	jpt::UniquePtr<char, decltype(deleter)> source = jpt::UniquePtr<char, decltype(deleter)>(new char('A'), deleter);
	jpt::UniquePtr<char, decltype(deleter)> other  = jpt::UniquePtr<char, decltype(deleter)>(new char('B'), deleter);
	source = jpt::Move(other);

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

bool UnitTest_UniquePtr_Class()
{
	struct Foo
	{
		int32 m_left = 0;
		char m_right = '0';

		Foo() = default;
		Foo(int32 left, char right) : m_left(left), m_right(right) {}
	};

	class Bar
	{
		jpt::UniquePtr<Foo> m_pFoo;
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

export bool RunUniquePtrUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_UniquePtr_Char(), "UnitTest_UniquePtr_Char Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_UniquePtr_Int(), "UnitTest_UniquePtr_Int Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_UniquePtr_String(), "UnitTest_UniquePtr_String Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_UniquePtr_Class(), "UnitTest_UniquePtr_Class Failed");

	return true;
}