// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

#include <initializer_list>

export module jpt_private.HashTable;

import jpt.Concepts;
import jpt.Constants;
import jpt.DynamicArray;
import jpt.LinkedList;
import jpt.Math;
import jpt.TypeTraits;
import jpt.Utilities;

import jpt_private.HashTableIterator;

namespace jpt
{
	/** Base class for Hash Table structures. Serves as base for HashSet and HashMap */
	export template<typename _TKey, typename _TValue>
	class HashTable
	{
	public:
		using TKey          = _TKey;
		using TValue        = _TValue;
		using TBucket       = LinkedList<TValue>;
		using TBuckets      = DynamicArray<TBucket>;
		using Iterator      = jpt_private::HashTableIterator<TValue>;
		using ConstIterator = jpt_private::ConstHashTableIterator<TValue>;

	protected:
		TBuckets m_buckets;
		size_t   m_size = 0;

	public:
		// Constructors
		constexpr HashTable() noexcept = default;
		constexpr HashTable(const std::initializer_list<TValue>& list);
		constexpr HashTable(const HashTable& other);
		constexpr HashTable(HashTable&& other) noexcept;
		constexpr HashTable& operator=(const HashTable& other);
		constexpr HashTable& operator=(HashTable&& other) noexcept;
		constexpr virtual ~HashTable();

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
		constexpr void Resize(size_t capacity);

		// Modifiers
		constexpr void Clear();

	protected:
		constexpr size_t GetBucketIndex(const TKey& key) const;
		constexpr       TBucket& GetBucket(const TKey& key);
		constexpr const TBucket& GetBucket(const TKey& key) const;

		template<Iterable TContainer>
		constexpr void CopyData(const TContainer& container, size_t size);

		constexpr void MoveTable(HashTable&& other);
	};

	template<typename TKey, typename TValue>
	constexpr HashTable<TKey, TValue>::HashTable(const std::initializer_list<TValue>& list)
	{
		CopyData(list, list.size());
	}

	template<typename TKey, typename TValue>
	constexpr HashTable<TKey, TValue>::HashTable(const HashTable& other)
	{
		CopyData(other, other.Size());
	}

	template<typename TKey, typename TValue>
	constexpr HashTable<TKey, TValue>::HashTable(HashTable&& other) noexcept
	{
		MoveTable(Move(other));
	}

	template<typename TKey, typename TValue>
	constexpr HashTable<TKey, TValue>& HashTable<TKey, TValue>::operator=(const HashTable& other)
	{
		if (this != &other)
		{
			Clear();
			CopyData(other, other.Size());
		}

		return *this;
	}

	template<typename TKey, typename TValue>
	constexpr HashTable<TKey, TValue>& HashTable<TKey, TValue>::operator=(HashTable&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			MoveTable(Move(other));
		}

		return *this;
	}

	template<typename TKey, typename TValue>
	constexpr HashTable<TKey, TValue>::~HashTable()
	{
		Clear();
	}

	template<typename TKey, typename TValue>
	constexpr HashTable<TKey, TValue>::Iterator HashTable<TKey, TValue>::begin() noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return Iterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename TKey, typename TValue>
	constexpr HashTable<TKey, TValue>::Iterator HashTable<TKey, TValue>::end() noexcept
	{
		return Iterator(&m_buckets, m_buckets.Size(), nullptr);
	}

	template<typename TKey, typename TValue>
	constexpr HashTable<TKey, TValue>::ConstIterator HashTable<TKey, TValue>::begin() const noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename TKey, typename TValue>
	constexpr HashTable<TKey, TValue>::ConstIterator HashTable<TKey, TValue>::end() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Size(), nullptr);
	}

	template<typename TKey, typename TValue>
	constexpr HashTable<TKey, TValue>::ConstIterator HashTable<TKey, TValue>::cbegin() const noexcept
	{
		if (IsEmpty())
		{
			return cend();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().cbegin());
	}

	template<typename TKey, typename TValue>
	constexpr HashTable<TKey, TValue>::ConstIterator HashTable<TKey, TValue>::cend() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Size(), nullptr);
	}

	template<typename TKey, typename TValue>
	constexpr void HashTable<TKey, TValue>::Resize(size_t capacity)
	{
		static constexpr size_t kMinCapacity = 8;

		TBuckets newBuckets = m_buckets;
		Clear();
		m_buckets.Resize(Max(kMinCapacity, capacity));

		for (const TBucket& bucket : newBuckets)
		{
			for (const TValue& element : bucket)
			{
				const size_t index = GetBucketIndex(element);
				m_buckets[index].EmplaceBack(element);
			}
		}
	}

	template<typename TKey, typename TValue>
	constexpr void HashTable<TKey, TValue>::Clear()
	{
		m_buckets.Clear();
		m_size = 0;
	}

	template<typename TKey, typename TValue>
	constexpr size_t HashTable<TKey, TValue>::GetBucketIndex(const TKey& key) const
	{
		return Hash<TKey>()(key) % m_buckets.Size();
	}

	template<typename TKey, typename TValue>
	constexpr HashTable<TKey, TValue>::TBucket& HashTable<TKey, TValue>::GetBucket(const TKey& key)
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename TKey, typename TValue>
	constexpr const HashTable<TKey, TValue>::TBucket& HashTable<TKey, TValue>::GetBucket(const TKey& key) const
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename TKey, typename TValue>
	template<Iterable TContainer>
	constexpr void HashTable<TKey, TValue>::CopyData(const TContainer& container, size_t size)
	{
		Resize(size);

		for (const TValue& data : container)
		{
			TBucket& bucket = GetBucket(data);
			bucket.EmplaceBack(data);
		}

		m_size += size;
	}

	template<typename TKey, typename TValue>
	constexpr void HashTable<TKey, TValue>::MoveTable(HashTable&& other)
	{
		m_buckets = Move(other.m_buckets);
		m_size = other.m_size;

		other.m_size = 0;
	}
}