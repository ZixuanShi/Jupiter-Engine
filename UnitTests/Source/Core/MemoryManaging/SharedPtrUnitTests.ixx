// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

#include <memory>

export module SharedPtrUnitTests;

import jpt.CoreModules;

bool UnitTest_SharedPtr_Char()
{
	jpt::SharedPtr<char> sharedCharPtr1 = jpt::MakeShared<char>('C');
	JPT_RETURN_FALSE_IF_ERROR(!sharedCharPtr1.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!sharedCharPtr1, "");
	JPT_RETURN_FALSE_IF_ERROR(*sharedCharPtr1 != 'C', "");
	
	jpt::SharedPtr<char> sharedCharPtr2 = sharedCharPtr1;
	JPT_RETURN_FALSE_IF_ERROR(!sharedCharPtr2.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!sharedCharPtr2, "");
	JPT_RETURN_FALSE_IF_ERROR(*sharedCharPtr2 != 'C', "");

	jpt::SharedPtr<char> sharedCharPtr3 = sharedCharPtr1;
	JPT_RETURN_FALSE_IF_ERROR(!sharedCharPtr3.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!sharedCharPtr3, "");
	JPT_RETURN_FALSE_IF_ERROR(*sharedCharPtr3 != 'C', "");

	JPT_RETURN_FALSE_IF_ERROR(sharedCharPtr1.GetRefCount() != 3, "");
	JPT_RETURN_FALSE_IF_ERROR(sharedCharPtr2.GetRefCount() != 3, "");
	JPT_RETURN_FALSE_IF_ERROR(sharedCharPtr3.GetRefCount() != 3, "");

	sharedCharPtr3.Reset(new char('A'));
	JPT_RETURN_FALSE_IF_ERROR(!sharedCharPtr3.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!sharedCharPtr3, "");
	JPT_RETURN_FALSE_IF_ERROR(*sharedCharPtr3 != 'A', "");

	JPT_RETURN_FALSE_IF_ERROR(sharedCharPtr1.GetRefCount() != 2, "");
	JPT_RETURN_FALSE_IF_ERROR(sharedCharPtr2.GetRefCount() != 2, "");
	JPT_RETURN_FALSE_IF_ERROR(sharedCharPtr3.GetRefCount() != 1, "");

	return true;
}

bool UnitTest_SharedPtr_Class()
{
	struct Bar;

	// Foo has a shared ptr to a Bar obj
	struct Foo
	{
		jpt::SharedPtr<Bar> m_bar;
	};

	// Bar has a weak ptr to a Foo obj
	struct Bar
	{
		jpt::WeakPtr<Foo> m_foo;
	};

	jpt::SharedPtr<Foo> foo(new Foo);
	jpt::SharedPtr<Bar> bar(new Bar);

	// At this point, foo and bar both have 1 shared RefCount, 0 weak RefCount

	// foo's SharedPtr<Bar> is going to hold bar, with copy operator=(const SharedPtr&)
	foo->m_bar = bar;

	bar->m_foo = foo;

	return true;
}

bool UnitTest_SharedPtr_Class2()
{
	struct Bar;
	
	// Foo has a weak ptr to a Bar obj
	struct Foo
	{
		jpt::WeakPtr<Bar> m_bar;
	};

	// Bar has a shared ptr to a Foo obj
	struct Bar
	{
		jpt::SharedPtr<Foo> m_foo;
	};

	jpt::SharedPtr<Foo> foo(new Foo);
	jpt::SharedPtr<Bar> bar(new Bar);

	// At this point, foo and bar both have 1 shared RefCount, 0 weak RefCount

	// foo's WeakPtr<Bar> got assigned bar through WeakPtr<DataT>::WeakPtr(const SharedPtr<DataT>& shared)
	foo->m_bar = bar;

	// bar's SharedPtr<Foo> got assigned foo through SharedPtr<DataT>& SharedPtr<DataT>::operator=(const SharedPtr& other)
	bar->m_foo = foo;

	// bar calling destructor->Reset(nullptr)


	return true;
}

export bool RunSharedPtrUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_SharedPtr_Char(), "UnitTest_UniquePtr_Char Failed");
	
	// Failing
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_SharedPtr_Class(), "UnitTest_SharedPtr_Class Failed");
	
	
	// Works
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_SharedPtr_Class2(), "UnitTest_SharedPtr_Class2 Failed");



	return true;
}