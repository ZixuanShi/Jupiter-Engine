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
import jpt.Utilities;
import jpt.Math;

import jpt_private.HashTable;
import jpt_private.HashTableIterator;

export namespace jpt
{
	/** A hash set is a collection of unique elements. The order of the elements in a hash set is undefined. */
	template<typename _TData>
	class HashSet final : public HashTable<_TData>
	{
	public:
		using TData   = _TData;
		using Super   = HashTable<TData>;
		using TBucket = typename Super::TBucket;

	public:
		constexpr HashSet() = default;
		constexpr HashSet(const std::initializer_list<TData>& list);
		constexpr HashSet(const HashSet& other);
		constexpr HashSet(HashSet&& other) noexcept;
		constexpr HashSet& operator=(const HashSet& other);
		constexpr HashSet& operator=(HashSet&& other) noexcept;

		// Modifiers
		constexpr virtual void Add(const TData& data) override;
		constexpr void Erase(const TData& data);

		// Searching
		constexpr bool Contains(const TData& key) const;
	};

	template<typename _TData>
	constexpr HashSet<_TData>::HashSet(const std::initializer_list<TData>& list)
	{
		Super::CopyData(list);
	}

	template<typename _TData>
	constexpr HashSet<_TData>::HashSet(const HashSet& other)
	{
		Super::CopyData(other);
	}

	template<typename _TData>
	constexpr HashSet<_TData>::HashSet(HashSet&& other) noexcept
	{
		Super::MoveTable(Move(other));
	}

	template<typename _TData>
	constexpr HashSet<_TData>& HashSet<_TData>::operator=(const HashSet& other)
	{
		if (this != &other)
		{
			Super::Clear();
			Super::CopyData(other);
		}

		return *this;
	}

	template<typename _TData>
	constexpr HashSet<_TData>& HashSet<_TData>::operator=(HashSet&& other) noexcept
	{
		if (this != &other)
		{
			Super::Clear();
			Super::MoveTable(Move(other));
		}

		return *this;
	}

	template<typename _TData>
	constexpr void HashSet<_TData>::Add(const TData& data)
	{
		// Grow if needed
		if (Super::m_buckets.IsEmpty())
		{
			Super::Resize(Super::m_size * kGrowMultiplier);
		}

		TBucket& bucket = Super::GetBucket(data);

		// Check if the key already exists. If it does, return
		for (TData& element : bucket)
		{
			if (element == data)
			{
				return;
			}
		}

		// If the key does not exist, add it
		++Super::m_size;
		bucket.EmplaceBack(data);
	}

	template<typename _TData>
	constexpr void HashSet<_TData>::Erase(const TData& key)
	{
		if (Super::IsEmpty())
		{
			return;
		}

		TBucket& bucket = Super::GetBucket(key);

		for (auto it = bucket.begin(); it != bucket.end(); ++it)
		{
			if (*it == key)
			{
				bucket.Erase(it);
				--Super::m_size;
				return;
			}
		}
	}

	template<typename _TData>
	constexpr bool HashSet<_TData>::Contains(const TData& key) const
	{
		if (Super::IsEmpty())
		{
			return false;
		}

		const TBucket& bucket = Super::GetBucket(key);

		for (const TData& element : bucket)
		{
			if (element == key)
			{
				return true;
			}
		}

		return false;
	}
}