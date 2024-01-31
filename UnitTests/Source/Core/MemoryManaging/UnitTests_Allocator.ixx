// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Allocator;

import jpt.Allocator;
import jpt.TypeDefs;
import jpt.Concepts;

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

	// Multi
	TNum* pMulti = jpt::Allocator<TNum>::Allocate(10);
	JPT_RETURN_FALSE_IF_ERROR(pMulti == nullptr, "jpt::Allocator<TestType>::Allocate(10) returned a nullptr");
	jpt::Allocator<TNum>::Deallocate(pMulti);

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

export bool RunUnitTests_Allocator()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_TAllocatorrivialType<uint8>(), "UnitTest_TAllocatorrivialType<uint8> Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_TAllocatorrivialType<int32>(), "UnitTest_TAllocatorrivialType<int32> Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_TAllocatorrivialType<float>(), "UnitTest_TAllocatorrivialType<float> Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_TAllocatorrivialType<double>(), "UnitTest_TAllocatorrivialType<float> Failed");

	return true;
}