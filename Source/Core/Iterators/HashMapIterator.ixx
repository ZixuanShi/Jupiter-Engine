// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.HashMapIterator;

import jpt.TypeDefs;
import jpt.Pair;
import jpt.DynamicArray;
import jpt.LinkedList;

import jpt_private.LinearNodeIterator;

export namespace jpt_private
{
	/** Unordered Map iterator */
	template<typename TKey, typename TValue>
	class HashMapIterator
	{
		using TData    = jpt::Pair<TKey, TValue>;
		using TBucket  = jpt::LinkedList<TData>;
		using TBuckets = jpt::DynamicArray<TBucket>;

	private:
		LinearNodeIterator<TData> m_iterator;
		TBuckets* m_pBuckets = nullptr;
		size_t m_index = 0;

	public:
		constexpr HashMapIterator() = default;
		constexpr HashMapIterator(TBuckets* pBuckets, size_t index, LinearNodeIterator<TData> iterator);

		constexpr HashMapIterator& operator++();
		constexpr HashMapIterator operator++(int32);

		constexpr HashMapIterator& operator+=(size_t offset);
		constexpr HashMapIterator operator+(size_t offset);

		constexpr       TData* operator->()       { return &m_iterator; }
		constexpr const TData* operator->() const { return &m_iterator; }
		constexpr       TData& operator*()        { return *m_iterator; }
		constexpr const TData& operator*()  const { return *m_iterator; }

		constexpr bool operator==(const HashMapIterator& other) const;

	private:
		constexpr bool HasReachedEnd() const;
		constexpr void FindNextValidIterator();
	};

	template<typename TKey, typename TValue>
	constexpr HashMapIterator<TKey, TValue>::HashMapIterator(TBuckets* pBuckets, size_t index, LinearNodeIterator<TData> iterator)
		: m_pBuckets(pBuckets)
		, m_index(index)
		, m_iterator(iterator)
	{
		FindNextValidIterator();
	}

	template<typename TKey, typename TValue>
	constexpr HashMapIterator<TKey, TValue>& HashMapIterator<TKey, TValue>::operator++()
	{
		++m_iterator;
		FindNextValidIterator();
		return *this;
	}

	template<typename TKey, typename TValue>
	constexpr HashMapIterator<TKey, TValue> HashMapIterator<TKey, TValue>::operator++(int32)
	{
		HashMapIterator iterator = *this;
		++m_iterator;
		FindNextValidIterator();
		return iterator;
	}

	template<typename TKey, typename TValue>
	constexpr HashMapIterator<TKey, TValue>& HashMapIterator<TKey, TValue>::operator+=(size_t offset)
	{
		for (size_t i = 0; i < offset; ++i)
		{
			++m_iterator;
			FindNextValidIterator();
		}
		return *this;
	}

	template<typename TKey, typename TValue>
	constexpr HashMapIterator<TKey, TValue> HashMapIterator<TKey, TValue>::operator+(size_t offset)
	{
		HashMapIterator iterator = *this;
		return iterator += offset;
	}

	template<typename TKey, typename TValue>
	constexpr bool HashMapIterator<TKey, TValue>::operator==(const HashMapIterator& other) const
	{
		return m_pBuckets == other.m_pBuckets && 
			   m_index    == other.m_index    && 
			   m_iterator == other.m_iterator;
	}

	template<typename TKey, typename TValue>
	constexpr bool HashMapIterator<TKey, TValue>::HasReachedEnd() const
	{ 
		return m_index == m_pBuckets->Size(); 
	}

	template<typename TKey, typename TValue>
	constexpr void HashMapIterator<TKey, TValue>::FindNextValidIterator()
	{
		while (!m_iterator.GetNode() && !HasReachedEnd())
		{
			++m_index;

			if (HasReachedEnd())
			{
				m_iterator = LinearNodeIterator<TData>(nullptr);
				return;
			}

			TBucket& pBucket = m_pBuckets->At(m_index);
			m_iterator = pBucket.begin();
		}
	}

	template<typename TKey, typename TValue>
	class ConstHashMapIterator
	{
		using TData    = jpt::Pair<TKey, TValue>;
		using TBucket  = jpt::LinkedList<TData>;
		using TBuckets = jpt::DynamicArray<TBucket>;

	private:
		ConstLinearNodeIterator<TData> m_iterator;
		const TBuckets* m_pBuckets = nullptr;
		size_t m_index = 0;

	public:
		constexpr ConstHashMapIterator() = default;
		constexpr ConstHashMapIterator(const TBuckets* pBuckets, size_t index, ConstLinearNodeIterator<TData> iterator);

		constexpr ConstHashMapIterator& operator++();
		constexpr ConstHashMapIterator operator++(int32);

		constexpr ConstHashMapIterator& operator+=(size_t offset);
		constexpr ConstHashMapIterator operator+(size_t offset);

		constexpr const TData* operator->() const { return &m_iterator; }
		constexpr const TData& operator*()  const { return *m_iterator; }

		constexpr bool operator==(const ConstHashMapIterator& other) const;

	private:
		constexpr bool HasReachedEnd() const;
		constexpr void FindNextValidIterator();
	};

	template<typename TKey, typename TValue>
	constexpr ConstHashMapIterator<TKey, TValue>::ConstHashMapIterator(const TBuckets* pBuckets, size_t index, ConstLinearNodeIterator<TData> iterator)
		: m_pBuckets(pBuckets)
		, m_index(index)
		, m_iterator(iterator)
	{
		FindNextValidIterator();
	}

	template<typename TKey, typename TValue>
	constexpr ConstHashMapIterator<TKey, TValue>& ConstHashMapIterator<TKey, TValue>::operator++()
	{
		++m_iterator;
		FindNextValidIterator();
		return *this;
	}

	template<typename TKey, typename TValue>
	constexpr ConstHashMapIterator<TKey, TValue> ConstHashMapIterator<TKey, TValue>::operator++(int32)
	{
		ConstHashMapIterator iterator = *this;
		++m_iterator;
		FindNextValidIterator();
		return iterator;
	}

	template<typename TKey, typename TValue>
	constexpr ConstHashMapIterator<TKey, TValue>& ConstHashMapIterator<TKey, TValue>::operator+=(size_t offset)
	{
		for (size_t i = 0; i < offset; ++i)
		{
			++m_iterator;
			FindNextValidIterator();
		}
		return *this;
	}

	template<typename TKey, typename TValue>
	constexpr ConstHashMapIterator<TKey, TValue> ConstHashMapIterator<TKey, TValue>::operator+(size_t offset)
	{
		ConstHashMapIterator iterator = *this;
		return iterator += offset;
	}

	template<typename TKey, typename TValue>
	constexpr bool ConstHashMapIterator<TKey, TValue>::operator==(const ConstHashMapIterator& other) const
	{
		return m_pBuckets == other.m_pBuckets &&
			   m_index    == other.m_index    &&
			   m_iterator == other.m_iterator;
	}

	template<typename TKey, typename TValue>
	constexpr bool ConstHashMapIterator<TKey, TValue>::HasReachedEnd() const
	{
		return m_index == m_pBuckets->Size();
	}

	template<typename TKey, typename TValue>
	constexpr void ConstHashMapIterator<TKey, TValue>::FindNextValidIterator()
	{
		while (!m_iterator.GetNode() && !HasReachedEnd())
		{
			++m_index;

			if (HasReachedEnd())
			{
				m_iterator = ConstLinearNodeIterator<TData>(nullptr);
				return;
			}

			const TBucket& pBucket = m_pBuckets->At(m_index);
			m_iterator = pBucket.cbegin();
		}
	}
}
