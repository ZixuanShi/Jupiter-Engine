// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.ChainedBucketIterator;

import jpt.TypeDefs;
import jpt.Pair;
import jpt.DynamicArray;
import jpt.LinkedList;

import jpt_private.LinearNodeIterator;

export namespace jpt_private
{
	/** Unordered Map iterator */
	template<typename TKey, typename TValue>
	class ChainedBucketIterator
	{
		using TData    = jpt::Pair<TKey, TValue>;
		using TBucket  = jpt::LinkedList<TData>;
		using TBuckets = jpt::DynamicArray<TBucket>;

	private:
		TBuckets::Iterator m_iterator;

	public:
		constexpr ChainedBucketIterator(TBuckets::Iterator iterator) : m_iterator(iterator) {}

	};
}
