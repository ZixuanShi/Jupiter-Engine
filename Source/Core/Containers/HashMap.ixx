// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.HashMap;

import jpt.Pair;
import jpt.DynamicArray;
import jpt.LinkedList;
import jpt.Utilities;
import jpt.Math;
import jpt.Searching;

import jpt_private.ChainedBucketIterator;

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
		using TData         = Pair<TKey, TValue>;
		using TBucket       = LinkedList<TData>;
		using TBuckets 	    = DynamicArray<TBucket>;
		using Iterator	    = jpt_private::ChainedBucketIterator<TKey, TValue>;

	private:
		TBuckets m_buckets; 
		size_t m_size = 0;		// Count of actual elements in the map

	public:
		constexpr HashMap() = default;
		constexpr ~HashMap();
		
		// Element access


		// Iterators	
		//constexpr Iterator begin() noexcept { return Iterator(m_buckets.begin()); }
		//constexpr Iterator end()   noexcept { return Iterator(m_buckets.end()); }

		// Modifiers
		constexpr void Clear();

		// Inserting
		constexpr TValue& Insert(const TKey& key, const TValue& value);
		constexpr TValue& Insert(const TData& element);

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

	template<typename TKey, typename TValue>
	constexpr TValue& HashMap<TKey, TValue>::Insert(const TKey& key, const TValue& value)
	{
		// Grow if needed
		if (m_size >= m_buckets.Size() * kLocGrowMultiplier)
		{
			ResizeBuckets(m_size * kLocGrowMultiplier);
		}

		const size_t index = GetBucketIndex(key);
		TBucket& bucket = m_buckets[index];

		// Check if the key already exists. If it does, update the value
		bool found = false;
		for (TData& element : bucket)
		{
			if (element.first == key)
			{
				found = true;
				element.second = value;
				return element.second;
			}
		}

		// If the key does not exist, add it
		if (!found)
		{
			bucket.EmplaceBack(Pair{ key ,value });
			++m_size;
		}

		return bucket.Back().second;
	}

	template<typename TKey, typename TValue>
	constexpr TValue& HashMap<TKey, TValue>::Insert(const TData& element)
	{
		return Insert(element.first, element.second);
	}

	template<typename _TKey, typename _TValue>
	constexpr bool HashMap<_TKey, _TValue>::Contains(const TKey& key) const
	{
		const size_t index = GetBucketIndex(key);
		const TBucket& bucket = m_buckets[index];

		for (const TData& element : bucket)
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
			for (const TData& element : bucket)
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
