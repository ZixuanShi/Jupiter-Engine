// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.ChainedBucketIterator;

import jpt.TypeDefs;
import jpt.Pair;
import jpt.DynamicArray;
import jpt.LinkedList;

import jpt_private.LinearNodeIterator;

export namespace jpt_private
{
	/** Unordered Map iterator */
	template<typename TKey, typename TValue>
	class ChainedBucketIterator
	{
		using TData    = jpt::Pair<TKey, TValue>;
		using TBucket  = jpt::LinkedList<TData>;
		using TBuckets = jpt::DynamicArray<TBucket>;

	private:
		LinearNodeIterator<TData> m_iterator;
		TBuckets* m_pBuckets = nullptr;
		size_t m_index = 0;

	public:
		constexpr ChainedBucketIterator() = default;
		constexpr ChainedBucketIterator(TBuckets* pBuckets, size_t index, LinearNodeIterator<TData> iterator);

		constexpr ChainedBucketIterator& operator++();
		constexpr ChainedBucketIterator operator++(int32);

		constexpr ChainedBucketIterator& operator+=(size_t offset);
		constexpr ChainedBucketIterator operator+(size_t offset);

		constexpr       TData* operator->()       { return &m_iterator; }
		constexpr const TData* operator->() const { return &m_iterator; }
		constexpr       TData& operator*()        { return *m_iterator; }
		constexpr const TData& operator*()  const { return *m_iterator; }

		constexpr bool operator==(const ChainedBucketIterator& other) const;

	private:
		constexpr bool HasReachedEnd() const;
		constexpr void FindNextValidIterator();
	};

	template<typename TKey, typename TValue>
	constexpr ChainedBucketIterator<TKey, TValue>::ChainedBucketIterator(TBuckets* pBuckets, size_t index, LinearNodeIterator<TData> iterator)
		: m_pBuckets(pBuckets)
		, m_index(index)
		, m_iterator(iterator)
	{
		FindNextValidIterator();
	}

	template<typename TKey, typename TValue>
	constexpr ChainedBucketIterator<TKey, TValue>& ChainedBucketIterator<TKey, TValue>::operator++()
	{
		++m_iterator;
		FindNextValidIterator();
		return *this;
	}

	template<typename TKey, typename TValue>
	constexpr ChainedBucketIterator<TKey, TValue> ChainedBucketIterator<TKey, TValue>::operator++(int32)
	{
		ChainedBucketIterator iterator = *this;
		++m_iterator;
		FindNextValidIterator();
		return iterator;
	}

	template<typename TKey, typename TValue>
	constexpr ChainedBucketIterator<TKey, TValue>& ChainedBucketIterator<TKey, TValue>::operator+=(size_t offset)
	{
		for (size_t i = 0; i < offset; ++i)
		{
			++m_iterator;
			FindNextValidIterator();
		}
		return *this;
	}

	template<typename TKey, typename TValue>
	constexpr ChainedBucketIterator<TKey, TValue> ChainedBucketIterator<TKey, TValue>::operator+(size_t offset)
	{
		ChainedBucketIterator iterator = *this;
		return iterator += offset;
	}

	template<typename TKey, typename TValue>
	constexpr bool ChainedBucketIterator<TKey, TValue>::operator==(const ChainedBucketIterator& other) const
	{
		return m_pBuckets == other.m_pBuckets && 
			   m_index    == other.m_index    && 
			   m_iterator == other.m_iterator;
	}

	template<typename TKey, typename TValue>
	constexpr bool ChainedBucketIterator<TKey, TValue>::HasReachedEnd() const
	{ 
		return m_index == m_pBuckets->Size(); 
	}

	template<typename TKey, typename TValue>
	constexpr void ChainedBucketIterator<TKey, TValue>::FindNextValidIterator()
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
	class ConstChainedBucketIterator
	{
		using TData    = jpt::Pair<TKey, TValue>;
		using TBucket  = jpt::LinkedList<TData>;
		using TBuckets = jpt::DynamicArray<TBucket>;

	private:
		ConstLinearNodeIterator<TData> m_iterator;
		const TBuckets* m_pBuckets = nullptr;
		size_t m_index = 0;

	public:
		constexpr ConstChainedBucketIterator() = default;
		constexpr ConstChainedBucketIterator(const TBuckets* pBuckets, size_t index, ConstLinearNodeIterator<TData> iterator);

		constexpr ConstChainedBucketIterator& operator++();
		constexpr ConstChainedBucketIterator operator++(int32);

		constexpr ConstChainedBucketIterator& operator+=(size_t offset);
		constexpr ConstChainedBucketIterator operator+(size_t offset);

		constexpr const TData* operator->() const { return &m_iterator; }
		constexpr const TData& operator*()  const { return *m_iterator; }

		constexpr bool operator==(const ConstChainedBucketIterator& other) const;

	private:
		constexpr bool HasReachedEnd() const;
		constexpr void FindNextValidIterator();
	};

	template<typename TKey, typename TValue>
	constexpr ConstChainedBucketIterator<TKey, TValue>::ConstChainedBucketIterator(const TBuckets* pBuckets, size_t index, ConstLinearNodeIterator<TData> iterator)
		: m_pBuckets(pBuckets)
		, m_index(index)
		, m_iterator(iterator)
	{
		FindNextValidIterator();
	}

	template<typename TKey, typename TValue>
	constexpr ConstChainedBucketIterator<TKey, TValue>& ConstChainedBucketIterator<TKey, TValue>::operator++()
	{
		++m_iterator;
		FindNextValidIterator();
		return *this;
	}

	template<typename TKey, typename TValue>
	constexpr ConstChainedBucketIterator<TKey, TValue> ConstChainedBucketIterator<TKey, TValue>::operator++(int32)
	{
		ConstChainedBucketIterator iterator = *this;
		++m_iterator;
		FindNextValidIterator();
		return iterator;
	}

	template<typename TKey, typename TValue>
	constexpr ConstChainedBucketIterator<TKey, TValue>& ConstChainedBucketIterator<TKey, TValue>::operator+=(size_t offset)
	{
		for (size_t i = 0; i < offset; ++i)
		{
			++m_iterator;
			FindNextValidIterator();
		}
		return *this;
	}

	template<typename TKey, typename TValue>
	constexpr ConstChainedBucketIterator<TKey, TValue> ConstChainedBucketIterator<TKey, TValue>::operator+(size_t offset)
	{
		ConstChainedBucketIterator iterator = *this;
		return iterator += offset;
	}

	template<typename TKey, typename TValue>
	constexpr bool ConstChainedBucketIterator<TKey, TValue>::operator==(const ConstChainedBucketIterator& other) const
	{
		return m_pBuckets == other.m_pBuckets &&
			   m_index    == other.m_index    &&
			   m_iterator == other.m_iterator;
	}

	template<typename TKey, typename TValue>
	constexpr bool ConstChainedBucketIterator<TKey, TValue>::HasReachedEnd() const
	{
		return m_index == m_pBuckets->Size();
	}

	template<typename TKey, typename TValue>
	constexpr void ConstChainedBucketIterator<TKey, TValue>::FindNextValidIterator()
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
