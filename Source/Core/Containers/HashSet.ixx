// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

#include <initializer_list>

export module jpt.HashSet;

import jpt.Comparators;
import jpt.Concepts;
import jpt.Constants;
import jpt.DynamicArray;
import jpt.Hash;
import jpt.LinkedList;
import jpt.Math;
import jpt.Utilities;

import jpt_private.HashTableIterator;

export namespace jpt
{
	/** A hash set is a collection of unique elements. The order of the elements in a hash set is undefined. */
	template<typename _TData, bool kShouldGrow = true, typename _Comparator = Comparator_Equal<_TData>>
	class HashSet
	{
	public:
		using TData         = _TData;
		using TComparator   = _Comparator;
		using TBucket       = LinkedList<TData>;
		using TBuckets      = DynamicArray<TBucket>;
		using Iterator      = jpt_private::HashTableIterator<TData>;
		using ConstIterator = jpt_private::ConstHashTableIterator<TData>;

	private:
		static constexpr TComparator kComparator = TComparator();
		static constexpr float kLoadFactor = 0.75f;
		static constexpr size_t kGrowMultiplier = 2;

	private:
		TBuckets m_buckets;
		size_t   m_count = 0;

	public:
		constexpr HashSet() noexcept = default;
		constexpr HashSet(const std::initializer_list<TData>& list);
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
		constexpr size_t Count() const noexcept;
		constexpr bool IsEmpty() const noexcept;
		constexpr void Reserve(size_t capacity);

		// Modifiers
		constexpr void Add(const TData& data);
		constexpr Iterator Erase(const TData& data);
		constexpr Iterator Erase(const Iterator& iterator);
		constexpr void Clear();

		// Searching
		constexpr Iterator      Find(const TData& key);
		constexpr ConstIterator Find(const TData& key) const;
		constexpr bool Has(const TData& key) const;

	protected:
		constexpr size_t GetBucketIndex(const TData& key) const;
		constexpr       TBucket& GetBucket(const TData& key);
		constexpr const TBucket& GetBucket(const TData& key) const;

		template<Iterable TContainer>
		constexpr void CopyData(const TContainer& container, size_t size);

		constexpr void MoveSet(HashSet&& other);
	};

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>::HashSet(const std::initializer_list<TData>& list)
	{
		CopyData(list, list.size());
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>::HashSet(const HashSet& other)
	{
		CopyData(other, other.Count());
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>::HashSet(HashSet&& other) noexcept
	{
		MoveSet(Move(other));
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>& HashSet<TValue, kShouldGrow, TComparator>::operator=(const HashSet& other)
	{
		if (this != &other)
		{
			Clear();
			CopyData(other, other.Count());
		}

		return *this;
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>& HashSet<TValue, kShouldGrow, TComparator>::operator=(HashSet&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			MoveSet(Move(other));
		}

		return *this;
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>::~HashSet()
	{
		Clear();
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr void HashSet<TValue, kShouldGrow, TComparator>::Add(const TData& data)
	{
		// Grow if needed
		if constexpr (kShouldGrow)
		{
			if (m_count >= m_buckets.Count() * kLoadFactor)
			{
				Reserve(m_count * kGrowMultiplier);
			}
		}

		TBucket& bucket = GetBucket(data);

		// Check if the key already exists. If it does, return
		for (TData& element : bucket)
		{
			if (kComparator(element, data))
			{
				return;
			}
		}

		// If the key does not exist, add it
		++m_count;
		bucket.EmplaceBack(data);
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>::Iterator HashSet<TValue, kShouldGrow, TComparator>::Erase(const TData& key)
	{
		if (Iterator itr = Find(key); itr != end())
		{
			Iterator nextItr = itr + 1;

			TBucket& bucket = m_buckets[itr.GetIndex()];
			bucket.Erase(itr.GetIterator());
			--m_count;

			return nextItr;
		}

		return end();
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr  HashSet<TValue, kShouldGrow, TComparator>::Iterator HashSet<TValue, kShouldGrow, TComparator>::Erase(const Iterator& iterator)
	{
		return Erase(*iterator);
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>::Iterator HashSet<TValue, kShouldGrow, TComparator>::Find(const TData& key)
	{
		if (IsEmpty())
		{
			return end();
		}

		const size_t index = GetBucketIndex(key);
		TBucket& bucket = m_buckets[index];

		for (typename TBucket::Iterator itr = bucket.begin(); itr != bucket.end(); ++itr)
		{
			if (kComparator(*itr, key))
			{
				return Iterator(&m_buckets, index, itr);
			}
		}

		return end();
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>::ConstIterator HashSet<TValue, kShouldGrow, TComparator>::Find(const TData& key) const
	{
		if (IsEmpty())
		{
			return cend();
		}

		const size_t index = GetBucketIndex(key);
		const TBucket& bucket = m_buckets[index];

		for (typename TBucket::ConstIterator itr = bucket.cbegin(); itr != bucket.cend(); ++itr)
		{
			if (kComparator(*itr, key))
			{
				return ConstIterator(&m_buckets, index, itr);
			}
		}

		return cend();
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr bool HashSet<TValue, kShouldGrow, TComparator>::Has(const TData& key) const
	{
		return Find(key) != end();
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>::Iterator HashSet<TValue, kShouldGrow, TComparator>::begin() noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return Iterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>::Iterator HashSet<TValue, kShouldGrow, TComparator>::end() noexcept
	{
		return Iterator(&m_buckets, m_buckets.Count(), nullptr);
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>::ConstIterator HashSet<TValue, kShouldGrow, TComparator>::begin() const noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>::ConstIterator HashSet<TValue, kShouldGrow, TComparator>::end() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Count(), nullptr);
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>::ConstIterator HashSet<TValue, kShouldGrow, TComparator>::cbegin() const noexcept
	{
		if (IsEmpty())
		{
			return cend();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().cbegin());
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>::ConstIterator HashSet<TValue, kShouldGrow, TComparator>::cend() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Count(), nullptr);
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr size_t HashSet<TValue, kShouldGrow, TComparator>::Count() const noexcept
	{
		return m_count;
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr bool HashSet<TValue, kShouldGrow, TComparator>::IsEmpty() const noexcept
	{
		return m_count == 0;
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr void HashSet<TValue, kShouldGrow, TComparator>::Reserve(size_t capacity)
	{
		static constexpr size_t kMinCapacity = 8;

		TBuckets oldDataCopy = m_buckets;
		m_buckets.Clear();
		m_buckets.Resize(Max(kMinCapacity, capacity));

		for (const TBucket& bucket : oldDataCopy)
		{
			for (const TData& element : bucket)
			{
				const size_t index = GetBucketIndex(element);
				m_buckets[index].EmplaceBack(element);
			}
		}
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr void HashSet<TValue, kShouldGrow, TComparator>::Clear()
	{
		m_buckets.Clear();
		m_count = 0;
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr size_t HashSet<TValue, kShouldGrow, TComparator>::GetBucketIndex(const TData& key) const
	{
		return Hash(key) % m_buckets.Count();
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr HashSet<TValue, kShouldGrow, TComparator>::TBucket& HashSet<TValue, kShouldGrow, TComparator>::GetBucket(const TData& key)
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr const HashSet<TValue, kShouldGrow, TComparator>::TBucket& HashSet<TValue, kShouldGrow, TComparator>::GetBucket(const TData& key) const
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	template<Iterable TContainer>
	constexpr void HashSet<TValue, kShouldGrow, TComparator>::CopyData(const TContainer& container, size_t size)
	{
		Reserve(m_count + size);

		for (const TData& data : container)
		{
			Add(data);
		}
	}

	template<typename TValue, bool kShouldGrow, typename TComparator>
	constexpr void HashSet<TValue, kShouldGrow, TComparator>::MoveSet(HashSet&& other)
	{
		m_buckets = Move(other.m_buckets);
		m_count = other.m_count;

		other.m_count = 0;
	}
}