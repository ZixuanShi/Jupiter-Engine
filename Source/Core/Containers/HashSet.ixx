// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

#include <initializer_list>

export module jpt.HashSet;

import jpt.Concepts;
import jpt.Constants;
import jpt.DynamicArray;
import jpt.LinkedList;
import jpt.Math;
import jpt.Utilities;

import jpt_private.HashTableIterator;

export namespace jpt
{
	/** A hash set is a collection of unique elements. The order of the elements in a hash set is undefined. */
	template<typename _TValue>
	class HashSet
	{
	public:
		using TValue        = _TValue;
		using TBucket       = LinkedList<TValue>;
		using TBuckets      = DynamicArray<TBucket>;
		using Iterator      = jpt_private::HashTableIterator<TValue>;
		using ConstIterator = jpt_private::ConstHashTableIterator<TValue>;

	private:
		TBuckets m_buckets;
		size_t   m_size = 0;

	public:
		constexpr HashSet() noexcept = default;
		constexpr HashSet(const std::initializer_list<TValue>& list);
		constexpr HashSet(const HashSet& other);
		constexpr HashSet(HashSet&& other) noexcept;
		constexpr HashSet& operator=(const HashSet& other);
		constexpr HashSet& operator=(HashSet&& other) noexcept;
		constexpr ~HashSet();

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
		constexpr void Add(const TValue& data);
		constexpr void Erase(const TValue& data);
		constexpr void Clear();

		// Searching
		constexpr bool Contains(const TValue& key) const;

	protected:
		constexpr size_t GetBucketIndex(const TValue& key) const;
		constexpr       TBucket& GetBucket(const TValue& key);
		constexpr const TBucket& GetBucket(const TValue& key) const;

		template<Iterable TContainer>
		constexpr void CopyData(const TContainer& container, size_t size);

		constexpr void MoveSet(HashSet&& other);
	};

	template<typename TValue>
	constexpr HashSet<TValue>::HashSet(const std::initializer_list<TValue>& list)
	{
		CopyData(list, list.size());
	}

	template<typename TValue>
	constexpr HashSet<TValue>::HashSet(const HashSet& other)
	{
		CopyData(other, other.Size());
	}

	template<typename TValue>
	constexpr HashSet<TValue>::HashSet(HashSet&& other) noexcept
	{
		MoveSet(Move(other));
	}

	template<typename TValue>
	constexpr HashSet<TValue>& HashSet<TValue>::operator=(const HashSet& other)
	{
		if (this != &other)
		{
			Clear();
			CopyData(other, other.Size());
		}

		return *this;
	}

	template<typename TValue>
	constexpr HashSet<TValue>& HashSet<TValue>::operator=(HashSet&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			MoveSet(Move(other));
		}

		return *this;
	}

	template<typename TValue>
	constexpr HashSet<TValue>::~HashSet()
	{
		Clear();
	}

	template<typename TValue>
	constexpr void HashSet<TValue>::Add(const TValue& data)
	{
		// Grow if needed
		if (m_size >= m_buckets.Size() * kGrowMultiplier)
		{
			Resize(m_size * kGrowMultiplier);
		}

		TBucket& bucket = GetBucket(data);

		// Check if the key already exists. If it does, return
		for (TValue& element : bucket)
		{
			if (element == data)
			{
				return;
			}
		}

		// If the key does not exist, add it
		++m_size;
		bucket.EmplaceBack(data);
	}

	template<typename TValue>
	constexpr void HashSet<TValue>::Erase(const TValue& key)
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

	template<typename TValue>
	constexpr bool HashSet<TValue>::Contains(const TValue& key) const
	{
		if (IsEmpty())
		{
			return false;
		}

		const TBucket& bucket = GetBucket(key);

		for (const TValue& element : bucket)
		{
			if (element == key)
			{
				return true;
			}
		}

		return false;
	}

	template<typename TValue>
	constexpr HashSet<TValue>::Iterator HashSet<TValue>::begin() noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return Iterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename TValue>
	constexpr HashSet<TValue>::Iterator HashSet<TValue>::end() noexcept
	{
		return Iterator(&m_buckets, m_buckets.Size(), nullptr);
	}

	template<typename TValue>
	constexpr HashSet<TValue>::ConstIterator HashSet<TValue>::begin() const noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename TValue>
	constexpr HashSet<TValue>::ConstIterator HashSet<TValue>::end() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Size(), nullptr);
	}

	template<typename TValue>
	constexpr HashSet<TValue>::ConstIterator HashSet<TValue>::cbegin() const noexcept
	{
		if (IsEmpty())
		{
			return cend();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().cbegin());
	}

	template<typename TValue>
	constexpr HashSet<TValue>::ConstIterator HashSet<TValue>::cend() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Size(), nullptr);
	}

	template<typename TValue>
	constexpr void HashSet<TValue>::Resize(size_t capacity)
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

	template<typename TValue>
	constexpr void HashSet<TValue>::Clear()
	{
		m_buckets.Clear();
		m_size = 0;
	}

	template<typename TValue>
	constexpr size_t HashSet<TValue>::GetBucketIndex(const TValue& key) const
	{
		return Hash<TValue>()(key) % m_buckets.Size();
	}

	template<typename TValue>
	constexpr HashSet<TValue>::TBucket& HashSet<TValue>::GetBucket(const TValue& key)
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename TValue>
	constexpr const HashSet<TValue>::TBucket& HashSet<TValue>::GetBucket(const TValue& key) const
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename TValue>
	template<Iterable TContainer>
	constexpr void HashSet<TValue>::CopyData(const TContainer& container, size_t size)
	{
		Resize(size);

		for (const TValue& data : container)
		{
			TBucket& bucket = GetBucket(data);
			bucket.EmplaceBack(data);
		}

		m_size += size;
	}

	template<typename TValue>
	constexpr void HashSet<TValue>::MoveSet(HashSet&& other)
	{
		m_buckets = Move(other.m_buckets);
		m_size = other.m_size;

		other.m_size = 0;
	}
}