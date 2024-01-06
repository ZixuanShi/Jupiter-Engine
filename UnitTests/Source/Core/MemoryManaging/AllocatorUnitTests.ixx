// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

export module AllocatorUnitTests;

import jpt.CoreModules;

template<jpt::Numeric TestType>
bool UnitTest_AllocatorTrivialType()
{
	TestType* pSingle = jpt::Allocator<TestType>::Allocate();
	JPT_RETURN_FALSE_IF_LOG(pSingle == nullptr, "jpt::Allocator<TestType>::Allocate() returned a nullptr");
	jpt::Allocator<TestType>::Deallocate(pSingle);

	TestType* pSingleWithValue = jpt::Allocator<TestType>::AllocateWithValue(static_cast<TestType>(42));
	JPT_RETURN_FALSE_IF_LOG(pSingleWithValue == nullptr, "jpt::Allocator<int>::AllocateWithValue(42) returned a nullptr");
	JPT_RETURN_FALSE_IF_LOG(*pSingleWithValue != static_cast<TestType>(42), "jpt::Allocator<int>::AllocateWithValue(42) assigned a wrong number");
	jpt::Allocator<TestType>::Deallocate(pSingleWithValue);

	TestType* pMulti = jpt::Allocator<TestType>::Allocate(10);
	JPT_RETURN_FALSE_IF_LOG(pMulti == nullptr, "jpt::Allocator<TestType>::Allocate(10) returned a nullptr");
	jpt::Allocator<TestType>::Deallocate(pMulti);

	TestType* pArray = jpt::Allocator<TestType>::AllocateArray(100);
	JPT_RETURN_FALSE_IF_LOG(pArray == nullptr, "jpt::Allocator<TestType>::AllocateArray(100) returned a nullptr");
	jpt::Allocator<TestType>::DeallocateArray(pArray);

	return true;
}

export void RunAllocatorUnitTests()
{
	JPT_LOG_IF(!UnitTest_AllocatorTrivialType<uint8>(),  "UnitTest_AllocatorTrivialType<uint8> Failed");
	JPT_LOG_IF(!UnitTest_AllocatorTrivialType<int32>(),  "UnitTest_AllocatorTrivialType<int32> Failed");
	JPT_LOG_IF(!UnitTest_AllocatorTrivialType<float>(),  "UnitTest_AllocatorTrivialType<float> Failed");
	JPT_LOG_IF(!UnitTest_AllocatorTrivialType<double>(), "UnitTest_AllocatorTrivialType<float> Failed");


}