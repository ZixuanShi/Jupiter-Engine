// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

export module AllocatorUnitTests;

import jpt.CoreModules;

template<jpt::Numeric TestT>
bool UnitTest_AllocatorTrivialType()
{
	// Single
	TestT* pSingle = jpt::Allocator<TestT>::Allocate();
	JPT_RETURN_FALSE_IF_ERROR(pSingle == nullptr, "jpt::Allocator<TestType>::Allocate() returned a nullptr");
	jpt::Allocator<TestT>::Deallocate(pSingle);

	// Single with value
	TestT* pSingleWithValue = jpt::Allocator<TestT>::AllocateWithValue(static_cast<TestT>(42));
	JPT_RETURN_FALSE_IF_ERROR(pSingleWithValue == nullptr, "jpt::Allocator<int>::AllocateWithValue(42) returned a nullptr");
	JPT_RETURN_FALSE_IF_ERROR(*pSingleWithValue != static_cast<TestT>(42), "jpt::Allocator<int>::AllocateWithValue(42) assigned a wrong number");
	jpt::Allocator<TestT>::Deallocate(pSingleWithValue);

	// Multi
	TestT* pMulti = jpt::Allocator<TestT>::Allocate(10);
	JPT_RETURN_FALSE_IF_ERROR(pMulti == nullptr, "jpt::Allocator<TestType>::Allocate(10) returned a nullptr");
	jpt::Allocator<TestT>::Deallocate(pMulti);

	// Array
	constexpr size_t kArraySize = 10'000;
	TestT* pArray = jpt::Allocator<TestT>::AllocateArray(kArraySize);
	JPT_RETURN_FALSE_IF_ERROR(pArray == nullptr, "jpt::Allocator<TestType>::AllocateArray(kArraySize) returned a nullptr");

	for (size_t i = 0; i < kArraySize; ++i)
	{
		pArray[i] = static_cast<TestT>(i);
	}
	for (size_t i = 0; i < kArraySize; ++i)
	{
		JPT_RETURN_FALSE_IF_ERROR(pArray[i] != static_cast<TestT>(i), "jpt::Allocator<TestType>::AllocateArray(kArraySize) doesn't write data correctly");
	}
	jpt::Allocator<TestT>::DeallocateArray(pArray);

	// Multi with value initializer list
	TestT* pMultiWithValue = jpt::Allocator<TestT>::AllocateMultiWithValue(10, { 0,1,2,3,4,5,6,7,8,9 });
	JPT_RETURN_FALSE_IF_ERROR(pMultiWithValue == nullptr, "jpt::Allocator<TestType>::jpt::Allocator<TestType>::AllocateMultiWithValue(10, { 0,1,2,3,4,5,6,7,8,9 }) returned a nullptr");
	for (size_t i = 0; i < 10; ++i)
	{
		JPT_RETURN_FALSE_IF_ERROR(pMultiWithValue[i] != static_cast<TestT>(i), "jpt::Allocator<TestType>::jpt::Allocator<TestType>::AllocateMultiWithValue(10, { 0,1,2,3,4,5,6,7,8,9 }) doesn't write data correctly");
	}
	jpt::Allocator<TestT>::Deallocate(pMultiWithValue);

	return true;
}

export bool RunAllocatorUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_AllocatorTrivialType<uint8>(), "UnitTest_AllocatorTrivialType<uint8> Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_AllocatorTrivialType<int32>(), "UnitTest_AllocatorTrivialType<int32> Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_AllocatorTrivialType<float>(), "UnitTest_AllocatorTrivialType<float> Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_AllocatorTrivialType<double>(), "UnitTest_AllocatorTrivialType<float> Failed");

	return true;
}