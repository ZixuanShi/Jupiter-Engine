// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"

#include <initializer_list>

export module jpt.HashMap;

import jpt.Comparators;
import jpt.Concepts;
import jpt.Constants;
import jpt.DynamicArray;
import jpt.Hash;
import jpt.LinkedList;
import jpt.Math;
import jpt.Pair;
import jpt.Utilities;

import jpt_private.HashTableIterator;

export namespace jpt
{
	/** Unordered map implementation with Chainning hash collision-handling  */
	template <typename _TKey, typename _TValue, typename _Comparator = Comparator_Equal<_TKey>>
	class HashMap
	{
	public:
		using TKey          = _TKey;
		using TValue        = _TValue;
		using TComparator   = _Comparator;
		using TData         = Pair<TKey, TValue>;
		using TBucket       = LinkedList<TData>;
		using TBuckets 	    = DynamicArray<TBucket>;
		using Iterator	    = jpt_private::HashTableIterator<TData>;
		using ConstIterator = jpt_private::ConstHashTableIterator<TData>;

	private:
		static constexpr TComparator kComparator = TComparator();

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
		constexpr size_t Count() const { return m_count; }
		constexpr bool IsEmpty() const { return m_count == 0; }
		constexpr void Reserve(size_t capacity);

		// Modifiers
		constexpr TValue& Add(const TKey& key, const TValue& value);
		constexpr TValue& Add(const TData& element);
		constexpr Iterator Erase(const TKey& key);
		constexpr Iterator Erase(const Iterator& iterator);
		constexpr void Clear();

		// Searching
		constexpr Iterator      Find(const TKey& key);
		constexpr ConstIterator Find(const TKey& key) const;
		constexpr bool Has(const TKey& key) const;

	private:
		constexpr size_t GetBucketIndex(const TKey& key) const;
		constexpr       TBucket& GetBucket(const TKey& key);
		constexpr const TBucket& GetBucket(const TKey& key) const;

		template<Iterable TContainer>
		constexpr void CopyData(const TContainer& container, size_t size);
		constexpr void MoveMap(HashMap&& other);
	};

	//----------------------------------------------------------------------------------------------
	// Non-member functions
	//----------------------------------------------------------------------------------------------
	template<typename TKey, typename TValue, typename TComparator>
	constexpr bool operator==(const HashMap<TKey, TValue, TComparator>& lhs, const HashMap<TKey, TValue, TComparator>& rhs)
	{
		if (lhs.Count() != rhs.Count())
		{
			return false;
		}

		for (const auto& [key, value] : lhs)
		{
			if (rhs.Has(key) == false || rhs[key] != value)
			{
				return false;
			}
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------
	// Member function definitions
	//----------------------------------------------------------------------------------------------
	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::HashMap(const std::initializer_list<TData>& list)
	{
		CopyData(list, list.size());
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::HashMap(const HashMap& other)
	{
		CopyData(other, other.Count());
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::HashMap(HashMap&& other) noexcept
	{
		MoveMap(Move(other));
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>& HashMap<TKey, TValue, TComparator>::operator=(const HashMap& other)
	{
		if (this != &other)
		{
			Clear();
			CopyData(other, other.Count());
		}

		return *this;
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>& HashMap<TKey, TValue, TComparator>::operator=(HashMap&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			MoveMap(Move(other));
		}

		return *this;
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::~HashMap()
	{
		Clear();
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::TValue& HashMap<TKey, TValue, TComparator>::operator[](const TKey& key)
	{
		Iterator itr = Find(key);
		if (itr == end())
		{
			return Add(key, TValue());
		}

		return itr->second;
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr const HashMap<TKey, TValue, TComparator>::TValue& HashMap<TKey, TValue, TComparator>::operator[](const TKey& key) const
	{
		ConstIterator itr = Find(key);
		JPT_ASSERT(itr != cend());
		return itr->second;
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::Iterator HashMap<TKey, TValue, TComparator>::begin() noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return Iterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::Iterator HashMap<TKey, TValue, TComparator>::end() noexcept
	{
		return Iterator(&m_buckets, m_buckets.Count(), nullptr);
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::ConstIterator HashMap<TKey, TValue, TComparator>::begin() const noexcept
	{
		if (IsEmpty())
		{
			return end();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().begin());
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::ConstIterator HashMap<TKey, TValue, TComparator>::end() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Count(), nullptr);
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::ConstIterator HashMap<TKey, TValue, TComparator>::cbegin() const noexcept
	{
		if (IsEmpty())
		{
			return cend();
		}
		return ConstIterator(&m_buckets, 0, m_buckets.Front().cbegin());
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::ConstIterator HashMap<TKey, TValue, TComparator>::cend() const noexcept
	{
		return ConstIterator(&m_buckets, m_buckets.Count(), nullptr);
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr void HashMap<TKey, TValue, TComparator>::Clear()
	{
		m_buckets.Clear(); 
		m_count = 0;
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr TValue& HashMap<TKey, TValue, TComparator>::Add(const TKey& key, const TValue& value)
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
			if (kComparator(element.first, key))
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

	template<typename TKey, typename TValue, typename TComparator>
	constexpr TValue& HashMap<TKey, TValue, TComparator>::Add(const TData& element)
	{
		return Add(element.first, element.second);
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::Iterator HashMap<TKey, TValue, TComparator>::Erase(const TKey& key)
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

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::Iterator HashMap<TKey, TValue, TComparator>::Erase(const Iterator& iterator)
	{
		return Erase(iterator->first);
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::Iterator HashMap<TKey, TValue, TComparator>::Find(const TKey& key)
	{
		if (IsEmpty())
		{
			return end();
		}

		const size_t index = GetBucketIndex(key);
		TBucket& bucket = m_buckets[index];

		for (typename TBucket::Iterator itr = bucket.begin(); itr != bucket.end(); ++itr)
		{
			if (kComparator(itr->first, key))
			{
				return Iterator(&m_buckets, index, itr);
			}
		}

		return end();
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::ConstIterator HashMap<TKey, TValue, TComparator>::Find(const TKey& key) const
	{
		if (IsEmpty())
		{
			return cend();
		}

		const size_t index = GetBucketIndex(key);
		const TBucket& bucket = m_buckets[index];

		for (typename TBucket::ConstIterator itr = bucket.cbegin(); itr != bucket.cend(); ++itr)
		{
			if (kComparator(itr->first, key))
			{
				return ConstIterator(&m_buckets, index, itr);
			}
		}

		return cend();
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr bool HashMap<TKey, TValue, TComparator>::Has(const TKey& key) const
	{
		return Find(key) != cend();
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr void HashMap<TKey, TValue, TComparator>::Reserve(size_t capacity)
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

	template<typename TKey, typename TValue, typename TComparator>
	constexpr size_t HashMap<TKey, TValue, TComparator>::GetBucketIndex(const TKey& key) const
	{
		return Hash(key) % m_buckets.Count();
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr HashMap<TKey, TValue, TComparator>::TBucket& HashMap<TKey, TValue, TComparator>::GetBucket(const TKey& key)
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr const HashMap<TKey, TValue, TComparator>::TBucket& HashMap<TKey, TValue, TComparator>::GetBucket(const TKey& key) const
	{
		return m_buckets[GetBucketIndex(key)];
	}

	template<typename TKey, typename TValue, typename TComparator>
	template<Iterable TContainer>
	constexpr void HashMap<TKey, TValue, TComparator>::CopyData(const TContainer& container, size_t size)
	{
		Reserve(m_count + size);

		for (const TData& element : container)
		{
			TBucket& bucket = GetBucket(element.first);
			bucket.EmplaceBack(element);
		}

		m_count += size;
	}

	template<typename TKey, typename TValue, typename TComparator>
	constexpr void HashMap<TKey, TValue, TComparator>::MoveMap(HashMap&& other)
	{
		m_buckets = Move(other.m_buckets);
		m_count   = other.m_count;

		other.m_count = 0;
	}
}
