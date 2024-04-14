// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

#include <initializer_list>

export module jpt.HashMap;

import jpt.Constants;
import jpt.Pair;
import jpt.DynamicArray;
import jpt.LinkedList;
import jpt.Concepts;
import jpt.Utilities;
import jpt.Math;

import jpt_private.HashTableIterator;

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
		using Iterator	    = jpt_private::HashTableIterator<TData>;
		using ConstIterator = jpt_private::ConstHashTableIterator<TData>;

	private:
		TBuckets m_buckets; 
		size_t m_count = 0;		/**< Count of actual elements in the map */

	public:
		constexpr HashMap() = default;
		constexpr HashMap(const std::initializer_list<TData>& list);
		constexpr HashMap(const HashMap& other);
		constexpr HashMap(HashMap&& other) noexcept;
		constexpr HashMap& operator=(const HashMap& other);
		constexpr HashMap& operator=(HashMap&& other) noexcept;
		constexpr ~HashMap();
		
		// Element access
		/** If key exists, return reference to it's associated value, caller may update it outside
			If key doesn't exist, Add a default value, return the inserted value too */
		constexpr       TValue& operator[](const TKey& key);

		/** If key exists, return reference to it's associated value, caller can't update it outside
			If key doesn't exist, assertion fails */
		constexpr const TValue& operator[](const TKey& key) const;

		// Iterators	
		constexpr Iterator begin() noexcept;
		constexpr Iterator end()   noexcept;
		constexpr ConstIterator begin()  const noexcept;
		constexpr ConstIterator end()    const noexcept;
		constexpr ConstIterator cbegin() const noexcept;
		constexpr ConstIterator cend()   const noexcept;

		// Capacity
		constexpr size_t Count()  const { return m_count; }
		constexpr bool IsEmpty() const { return m_count == 0; }
		constexpr void Reserve(size_t capacity);

		// Modifiers
		constexpr TValue& Add(const TKey& key, const TValue& value);
		constexpr TValue& Add(const TData& element);
		constexpr void Erase(const TKey& key);
		constexpr void Clear();

		// Searching
		constexpr Iterator      Find(const TKey& key);
		constexpr ConstIterator Find(const TKey& key) const;
		constexpr bool Contains(const TKey& key) const;

	private:
		constexpr size_t GetBucketIndex(const TKey& key) const;
		constexpr       TBucket& GetBucket(const TKey& key);
		constexpr const TBucket& GetBucket(const TKey& key) const;

		template<Iterable TContainer>
		constexpr void CopyData(const TContainer& container, size_t size);
		constexpr void MoveMap(HashMap&& other);
	};

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>::HashMap(const std::initializer_list<TData>& list)
	{
		CopyData(list, list.size());
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>::HashMap(const HashMap& other)
	{
		CopyData(other, other.Count());
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>::HashMap(HashMap&& other) noexcept
	{
		MoveMap(Move(other));
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>& HashMap<TKey, TValue>::operator=(const HashMap& other)
	{
		if (this != &other)
		{
			Clear();
			CopyData(other, other.Count());
		}

		return *this;
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>& HashMap<TKey, TValue>::operator=(HashMap&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			MoveMap(Move(other));
		}

		return *this;
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>::~HashMap()
	{
		Clear();
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>::TValue& HashMap<TKey, TValue>::operator[](const TKey& key)
	{
		Iterator itr = Find(key);
		if (itr == end())
		{
			return Add(key, TValue());
		}

		return itr->second;
	}

	template<typename TKey, typename TValue>
	constexpr const HashMap<TKey, TValue>::TValue& HashMap<TKey, TValue>::operator[](const TKey& key) const
	{
		ConstIterator itr = Find(key);
		JPT_ASSERT(itr != cend());
		return itr->second;
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::begin() noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return Iterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::end() noexcept
	{
		return Iterator(&m_buckets, m_buckets.Count(), nullptr);
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::begin() const noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::end() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Count(), nullptr);
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::cbegin() const noexcept
	{
		if (IsEmpty())
		{
			return cend();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().cbegin());
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::cend() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Count(), nullptr);
	}

	template<typename TKey, typename TValue>
	constexpr void HashMap<TKey, TValue>::Clear()
	{
		m_buckets.Clear(); 
		m_count = 0;
	}

	template<typename TKey, typename TValue>
	constexpr TValue& HashMap<TKey, TValue>::Add(const TKey& key, const TValue& value)
	{
		// Grow if needed
		if (m_count >= m_buckets.Count() * kGrowMultiplier)
		{
			Reserve(m_count * kGrowMultiplier);
		}

		TBucket& bucket = GetBucket(key);

		// Check if the key already exists. If it does, update the value and return it
		for (TData& element : bucket)
		{
			if (element.first == key)
			{
				element.second = value;
				return element.second;
			}
		}

		// If the key does not exist, add and return it
		++m_count;
		TData& inserted = bucket.EmplaceBack(Pair{ key, value });
		return inserted.second;
	}

	template<typename TKey, typename TValue>
	constexpr TValue& HashMap<TKey, TValue>::Add(const TData& element)
	{
		return Add(element.first, element.second);
	}

	template<typename TKey, typename TValue>
	constexpr void HashMap<TKey, TValue>::Erase(const TKey& key)
	{
		if (IsEmpty())
		{
			return;
		}

		TBucket& bucket = GetBucket(key);

		for (auto itr = bucket.begin(); itr != bucket.end(); ++itr)
		{
			if (itr->first == key)
			{
				bucket.Erase(itr);
				--m_count;
				return;
			}
		}
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::Find(const TKey& key)
	{
		if (IsEmpty())
		{
			return end();
		}

		const size_t index = GetBucketIndex(key);
		TBucket& bucket = m_buckets[index];

		for (auto itr = bucket.begin(); itr != bucket.end(); ++itr)
		{
			if (itr->first == key)
			{
				return Iterator(&m_buckets, index, itr);
			}
		}

		return end();
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::Find(const TKey& key) const
	{
		if (IsEmpty())
		{
			return cend();
		}

		const size_t index = GetBucketIndex(key);
		const TBucket& bucket = m_buckets[index];

		for (auto itr = bucket.cbegin(); itr != bucket.cend(); ++itr)
		{
			if (itr->first == key)
			{
				return ConstIterator(&m_buckets, index, itr);
			}
		}

		return cend();
	}

	template<typename TKey, typename TValue>
	constexpr bool HashMap<TKey, TValue>::Contains(const TKey& key) const
	{
		return Find(key) != cend();
	}

	template<typename TKey, typename TValue>
	constexpr void HashMap<TKey, TValue>::Reserve(size_t capacity)
	{
		static constexpr size_t kMinCapacity = 8;

		TBuckets oldDataCopy = m_buckets;
		m_buckets.Clear();
		m_buckets.Resize(Max(kMinCapacity, capacity));

		for (const TBucket& bucket : oldDataCopy)
		{
			for (const TData& element : bucket)
			{
				const size_t index = GetBucketIndex(element.first);
				m_buckets[index].EmplaceBack(element);
			}
		}
	}

	template<typename TKey, typename TValue>
	constexpr size_t HashMap<TKey, TValue>::GetBucketIndex(const TKey& key) const
	{
		return Hash<TKey>()(key) % m_buckets.Count();
	}

	template<typename TKey, typename TValue>
	constexpr HashMap<TKey, TValue>::TBucket& HashMap<TKey, TValue>::GetBucket(const TKey& key)
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename TKey, typename TValue>
	constexpr const HashMap<TKey, TValue>::TBucket& HashMap<TKey, TValue>::GetBucket(const TKey& key) const
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename TKey, typename TValue>
	template<Iterable TContainer>
	constexpr void HashMap<TKey, TValue>::CopyData(const TContainer& container, size_t size)
	{
		Reserve(size);

		for (const TData& element : container)
		{
			TBucket& bucket = GetBucket(element.first);
			bucket.EmplaceBack(element);
		}

		m_count += size;
	}

	template<typename TKey, typename TValue>
	constexpr void HashMap<TKey, TValue>::MoveMap(HashMap&& other)
	{
		m_buckets = Move(other.m_buckets);
		m_count    = other.m_count;

		other.m_count = 0;
	}
}
