// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

export module AllocatorUnitTests;

import jpt.CoreModules;

template<jpt::Numeric TestType>
bool UnitTest_AllocatorTrivialType()
{
	// Single
	TestType* pSingle = jpt::Allocator<TestType>::Allocate();
	JPT_RETURN_FALSE_IF_LOG(pSingle == nullptr, "jpt::Allocator<TestType>::Allocate() returned a nullptr");
	jpt::Allocator<TestType>::Deallocate(pSingle);

	// Single with value
	TestType* pSingleWithValue = jpt::Allocator<TestType>::AllocateWithValue(static_cast<TestType>(42));
	JPT_RETURN_FALSE_IF_LOG(pSingleWithValue == nullptr, "jpt::Allocator<int>::AllocateWithValue(42) returned a nullptr");
	JPT_RETURN_FALSE_IF_LOG(*pSingleWithValue != static_cast<TestType>(42), "jpt::Allocator<int>::AllocateWithValue(42) assigned a wrong number");
	jpt::Allocator<TestType>::Deallocate(pSingleWithValue);

	// Multi
	TestType* pMulti = jpt::Allocator<TestType>::Allocate(10);
	JPT_RETURN_FALSE_IF_LOG(pMulti == nullptr, "jpt::Allocator<TestType>::Allocate(10) returned a nullptr");
	jpt::Allocator<TestType>::Deallocate(pMulti);

	// Array
	constexpr size_t kArraySize = 10'000;
	TestType* pArray = jpt::Allocator<TestType>::AllocateArray(kArraySize);
	JPT_RETURN_FALSE_IF_LOG(pArray == nullptr, "jpt::Allocator<TestType>::AllocateArray(kArraySize) returned a nullptr");

	for (size_t i = 0; i < kArraySize; ++i)
	{
		pArray[i] = static_cast<TestType>(i);
	}
	for (size_t i = 0; i < kArraySize; ++i)
	{
		JPT_RETURN_FALSE_IF_LOG(pArray[i] != static_cast<TestType>(i), "jpt::Allocator<TestType>::AllocateArray(kArraySize) doesn't write data correctly");
	}
	jpt::Allocator<TestType>::DeallocateArray(pArray);

	// Multi with value initializer list
	TestType* pMultiWithValue = jpt::Allocator<TestType>::AllocateMultiWithValue(10, { 0,1,2,3,4,5,6,7,8,9 });
	JPT_RETURN_FALSE_IF_LOG(pMultiWithValue == nullptr, "jpt::Allocator<TestType>::jpt::Allocator<TestType>::AllocateMultiWithValue(10, { 0,1,2,3,4,5,6,7,8,9 }) returned a nullptr");
	for (size_t i = 0; i < 10; ++i)
	{
		JPT_RETURN_FALSE_IF_LOG(pMultiWithValue[i] != static_cast<TestType>(i), "jpt::Allocator<TestType>::jpt::Allocator<TestType>::AllocateMultiWithValue(10, { 0,1,2,3,4,5,6,7,8,9 }) doesn't write data correctly");
	}
	jpt::Allocator<TestType>::Deallocate(pMultiWithValue);

	return true;
}

export void RunAllocatorUnitTests()
{
	JPT_LOG_IF(!UnitTest_AllocatorTrivialType<uint8>(),  "UnitTest_AllocatorTrivialType<uint8> Failed");
	JPT_LOG_IF(!UnitTest_AllocatorTrivialType<int32>(),  "UnitTest_AllocatorTrivialType<int32> Failed");
	JPT_LOG_IF(!UnitTest_AllocatorTrivialType<float>(),  "UnitTest_AllocatorTrivialType<float> Failed");
	JPT_LOG_IF(!UnitTest_AllocatorTrivialType<double>(), "UnitTest_AllocatorTrivialType<float> Failed");


}