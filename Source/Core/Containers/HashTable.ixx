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
import jpt.Utilities;
import jpt.Math;

import jpt_private.HashTableIterator;

namespace jpt
{
	/** Base class for Hash Table structures. Serves as base for HashSet and HashMap */
	export template<typename _TData>
	class HashTable
	{
	public:
		using TData         = _TData;
		using TBucket       = LinkedList<TData>;
		using TBuckets      = DynamicArray<TBucket>;
		using Iterator      = jpt_private::HashTableIterator<TData>;
		using ConstIterator = jpt_private::ConstHashTableIterator<TData>;

	protected:
		TBuckets m_buckets;
		size_t   m_size = 0;

	public:
		// Constructors
		constexpr HashTable() noexcept = default;
		constexpr HashTable(const std::initializer_list<TData>& list);
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
		template<typename TKey = TData>
		constexpr size_t GetBucketIndex(const TKey& key) const;

		template<typename TKey = TData>
		constexpr       TBucket& GetBucket(const TKey& key);

		template<typename TKey = TData>
		constexpr const TBucket& GetBucket(const TKey& key) const;

		template<Iterable TContainer>
		constexpr void CopyData(const TContainer& container, size_t size);

		constexpr void MoveTable(HashTable&& other);
	};

	template<typename TData>
	constexpr HashTable<TData>::HashTable(const std::initializer_list<TData>& list)
	{
		CopyData(list, list.size());
	}

	template<typename TData>
	constexpr HashTable<TData>::HashTable(const HashTable& other)
	{
		CopyData(other, other.Size());
	}

	template<typename TData>
	constexpr HashTable<TData>::HashTable(HashTable&& other) noexcept
	{
		MoveTable(Move(other));
	}

	template<typename TData>
	constexpr HashTable<TData>& HashTable<TData>::operator=(const HashTable& other)
	{
		if (this != &other)
		{
			Clear();
			CopyData(other, other.Size());
		}

		return *this;
	}

	template<typename TData>
	constexpr HashTable<TData>& HashTable<TData>::operator=(HashTable&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			MoveTable(Move(other));
		}

		return *this;
	}

	template<typename TData>
	constexpr HashTable<TData>::~HashTable()
	{
		Clear();
	}

	template<typename TData>
	constexpr HashTable<TData>::Iterator HashTable<TData>::begin() noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return Iterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename TData>
	constexpr HashTable<TData>::Iterator HashTable<TData>::end() noexcept
	{
		return Iterator(&m_buckets, m_buckets.Size(), nullptr);
	}

	template<typename TData>
	constexpr HashTable<TData>::ConstIterator HashTable<TData>::begin() const noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename TData>
	constexpr HashTable<TData>::ConstIterator HashTable<TData>::end() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Size(), nullptr);
	}

	template<typename TData>
	constexpr HashTable<TData>::ConstIterator HashTable<TData>::cbegin() const noexcept
	{
		if (IsEmpty())
		{
			return cend();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().cbegin());
	}

	template<typename TData>
	constexpr HashTable<TData>::ConstIterator HashTable<TData>::cend() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Size(), nullptr);
	}

	template<typename TData>
	constexpr void HashTable<TData>::Resize(size_t capacity)
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

	template<typename TData>
	constexpr void HashTable<TData>::Clear()
	{
		m_buckets.Clear();
		m_size = 0;
	}

	template<typename TData>
	template<typename TKey>
	constexpr size_t HashTable<TData>::GetBucketIndex(const TKey& key) const
	{
		return Hash<TKey>()(key) % m_buckets.Size();
	}

	template<typename TData>
	template<typename TKey>
	constexpr HashTable<TData>::TBucket& HashTable<TData>::GetBucket(const TKey& key)
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename TData>
	template<typename TKey>
	constexpr const HashTable<TData>::TBucket& HashTable<TData>::GetBucket(const TKey& key) const
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename TData>
	template<Iterable TContainer>
	constexpr void HashTable<TData>::CopyData(const TContainer& container, size_t size)
	{
		Resize(size);

		for (const TData& data : container)
		{
			TBucket& bucket = GetBucket(data);
			bucket.EmplaceBack(data);
		}

		m_size += size;
	}

	template<typename TData>
	constexpr void HashTable<TData>::MoveTable(HashTable&& other)
	{
		m_buckets = Move(other.m_buckets);
		m_size = other.m_size;

		other.m_size = 0;
	}
}