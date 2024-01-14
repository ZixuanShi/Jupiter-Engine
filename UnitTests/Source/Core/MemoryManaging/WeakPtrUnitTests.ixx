// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

#include <memory>

export module WeakPtrUnitTests;

import jpt.CoreModules;

bool UnitTests_WeakPtr_Class()
{
	struct Foo
	{
		int32 m_left = 0;
		char m_right = '0';

		Foo() = default;
		Foo(int32 left, char right) : m_left(left), m_right(right) {}
		~Foo() 
		{ 
			//JPT_LOG("Destroyed a Foo %s", ToString().ConstBuffer()); 
		}

		jpt::String ToString() const
		{
			jpt::String result;
			result += jpt::ToString(m_left);
			result += ' ';
			result += jpt::ToString(m_right);
			return result;
		}
	};

	//std::weak_ptr<Foo> fooWeak;
	//JPT_RETURN_FALSE_IF_ERROR(!fooWeak.expired(), "");

	//{
	//	std::shared_ptr<Foo> fooShared1 = std::make_shared<Foo>(42, 'C');
	//	fooWeak = fooShared1;

	//	JPT_RETURN_FALSE_IF_ERROR(fooWeak.expired(), "");
	//	JPT_RETURN_FALSE_IF_ERROR(fooWeak.use_count() != 1, "");

	//	std::shared_ptr<Foo> fooShared2 = fooWeak.lock();
	//	JPT_RETURN_FALSE_IF_ERROR(fooWeak.expired(), "");
	//	JPT_RETURN_FALSE_IF_ERROR(fooWeak.use_count() != 2, "");
	//	JPT_RETURN_FALSE_IF_ERROR(fooShared2.use_count() != 2, "");

	//	JPT_RETURN_FALSE_IF_ERROR(fooShared1->m_left != 42, "");
	//	JPT_RETURN_FALSE_IF_ERROR(fooShared1->m_right != 'C', "");
	//	JPT_RETURN_FALSE_IF_ERROR(fooShared2->m_left != 42, "");
	//	JPT_RETURN_FALSE_IF_ERROR(fooShared2->m_right != 'C', "");
	//}

	//JPT_RETURN_FALSE_IF_ERROR(!fooWeak.expired(), "");
	//JPT_RETURN_FALSE_IF_ERROR(fooWeak.use_count() != 0, "");

	jpt::WeakPtr<Foo> fooWeak;
	JPT_RETURN_FALSE_IF_ERROR(!fooWeak.IsExpired(), "");
	JPT_RETURN_FALSE_IF_ERROR(fooWeak.GetRefCount() != 0, "");

	{
		jpt::SharedPtr<Foo> fooShared1 = jpt::MakeShared<Foo>(42, 'C');
		fooWeak = fooShared1;

		JPT_RETURN_FALSE_IF_ERROR(fooWeak.IsExpired(), "");
		JPT_RETURN_FALSE_IF_ERROR(fooWeak.GetRefCount() != 1, "");

		jpt::SharedPtr<Foo> fooShared2 = fooWeak.Lock();
		JPT_RETURN_FALSE_IF_ERROR(fooWeak.IsExpired(), "");
		JPT_RETURN_FALSE_IF_ERROR(fooWeak.GetRefCount() != 2, "");
		JPT_RETURN_FALSE_IF_ERROR(fooShared2.GetRefCount() != 2, "");

		JPT_RETURN_FALSE_IF_ERROR(fooShared1->m_left != 42, "");
		JPT_RETURN_FALSE_IF_ERROR(fooShared1->m_right != 'C', "");
		JPT_RETURN_FALSE_IF_ERROR(fooShared2->m_left != 42, "");
		JPT_RETURN_FALSE_IF_ERROR(fooShared2->m_right != 'C', "");
	}

	JPT_RETURN_FALSE_IF_ERROR(!fooWeak.IsExpired(), "");
	JPT_RETURN_FALSE_IF_ERROR(fooWeak.GetRefCount() != 0, "");

	return true;
}

export bool RunWeakPtrUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTests_WeakPtr_Class(), "UnitTests_WeakPtr_Class Failed");

	return true;
}