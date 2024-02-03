// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Allocator;

import jpt.Allocator;
import jpt.TypeDefs;
import jpt.Concepts;
import jpt.Utilities;
import jpt.String;
import jpt.ToString;

template<jpt::Numeric TNum>
bool UnitTest_TAllocatorrivialType()
{
	// Single
	TNum* pSingle = jpt::Allocator<TNum>::Allocate();
	JPT_RETURN_FALSE_IF_ERROR(pSingle == nullptr, "jpt::Allocator<TestType>::Allocate() returned a nullptr");
	jpt::Allocator<TNum>::Deallocate(pSingle);

	// Single with value
	TNum* pSingleWithValue = jpt::Allocator<TNum>::AllocateWithValue(static_cast<TNum>(42));
	JPT_RETURN_FALSE_IF_ERROR(pSingleWithValue == nullptr, "jpt::Allocator<int>::AllocateWithValue(42) returned a nullptr");
	JPT_RETURN_FALSE_IF_ERROR(*pSingleWithValue != static_cast<TNum>(42), "jpt::Allocator<int>::AllocateWithValue(42) assigned a wrong number");
	jpt::Allocator<TNum>::Deallocate(pSingleWithValue);

	// Array
	constexpr size_t kArraySize = 10'000;
	TNum* pArray = jpt::Allocator<TNum>::AllocateArray(kArraySize);
	JPT_RETURN_FALSE_IF_ERROR(pArray == nullptr, "jpt::Allocator<TestType>::AllocateArray(kArraySize) returned a nullptr");

	for (size_t i = 0; i < kArraySize; ++i)
	{
		pArray[i] = static_cast<TNum>(i);
	}
	for (size_t i = 0; i < kArraySize; ++i)
	{
		JPT_RETURN_FALSE_IF_ERROR(pArray[i] != static_cast<TNum>(i), "jpt::Allocator<TestType>::AllocateArray(kArraySize) doesn't write data correctly");
	}
	jpt::Allocator<TNum>::DeallocateArray(pArray);

	// Multi with value initializer list
	TNum* pMultiWithValue = jpt::Allocator<TNum>::AllocateArray(10, { 0,1,2,3,4,5,6,7,8,9 });
	JPT_RETURN_FALSE_IF_ERROR(pMultiWithValue == nullptr, "jpt::Allocator<TestType>::jpt::Allocator<TestType>::AllocateMultiWithValue(10, { 0,1,2,3,4,5,6,7,8,9 }) returned a nullptr");
	for (size_t i = 0; i < 10; ++i)
	{
		JPT_RETURN_FALSE_IF_ERROR(pMultiWithValue[i] != static_cast<TNum>(i), "jpt::Allocator<TestType>::jpt::Allocator<TestType>::AllocateMultiWithValue(10, { 0,1,2,3,4,5,6,7,8,9 }) doesn't write data correctly");
	}
	jpt::Allocator<TNum>::Deallocate(pMultiWithValue);

	return true;
}

bool UnitTest_TAllocator_NonTrivial()
{
	// If uncomment the following JPT_LOGs, only one "Foo copy ctor called" should be printed

	class Foo
	{
	private:
		jpt::String m_str;

	public:
		Foo() 
		{
			//JPT_LOG("Foo default ctor called");
		}

		Foo(const jpt::String& str) 
			: m_str(str) 
		{
			//JPT_LOG("Foo String ctor called");
		}

		Foo(const Foo& other) 
			: m_str(other.m_str)
		{
			//JPT_LOG("Foo copy ctor called");
		}

		Foo(Foo&& other)
			: m_str(jpt::Move(other.m_str))
		{
			//JPT_LOG("Foo move ctor called");
		}

		Foo& operator=(const Foo& other) 
		{ 
			//JPT_LOG("Foo copy operator= called");
			m_str = other.m_str;
			return *this; 
		}

		Foo& operator=(Foo&& other) 
		{ 
			//JPT_LOG("Foo move operator= called");
			m_str = jpt::Move(other.m_str);
			return *this; 
		}

		~Foo() 
		{
			//JPT_LOG("Foo dtor called");
		}

		jpt::String ToString() const 
		{ 
			return "Foo" + m_str;
		}
	};

	jpt::Allocator<Foo> allocator;

	Foo* pFoo = allocator.Allocate();
	JPT_RETURN_FALSE_IF_ERROR(pFoo == nullptr, "");
	allocator.Deallocate(pFoo);

	Foo* pFooWithArgs = allocator.AllocateWithValue(jpt::String("Bar"));
	JPT_RETURN_FALSE_IF_ERROR(pFooWithArgs == nullptr, "");
	JPT_RETURN_FALSE_IF_ERROR(pFooWithArgs->ToString() != "FooBar", "");
	allocator.Deallocate(pFooWithArgs);

	Foo* pFooArray = allocator.AllocateArray(10, { Foo("0"), Foo("1"), Foo("2"), Foo("3"), Foo("4"), Foo("5"), Foo("6"), Foo("7"),Foo("8"), Foo("9") });
	JPT_RETURN_FALSE_IF_ERROR(pFooArray == nullptr, "");
	for (size_t i = 0; i < 10; ++i)
	{
		JPT_RETURN_FALSE_IF_ERROR(pFooArray[i].ToString() != "Foo" + jpt::ToString(i), "");
	}
	allocator.DeallocateArray(pFooArray);

	Foo* pConstructedFoo = allocator.Allocate();
	allocator.Construct(pConstructedFoo, Foo("Bar"));
	JPT_RETURN_FALSE_IF_ERROR(pConstructedFoo == nullptr, "");
	JPT_RETURN_FALSE_IF_ERROR(pConstructedFoo->ToString() != "FooBar", "");
	allocator.Destruct(pConstructedFoo);

	Foo anotherFoo = jpt::String("Baz");
	allocator.Construct(pConstructedFoo, anotherFoo);
	JPT_RETURN_FALSE_IF_ERROR(pConstructedFoo == nullptr, "");
	JPT_RETURN_FALSE_IF_ERROR(pConstructedFoo->ToString() != "FooBaz", "");
	allocator.Destruct(pConstructedFoo);

	allocator.Construct(pConstructedFoo, jpt::Move(anotherFoo));
	JPT_RETURN_FALSE_IF_ERROR(pConstructedFoo == nullptr, "");
	JPT_RETURN_FALSE_IF_ERROR(pConstructedFoo->ToString() != "FooBaz", "");
	allocator.Destruct(pConstructedFoo);

	allocator.Deallocate(pConstructedFoo);

	return true;
}

export bool RunUnitTests_Allocator()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_TAllocatorrivialType<uint8>(), "UnitTest_TAllocatorrivialType<uint8> Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_TAllocatorrivialType<int32>(), "UnitTest_TAllocatorrivialType<int32> Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_TAllocatorrivialType<float>(), "UnitTest_TAllocatorrivialType<float> Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_TAllocatorrivialType<double>(), "UnitTest_TAllocatorrivialType<float> Failed");

	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_TAllocator_NonTrivial(), "UnitTest_TAllocator_NonTrivial Failed");

	return true;
}