// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <memory>

export module UnitTests_WeakPtr;

import jpt.StrongPtr;
import jpt.WeakPtr;
import jpt.ToString;
import jpt.TypeDefs;
import jpt.String;
import jpt.Utilities;

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
	//JPT_ENSURE(fooWeak.expired());

	//{
	//	std::shared_ptr<Foo> fooShared1 = std::make_shared<Foo>(42, 'C');
	//	fooWeak = fooShared1;

	//	JPT_ENSURE(fooWeak.expired());
	//	JPT_ENSURE(fooWeak.use_count() == 1);

	//	std::shared_ptr<Foo> fooShared2 = fooWeak.lock();
	//	JPT_ENSURE(fooWeak.expired());
	//	JPT_ENSURE(fooWeak.use_count() == 2);
	//	JPT_ENSURE(fooShared2.use_count() == 2);

	//	JPT_ENSURE(fooShared1->m_left == 42);
	//	JPT_ENSURE(fooShared1->m_right == 'C');
	//	JPT_ENSURE(fooShared2->m_left == 42);
	//	JPT_ENSURE(fooShared2->m_right == 'C');
	//}

	//JPT_ENSURE(fooWeak.expired());
	//JPT_ENSURE(fooWeak.use_count() == 0);

	jpt::WeakPtr<Foo> fooWeak;
	JPT_ENSURE(fooWeak.IsExpired());
	JPT_ENSURE(fooWeak.GetRefCount() == 0);

	{
		jpt::StrongPtr<Foo> fooShared1 = jpt::MakeStrong<Foo>(42, 'C');
		fooWeak = fooShared1;

		JPT_ENSURE(!fooWeak.IsExpired());
		JPT_ENSURE(fooWeak.GetRefCount() == 1);

		jpt::StrongPtr<Foo> fooShared2 = fooWeak;
		JPT_ENSURE(!fooWeak.IsExpired());
		JPT_ENSURE(fooWeak.GetRefCount() == 2);
		JPT_ENSURE(fooShared2.GetRefCount() == 2);

		JPT_ENSURE(fooShared1->m_left == 42);
		JPT_ENSURE(fooShared1->m_right == 'C');
		JPT_ENSURE(fooShared2->m_left == 42);
		JPT_ENSURE(fooShared2->m_right == 'C');
	}

	JPT_ENSURE(fooWeak.IsExpired());
	JPT_ENSURE(fooWeak.GetRefCount() == 0);

	return true;
}

jpt::WeakPtr<jpt::String> weakStringPtr1;
jpt::WeakPtr<jpt::String> weakStringPtr2;
jpt::WeakPtr<jpt::String> weakStringPtr3;
bool UnitTests_WeakPtr_Referencing()
{
	jpt::StrongPtr<jpt::String> strongStringPtr1 = jpt::MakeStrong<jpt::String>("Jupiter Engine");
	JPT_ENSURE(*strongStringPtr1 == "Jupiter Engine");

	weakStringPtr1 = strongStringPtr1;
	weakStringPtr2 = strongStringPtr1;
	weakStringPtr3 = strongStringPtr1;

	JPT_ENSURE(*weakStringPtr1 == "Jupiter Engine");
	JPT_ENSURE(*weakStringPtr2 == "Jupiter Engine");
	JPT_ENSURE(*weakStringPtr3 == "Jupiter Engine");
	JPT_ENSURE(weakStringPtr1.GetRefCount() == 1);
	JPT_ENSURE(weakStringPtr2.GetRefCount() == 1);
	JPT_ENSURE(weakStringPtr3.GetRefCount() == 1);

	{
		jpt::StrongPtr<jpt::String> strongStringPtr2 = jpt::MakeStrong<jpt::String>("Zixuan Shi");
		JPT_ENSURE(*strongStringPtr2 == "Zixuan Shi");

		weakStringPtr1 = strongStringPtr2;
		weakStringPtr2 = strongStringPtr2;
		weakStringPtr3 = strongStringPtr2;

		JPT_ENSURE(*weakStringPtr1 == "Zixuan Shi");
		JPT_ENSURE(*weakStringPtr2 == "Zixuan Shi");
		JPT_ENSURE(*weakStringPtr3 == "Zixuan Shi");
		JPT_ENSURE(weakStringPtr1.GetRefCount() == 1);
		JPT_ENSURE(weakStringPtr2.GetRefCount() == 1);
		JPT_ENSURE(weakStringPtr3.GetRefCount() == 1);
	}

	JPT_ENSURE(weakStringPtr1.IsExpired());
	JPT_ENSURE(weakStringPtr2.IsExpired());
	JPT_ENSURE(weakStringPtr3.IsExpired());

	weakStringPtr1 = strongStringPtr1;
	weakStringPtr2 = strongStringPtr1;
	weakStringPtr3 = strongStringPtr1;
	JPT_ENSURE(*weakStringPtr1 == "Jupiter Engine");
	JPT_ENSURE(*weakStringPtr2 == "Jupiter Engine");
	JPT_ENSURE(*weakStringPtr3 == "Jupiter Engine");
	JPT_ENSURE(weakStringPtr1.GetRefCount() == 1);
	JPT_ENSURE(weakStringPtr2.GetRefCount() == 1);
	JPT_ENSURE(weakStringPtr3.GetRefCount() == 1);

	return true;
}

export bool RunUnitTests_WeakPtr()
{
	JPT_ENSURE(UnitTests_WeakPtr_Class());

	JPT_ENSURE(weakStringPtr1.IsExpired());
	JPT_ENSURE(weakStringPtr2.IsExpired());
	JPT_ENSURE(weakStringPtr3.IsExpired());
	JPT_ENSURE(UnitTests_WeakPtr_Referencing());
	JPT_ENSURE(weakStringPtr1.IsExpired());
	JPT_ENSURE(weakStringPtr2.IsExpired());
	JPT_ENSURE(weakStringPtr3.IsExpired());

	return true;
}