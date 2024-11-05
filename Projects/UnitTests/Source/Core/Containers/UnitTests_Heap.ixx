// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <queue>

export module UnitTests_Heap;

import jpt.Heap;
import jpt.TypeDefs;
import jpt.String;
import jpt.ToString;
import jpt.Comparators;

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

static bool Constructing()
{
	jpt::Heap<Foo, jpt::Comparator_Greater<Foo>> heap;

	heap.Add(1);

	return true;
}

export bool RunUnitTests_Heap()
{
	JPT_ENSURE(Constructing());

	return true;
}