// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <queue>
#include <time.h>

export module UnitTests_Heap;

import jpt.Heap;
import jpt.TypeDefs;
import jpt.String;
import jpt.ToString;
import jpt.Comparators;
import jpt.DynamicArray;
import jpt.Rand;
import jpt.Math;

class Foo
{
private:
	int32 m_data = 0;

public:
	constexpr Foo() = default;
	constexpr Foo(int32 data) : m_data(data) {}

	constexpr int32 GetData() const { return m_data; }
	constexpr jpt::String ToString() const { return jpt::ToString(m_data); }
};

constexpr bool operator<(const Foo& lhs, const Foo& rhs)
{
	return lhs.GetData() < rhs.GetData();
}

constexpr bool operator>(const Foo& lhs, const Foo& rhs)
{
	return lhs.GetData() > rhs.GetData();
}

jpt::DynamicArray<int32> GetRandArray()
{
	jpt::DynamicArray<int32> arr;
	arr.Reserve(10);

	for (int32 i = 0; i < 10; ++i)
	{
		arr.EmplaceBack(i);
	}

	// Shuffle the array
	for (int32 i = 0; i < 10; ++i)
	{
		const size_t randIndex = jpt::RNG::Global().MaxInt(9);
		jpt::Swap(arr[i], arr[randIndex]);
	}

	return arr;
}

static bool MaxHeap()
{
	jpt::DynamicArray<int32> arr = GetRandArray();
	jpt::MaxHeap<Foo> heap;

	for (int32 i = 0; i < 10; ++i)
	{
		heap.Emplace(arr[i]);
	}

	for (int32 i = 9; i >= 0; --i)
	{
		const Foo& top = heap.Top();
		JPT_ENSURE(top.GetData() == i);
		heap.Pop();
	}

	return true;
}

static bool MinHeap()
{
	jpt::DynamicArray<int32> arr = GetRandArray();
	jpt::MinHeap<Foo> heap;

	for (int32 i = 0; i < 10; ++i)
	{
		heap.Emplace(arr[i]);
	}

	for (int32 i = 0; i < 10; ++i)
	{
		const Foo& top = heap.Top();
		JPT_ENSURE(top.GetData() == i);
		heap.Pop();
	}

	return true;
}

static bool CustomComparator()
{
	jpt::DynamicArray<int32> arr = GetRandArray();

	auto comparator = [](const Foo& lhs, const Foo&)
		{
			return jpt::IsEven(lhs.GetData());
		};

	jpt::PriorityQueue<Foo, decltype(comparator)> heap(comparator);

	for (int32 i = 0; i < 10; ++i)
	{
		heap.Emplace(arr[i]);
	}

	for (int32 i = 0; i < 10; ++i)
	{
		const Foo& top = heap.Top();

		if (i < 5)
		{
			JPT_ENSURE(jpt::IsEven(top.GetData()));
		}
		else
		{
			JPT_ENSURE(!jpt::IsEven(top.GetData()));
		}

		heap.Pop();
	}

	return true;
}

export bool RunUnitTests_Heap()
{
	JPT_ENSURE(MaxHeap());
	JPT_ENSURE(MinHeap());
	JPT_ENSURE(CustomComparator());

	return true;
}