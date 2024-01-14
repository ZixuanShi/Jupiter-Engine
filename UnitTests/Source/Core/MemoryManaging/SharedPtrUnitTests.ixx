// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

#include <memory>

export module StrongPtrUnitTests;

import jpt.CoreModules;

bool UnitTest_StrongPtr_Char()
{
	jpt::StrongPtr<char> sharedCharPtr1 = jpt::MakeShared<char>('C');
	JPT_RETURN_FALSE_IF_ERROR(!sharedCharPtr1.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!sharedCharPtr1, "");
	JPT_RETURN_FALSE_IF_ERROR(*sharedCharPtr1 != 'C', "");
	
	jpt::StrongPtr<char> sharedCharPtr2 = sharedCharPtr1;
	JPT_RETURN_FALSE_IF_ERROR(!sharedCharPtr2.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!sharedCharPtr2, "");
	JPT_RETURN_FALSE_IF_ERROR(*sharedCharPtr2 != 'C', "");

	jpt::StrongPtr<char> sharedCharPtr3 = sharedCharPtr1;
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

bool UnitTest_StrongPtr_Class()
{
	struct Bar;

	struct Foo
	{
		jpt::StrongPtr<Bar> m_bar;
	};

	struct Bar
	{
		jpt::WeakPtr<Foo> m_foo;
	};

	jpt::StrongPtr<Foo> foo(new Foo);
	jpt::StrongPtr<Bar> bar(new Bar);

	foo->m_bar = bar;
	bar->m_foo = foo;

	return true;
}

bool UnitTest_StrongPtr_Class2()
{
	struct Bar;
	
	struct Foo
	{
		jpt::WeakPtr<Bar> m_bar;
	};
	struct Bar
	{
		jpt::StrongPtr<Foo> m_foo;
	};

	jpt::StrongPtr<Foo> foo(new Foo);
	jpt::StrongPtr<Bar> bar(new Bar);

	foo->m_bar = bar;
	bar->m_foo = foo;

	foo.Reset(new Foo);

	jpt::WeakPtr<Foo> weakFoo;
	jpt::WeakPtr<Foo> weakFoo2;
	jpt::StrongPtr<Foo> sharedFoo1(new Foo);
	jpt::StrongPtr<Foo> sharedFoo2(new Foo);
	weakFoo = sharedFoo1;
	weakFoo = sharedFoo2;
	weakFoo = weakFoo2;
	weakFoo2 = sharedFoo1;
	weakFoo2.Lock();

	return true;
}

export bool RunStrongPtrUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StrongPtr_Char(), "UnitTest_UniquePtr_Char Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StrongPtr_Class(), "UnitTest_StrongPtr_Class Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StrongPtr_Class2(), "UnitTest_StrongPtr_Class2 Failed");

	return true;
}