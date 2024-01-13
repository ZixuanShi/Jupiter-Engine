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
	struct B;
	struct A
	{
		jpt::SharedPtr<B> m_b;
	};

	struct B
	{
		jpt::SharedPtr<A> m_a;
	};

	jpt::SharedPtr<A> a(new A);
	jpt::SharedPtr<B> b(new B);
	a->m_b = b;
	b->m_a = a;

	return true;
}

export bool RunSharedPtrUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_SharedPtr_Char(), "UnitTest_UniquePtr_Char Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_SharedPtr_Class(), "UnitTest_SharedPtr_Class Failed");

	return true;
}