// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.HashMap;

import jpt.Pair;
import jpt.DynamicArray;
import jpt.LinkedList;
import jpt.Utilities;

import jpt_private.ChainedBucketIterator;

export namespace jpt
{
	/** Unordered map implementation with Chainning hash collision-handling  */
	template <typename _TKey, typename _TValue>
	class HashMap
	{
	public:
		using TKey          = _TKey;
		using TValue        = _TValue;
		using Element       = Pair<TKey, TValue>;
		using Bucket        = LinkedList<Element>;
		using Buckets 	    = DynamicArray<Bucket>;
		using Iterator      = jpt_private::ChainedBucketIterator<TKey, TValue>;
		using ConstIterator = jpt_private::ConstChainedBucketIterator<TKey, TValue>;

	private:
		Buckets m_buckets;

	public:
		constexpr HashMap() = default;
		constexpr ~HashMap();

		// Element Access
		

		// Iterators
		constexpr Iterator begin() noexcept { return m_buckets.begin(); }
		constexpr Iterator end()   noexcept { return m_buckets.end();   }
		constexpr ConstIterator begin()  const noexcept { return m_buckets.begin();  }
		constexpr ConstIterator end()    const noexcept { return m_buckets.end();    }
		constexpr ConstIterator cbegin() const noexcept { return m_buckets.cbegin(); }
		constexpr ConstIterator cend()   const noexcept { return m_buckets.cend();   }

	private:
	};

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::~HashMap()
	{
	}
}
