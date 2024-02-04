// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.HashMap;

import jpt.Pair;
import jpt.DynamicArray;
import jpt.LinkedList;
import jpt.Utilities;
import jpt.Math;

static constexpr size_t kLocGrowMultiplier = 2;

export namespace jpt
{
	/** Unordered map implementation with Chainning hash collision-handling  */
	template <typename _TKey, typename _TValue>
	class HashMap
	{
	public:
		using TKey          = _TKey;
		using TValue        = _TValue;
		using TElement      = Pair<TKey, TValue>;
		using TBucket       = LinkedList<TElement>;
		using TBuckets 	    = DynamicArray<TBucket>;

	private:
		TBuckets m_buckets;
		size_t m_size = 0;		// Count of actual elements in the map

	public:
		constexpr HashMap() = default;
		constexpr ~HashMap();
		
		// Modifiers
		constexpr void Clear();

		// Inserting
		constexpr void Insert(const TKey& key, const TValue& value);
		constexpr void Insert(const TElement& element);

		// Erasing


		// Lookup
		constexpr bool Contains(const TKey& key) const;

	private:
		constexpr void ResizeBuckets(size_t capacity);
		constexpr size_t GetBucketIndex(const TKey& key) const;
	};

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::~HashMap()
	{
		Clear();
	}

	template<typename _TKey, typename _TValue>
	constexpr void HashMap<_TKey, _TValue>::Clear()
	{
		m_buckets.Clear(); 
		m_size = 0;
	}

	template<typename _TKey, typename _TValue>
	constexpr void HashMap<_TKey, _TValue>::Insert(const TKey& key, const TValue& value)
	{
		Insert(TElement{ key, value });
	}

	template<typename _TKey, typename _TValue>
	constexpr void HashMap<_TKey, _TValue>::Insert(const TElement& element)
	{
		// Grow if needed
		if (m_size >= m_buckets.Size() * kLocGrowMultiplier)
		{
			ResizeBuckets(m_size * kLocGrowMultiplier);
		}

		const size_t index = GetBucketIndex(element.first);

			// insert the value
			m_buckets[index].PushBack(element);
	}

	template<typename _TKey, typename _TValue>
	constexpr bool HashMap<_TKey, _TValue>::Contains(const TKey& key) const
	{
		const size_t index = GetBucketIndex(key);
		const TBucket& bucket = m_buckets[index];

		for (const TElement& element : bucket)
		{
			if (element.first == key)
			{
				return true;
			}
		}

		return false;
	}

	template<typename _TKey, typename _TValue>
	constexpr void HashMap<_TKey, _TValue>::ResizeBuckets(size_t capacity)
	{
		static constexpr size_t kMinCapacity = 8;

		TBuckets newBuckets;
		newBuckets.Resize(Max(kMinCapacity, capacity));

		for (const TBucket& bucket : m_buckets)
		{
			for (const TElement& element : bucket)
			{
				const size_t index = GetBucketIndex(element.first);
				newBuckets[index].EmplaceBack(element);
			}
		}

		m_buckets = Move(newBuckets);
	}

	template<typename _TKey, typename _TValue>
	constexpr size_t HashMap<_TKey, _TValue>::GetBucketIndex(const TKey& key) const
	{
		return Hash<TKey>()(key) % m_buckets.Size();
	}
}
