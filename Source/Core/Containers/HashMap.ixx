// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.HashMap;

import jpt.Pair;
import jpt.DynamicArray;
import jpt.LinkedList;

import jpt_private.UnorderedPairedIterator;

export namespace jpt
{
	/** Unordered map implementation with Chainning hash collision-handling  */
	template <typename _TKey, typename _TValue>
	class HashMap
	{
	public:
		using TKey          = _TKey;
		using TValue        = _TValue;
		using Pair          = Pair<TKey, TValue>;
		using Iterator      = jpt_private::UnorderedPairedIterator<TKey, TValue>;
		using ConstIterator = jpt_private::ConstUnorderedPairedIterator<TKey, TValue>;

	private:



	};
}
