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
		constexpr size_t GetBucketIndex(const TData& key) const;
		constexpr       TBucket& GetBucket(const TData& key);
		constexpr const TBucket& GetBucket(const TData& key) const;

		template<Iterable TContainer>
		constexpr void CopyData(const TContainer& container, size_t size);

		constexpr void MoveTable(HashTable&& other);
	};

	template<typename _TData>
	constexpr HashTable<_TData>::HashTable(const std::initializer_list<TData>& list)
	{
		CopyData(list, list.size());
	}

	template<typename _TData>
	constexpr HashTable<_TData>::HashTable(const HashTable& other)
	{
		CopyData(other, other.Size());
	}

	template<typename _TData>
	constexpr HashTable<_TData>::HashTable(HashTable&& other) noexcept
	{
		MoveTable(Move(other));
	}

	template<typename _TData>
	constexpr HashTable<_TData>& HashTable<_TData>::operator=(const HashTable& other)
	{
		if (this != &other)
		{
			Clear();
			CopyData(other, other.Size());
		}

		return *this;
	}

	template<typename _TData>
	constexpr HashTable<_TData>& HashTable<_TData>::operator=(HashTable&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			MoveTable(Move(other));
		}

		return *this;
	}

	template<typename _TData>
	constexpr HashTable<_TData>::~HashTable()
	{
		Clear();
	}

	template<typename _TData>
	constexpr HashTable<_TData>::Iterator HashTable<_TData>::begin() noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return Iterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename _TData>
	constexpr HashTable<_TData>::Iterator HashTable<_TData>::end() noexcept
	{
		return Iterator(&m_buckets, m_buckets.Size(), nullptr);
	}

	template<typename _TData>
	constexpr HashTable<_TData>::ConstIterator HashTable<_TData>::begin() const noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename _TData>
	constexpr HashTable<_TData>::ConstIterator HashTable<_TData>::end() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Size(), nullptr);
	}

	template<typename _TData>
	constexpr HashTable<_TData>::ConstIterator HashTable<_TData>::cbegin() const noexcept
	{
		if (IsEmpty())
		{
			return cend();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().cbegin());
	}

	template<typename _TData>
	constexpr HashTable<_TData>::ConstIterator HashTable<_TData>::cend() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Size(), nullptr);
	}

	template<typename _TData>
	constexpr void HashTable<_TData>::Resize(size_t capacity)
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
	constexpr void HashTable<_TData>::Clear()
	{
		m_buckets.Clear();
		m_size = 0;
	}

	template<typename _TData>
	constexpr size_t HashTable<_TData>::GetBucketIndex(const TData& key) const
	{
		return Hash<TData>()(key) % m_buckets.Size();
	}

	template<typename _TData>
	constexpr HashTable<_TData>::TBucket& HashTable<_TData>::GetBucket(const TData& key)
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename _TData>
	constexpr const HashTable<_TData>::TBucket& HashTable<_TData>::GetBucket(const TData& key) const
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename _TData>
	template<Iterable TContainer>
	constexpr void HashTable<_TData>::CopyData(const TContainer& container, size_t size)
	{
		Resize(size);

		for (const TData& data : container)
		{
			TBucket& bucket = GetBucket(data);
			bucket.EmplaceBack(data);
		}

		m_size += size;
	}

	template<typename _TData>
	constexpr void HashTable<_TData>::MoveTable(HashTable&& other)
	{
		m_buckets = Move(other.m_buckets);
		m_size = other.m_size;

		other.m_size = 0;
	}
}