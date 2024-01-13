// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

export module AllocatorUnitTests;

import jpt.CoreModules;

template<jpt::Numeric NumT>
bool UnitTest_AllocatorTrivialType()
{
	// Single
	NumT* pSingle = jpt::Allocator<NumT>::Allocate();
	JPT_RETURN_FALSE_IF_ERROR(pSingle == nullptr, "jpt::Allocator<TestType>::Allocate() returned a nullptr");
	jpt::Allocator<NumT>::Deallocate(pSingle);

	// Single with value
	NumT* pSingleWithValue = jpt::Allocator<NumT>::AllocateWithValue(static_cast<NumT>(42));
	JPT_RETURN_FALSE_IF_ERROR(pSingleWithValue == nullptr, "jpt::Allocator<int>::AllocateWithValue(42) returned a nullptr");
	JPT_RETURN_FALSE_IF_ERROR(*pSingleWithValue != static_cast<NumT>(42), "jpt::Allocator<int>::AllocateWithValue(42) assigned a wrong number");
	jpt::Allocator<NumT>::Deallocate(pSingleWithValue);

	// Multi
	NumT* pMulti = jpt::Allocator<NumT>::Allocate(10);
	JPT_RETURN_FALSE_IF_ERROR(pMulti == nullptr, "jpt::Allocator<TestType>::Allocate(10) returned a nullptr");
	jpt::Allocator<NumT>::Deallocate(pMulti);

	// Array
	constexpr size_t kArraySize = 10'000;
	NumT* pArray = jpt::Allocator<NumT>::AllocateArray(kArraySize);
	JPT_RETURN_FALSE_IF_ERROR(pArray == nullptr, "jpt::Allocator<TestType>::AllocateArray(kArraySize) returned a nullptr");

	for (size_t i = 0; i < kArraySize; ++i)
	{
		pArray[i] = static_cast<NumT>(i);
	}
	for (size_t i = 0; i < kArraySize; ++i)
	{
		JPT_RETURN_FALSE_IF_ERROR(pArray[i] != static_cast<NumT>(i), "jpt::Allocator<TestType>::AllocateArray(kArraySize) doesn't write data correctly");
	}
	jpt::Allocator<NumT>::DeallocateArray(pArray);

	// Multi with value initializer list
	NumT* pMultiWithValue = jpt::Allocator<NumT>::AllocateArray(10, { 0,1,2,3,4,5,6,7,8,9 });
	JPT_RETURN_FALSE_IF_ERROR(pMultiWithValue == nullptr, "jpt::Allocator<TestType>::jpt::Allocator<TestType>::AllocateMultiWithValue(10, { 0,1,2,3,4,5,6,7,8,9 }) returned a nullptr");
	for (size_t i = 0; i < 10; ++i)
	{
		JPT_RETURN_FALSE_IF_ERROR(pMultiWithValue[i] != static_cast<NumT>(i), "jpt::Allocator<TestType>::jpt::Allocator<TestType>::AllocateMultiWithValue(10, { 0,1,2,3,4,5,6,7,8,9 }) doesn't write data correctly");
	}
	jpt::Allocator<NumT>::Deallocate(pMultiWithValue);

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