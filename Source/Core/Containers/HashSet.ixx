// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

#include <initializer_list>

export module jpt.HashSet;

import jpt.Constants;
import jpt.DynamicArray;
import jpt.LinkedList;
import jpt.Utilities;
import jpt.Math;

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
		constexpr HashSet() = default;
		constexpr ~HashSet();

		// Capacity
		constexpr size_t Size()  const { return m_size; }
		constexpr bool IsEmpty() const { return m_size == 0; }
		constexpr void Resize(size_t capacity);

		// Modifiers
		constexpr void Clear();
		constexpr TData& Add(const TData& data);
		constexpr void Erase(const TData& key);

		// Searching
		constexpr bool Contains(const TData& key) const;

	private:
		constexpr size_t GetBucketIndex(const TData& key) const;
		constexpr       TBucket& GetBucket(const TData& key);
		constexpr const TBucket& GetBucket(const TData& key) const;
	};

	template<typename _TData>
	constexpr HashSet<_TData>::~HashSet()
	{
		Clear();
	}

	template<typename _TData>
	constexpr void HashSet<_TData>::Resize(size_t capacity)
	{
		static constexpr size_t kMinCapacity = 8;

		TBuckets newBuckets;
		newBuckets.Resize(Max(kMinCapacity, capacity));

		for (const TBucket& bucket : m_buckets)
		{
			for (const TData& element : bucket)
			{
				const size_t index = GetBucketIndex(element);
				newBuckets[index].EmplaceBack(element);
			}
		}

		m_buckets = Move(newBuckets);
	}

	template<typename _TData>
	constexpr void HashSet<_TData>::Clear()
	{
		m_buckets.Clear();
		m_size = 0;	
	}

	template<typename _TData>
	constexpr HashSet<_TData>::TData& HashSet<_TData>::Add(const TData& data)
	{
		// Grow if needed
		if (m_buckets.IsEmpty())
		{
			Resize(m_size * kGrowMultiplier);
		}

		TBucket& bucket = GetBucket(data);

		// Check if the key already exists. If it does, return it
		for (TData& element : bucket)
		{
			if (element == data)
			{
				return element;
			}
		}

		// If the key does not exist, add and return it
		++m_size;
		return bucket.EmplaceBack(data);
	}

	template<typename _TData>
	constexpr void HashSet<_TData>::Erase(const TData& key)
	{
		if (IsEmpty())
		{
			return;
		}

		TBucket& bucket = GetBucket(key);

		for (auto it = bucket.begin(); it != bucket.end(); ++it)
		{
			if (*it == key)
			{
				bucket.Erase(it);
				--m_size;
				return;
			}
		}
	}

	template<typename _TData>
	constexpr bool HashSet<_TData>::Contains(const TData& key) const
	{
		if (IsEmpty())
		{
			return false;
		}

		const TBucket& bucket = GetBucket(key);

		for (const TData& element : bucket)
		{
			if (element == key)
			{
				return true;
			}
		}

		return false;
	}

	template<typename _TData>
	constexpr size_t HashSet<_TData>::GetBucketIndex(const TData& key) const
	{
		return Hash<TData>()(key) % m_buckets.Size();
	}

	template<typename _TData>
	constexpr HashSet<_TData>::TBucket& HashSet<_TData>::GetBucket(const TData& key)
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename _TData>
	constexpr const HashSet<_TData>::TBucket& HashSet<_TData>::GetBucket(const TData& key) const
	{
		return m_buckets[GetBucketIndex(key)];
	}
}