// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

#include <initializer_list>

export module jpt.HashMap;

import jpt.Pair;
import jpt.DynamicArray;
import jpt.LinkedList;
import jpt.Concepts;
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
		using ConstIterator = jpt_private::ConstChainedBucketIterator<TKey, TValue>;

	private:
		TBuckets m_buckets; 
		size_t m_size = 0;		/**< Count of actual elements in the map */

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
			If key doesn't exist, insert a default value, return the inserted value too */
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
		constexpr size_t Size()  const { return m_size; }
		constexpr bool IsEmpty() const { return m_size == 0; }

		// Modifiers
		constexpr void Clear();

		// Inserting
		constexpr TValue& Insert(const TKey& key, const TValue& value);
		constexpr TValue& Insert(const TData& element);

		// Erasing
		constexpr void Erase(const TKey& key);

		// Searching
		constexpr Iterator      Find(const TKey& key);
		constexpr ConstIterator Find(const TKey& key) const;
		constexpr bool Contains(const TKey& key) const;

	private:
		constexpr void ResizeBuckets(size_t capacity);

		constexpr size_t GetBucketIndex(const TKey& key) const;
		constexpr       TBucket& GetBucket(const TKey& key);
		constexpr const TBucket& GetBucket(const TKey& key) const;

		template<Iterable TContainer>
		constexpr void CopyData(const TContainer& container);
		constexpr void MoveMap(HashMap&& other);
	};

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::HashMap(const std::initializer_list<TData>& list)
	{
		CopyData(list);
	}

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::HashMap(const HashMap& other)
	{
		CopyData(other);
	}

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::HashMap(HashMap&& other) noexcept
	{
		MoveMap(Move(other));
	}

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>& HashMap<_TKey, _TValue>::operator=(const HashMap& other)
	{
		if (this != &other)
		{
			Clear();
			CopyData(other);
		}

		return *this;
	}

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>& HashMap<_TKey, _TValue>::operator=(HashMap&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			MoveMap(Move(other));
		}

		return *this;
	}

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::~HashMap()
	{
		Clear();
	}

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::TValue& HashMap<_TKey, _TValue>::operator[](const TKey& key)
	{
		Iterator itr = Find(key);
		if (itr == end())
		{
			return Insert(key, TValue());
		}

		return itr->second;
	}

	template<typename _TKey, typename _TValue>
	constexpr const HashMap<_TKey, _TValue>::TValue& HashMap<_TKey, _TValue>::operator[](const TKey& key) const
	{
		ConstIterator itr = Find(key);
		JPT_ASSERT(itr != cend());
		return itr->second;
	}

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::Iterator HashMap<_TKey, _TValue>::begin() noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return Iterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::Iterator HashMap<_TKey, _TValue>::end() noexcept
	{
		return Iterator(&m_buckets, m_buckets.Size(), nullptr);
	}

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::ConstIterator HashMap<_TKey, _TValue>::begin() const noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::ConstIterator HashMap<_TKey, _TValue>::end() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Size(), nullptr);
	}

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::ConstIterator HashMap<_TKey, _TValue>::cbegin() const noexcept
	{
		if (IsEmpty())
		{
			return cend();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().cbegin());
	}

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::ConstIterator HashMap<_TKey, _TValue>::cend() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Size(), nullptr);
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

		TBucket& bucket = GetBucket(key);

		// Check if the key already exists. If it does, update the value and return it
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

		// If the key does not exist, add and return it
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
	constexpr void HashMap<_TKey, _TValue>::Erase(const TKey& key)
	{
		TBucket& bucket = GetBucket(key);

		for (auto itr = bucket.begin(); itr != bucket.end(); ++itr)
		{
			if (itr->first == key)
			{
				bucket.Erase(itr);
				return;
			}
		}
	}

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::Iterator HashMap<_TKey, _TValue>::Find(const TKey& key)
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

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::ConstIterator HashMap<_TKey, _TValue>::Find(const TKey& key) const
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

	template<typename _TKey, typename _TValue>
	constexpr bool HashMap<_TKey, _TValue>::Contains(const TKey& key) const
	{
		return Find(key) != cend();
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

	template<typename _TKey, typename _TValue>
	constexpr HashMap<_TKey, _TValue>::TBucket& HashMap<_TKey, _TValue>::GetBucket(const TKey& key)
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename _TKey, typename _TValue>
	constexpr const HashMap<_TKey, _TValue>::TBucket& HashMap<_TKey, _TValue>::GetBucket(const TKey& key) const
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename _TKey, typename _TValue>
	template<Iterable TContainer>
	constexpr void HashMap<_TKey, _TValue>::CopyData(const TContainer& container)
	{
		for (const TData& data : container)
		{
			Insert(data);
		}
	}

	template<typename _TKey, typename _TValue>
	constexpr void HashMap<_TKey, _TValue>::MoveMap(HashMap&& other)
	{
		m_buckets = Move(other.m_buckets);
		m_size    = other.m_size;

		other.m_size = 0;
	}
}
