// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

#include <initializer_list>

export module jpt.HashSet;

import jpt.Constants;
import jpt.DynamicArray;
import jpt.LinkedList;

export namespace jpt
{
	/** A hash set is a collection of unique elements. The order of the elements in a hash set is undefined. */
	template<typename _TData>
	class HashSet
	{
	public:
		using TData    = _TData;
		using TBucket  = LinkedList<TData>;
		using TBuckets = DynamicArray<TBucket>;

	private:
		TBuckets m_buckets;
		size_t   m_size = 0;

	public:

	};
}