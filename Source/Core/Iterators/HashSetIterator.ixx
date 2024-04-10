// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.HashSetIterator;

import jpt.TypeDefs;
import jpt.Pair;
import jpt.DynamicArray;
import jpt.LinkedList;

import jpt_private.LinearNodeIterator;

export namespace jpt_private
{
	template<typename TData>
	class HashSetIterator
	{
		using TBucket  = jpt::LinkedList<TData>;
		using TBuckets = jpt::DynamicArray<TBucket>;

	private:
		LinearNodeIterator<TData> m_iterator;
		TBuckets* m_pBuckets = nullptr;
		size_t m_index = 0;

	public:
		constexpr HashSetIterator() = default;
		constexpr HashSetIterator(TBuckets* pBuckets, size_t index, LinearNodeIterator<TData> iterator);

		constexpr HashSetIterator& operator++();
		constexpr HashSetIterator operator++(int32);

		constexpr HashSetIterator& operator+=(size_t offset);
		constexpr HashSetIterator operator+(size_t offset);

		constexpr       TData* operator->() { return &m_iterator; }
		constexpr const TData* operator->() const { return &m_iterator; }
		constexpr       TData& operator*() { return *m_iterator; }
		constexpr const TData& operator*()  const { return *m_iterator; }

		constexpr bool operator==(const HashSetIterator& other) const;

	private:
		constexpr bool HasReachedEnd() const;
		constexpr void FindNextValidIterator();
	};

	template<typename TData>
	constexpr HashSetIterator<TData>::HashSetIterator(TBuckets* pBuckets, size_t index, LinearNodeIterator<TData> iterator)
		: m_pBuckets(pBuckets)
		, m_index(index)
		, m_iterator(iterator)
	{
		FindNextValidIterator();
	}

	template<typename TData>
	constexpr HashSetIterator<TData>& HashSetIterator<TData>::operator++()
	{
		++m_iterator;
		FindNextValidIterator();
		return *this;
	}

	template<typename TData>
	constexpr HashSetIterator<TData> HashSetIterator<TData>::operator++(int32)
	{
		HashSetIterator iterator = *this;
		++m_iterator;
		FindNextValidIterator();
		return iterator;
	}

	template<typename TData>
	constexpr HashSetIterator<TData>& HashSetIterator<TData>::operator+=(size_t offset)
	{
		for (size_t i = 0; i < offset; ++i)
		{
			++m_iterator;
			FindNextValidIterator();
		}
		return *this;
	}

	template<typename TData>
	constexpr HashSetIterator<TData> HashSetIterator<TData>::operator+(size_t offset)
	{
		HashSetIterator iterator = *this;
		return iterator += offset;
	}

	template<typename TData>
	constexpr bool HashSetIterator<TData>::operator==(const HashSetIterator& other) const
	{
		return m_pBuckets == other.m_pBuckets &&
			   m_index == other.m_index &&
			   m_iterator == other.m_iterator;
	}

	template<typename TData>
	constexpr bool HashSetIterator<TData>::HasReachedEnd() const
	{
		return m_index == m_pBuckets->Size();
	}

	template<typename TData>
	constexpr void HashSetIterator<TData>::FindNextValidIterator()
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

	template<typename TData>
	class ConstHashSetIterator
	{
		using TBucket = jpt::LinkedList<TData>;
		using TBuckets = jpt::DynamicArray<TBucket>;

	private:
		ConstLinearNodeIterator<TData> m_iterator;
		const TBuckets* m_pBuckets = nullptr;
		size_t m_index = 0;

	public:
		constexpr ConstHashSetIterator() = default;
		constexpr ConstHashSetIterator(const TBuckets* pBuckets, size_t index, ConstLinearNodeIterator<TData> iterator);

		constexpr ConstHashSetIterator& operator++();
		constexpr ConstHashSetIterator operator++(int32);

		constexpr ConstHashSetIterator& operator+=(size_t offset);
		constexpr ConstHashSetIterator operator+(size_t offset);

		constexpr const TData* operator->() const { return &m_iterator; }
		constexpr const TData& operator*()  const { return *m_iterator; }

		constexpr bool operator==(const ConstHashSetIterator& other) const;

	private:
		constexpr bool HasReachedEnd() const;
		constexpr void FindNextValidIterator();
	};

	template<typename TData>
	constexpr ConstHashSetIterator<TData>::ConstHashSetIterator(const TBuckets* pBuckets, size_t index, ConstLinearNodeIterator<TData> iterator)
		: m_pBuckets(pBuckets)
		, m_index(index)
		, m_iterator(iterator)
	{
		FindNextValidIterator();
	}

	template<typename TData>
	constexpr ConstHashSetIterator<TData>& ConstHashSetIterator<TData>::operator++()
	{
		++m_iterator;
		FindNextValidIterator();
		return *this;
	}

	template<typename TData>
	constexpr ConstHashSetIterator<TData> ConstHashSetIterator<TData>::operator++(int32)
	{
		ConstHashSetIterator iterator = *this;
		++m_iterator;
		FindNextValidIterator();
		return iterator;
	}

	template<typename TData>
	constexpr ConstHashSetIterator<TData>& ConstHashSetIterator<TData>::operator+=(size_t offset)
	{
		for (size_t i = 0; i < offset; ++i)
		{
			++m_iterator;
			FindNextValidIterator();
		}
		return *this;
	}

	template<typename TData>
	constexpr ConstHashSetIterator<TData> ConstHashSetIterator<TData>::operator+(size_t offset)
	{
		ConstHashSetIterator iterator = *this;
		return iterator += offset;
	}

	template<typename TData>
	constexpr bool ConstHashSetIterator<TData>::operator==(const ConstHashSetIterator& other) const
	{
		return m_pBuckets == other.m_pBuckets &&
			   m_index == other.m_index &&
			   m_iterator == other.m_iterator;
	}

	template<typename TData>
	constexpr bool ConstHashSetIterator<TData>::HasReachedEnd() const
	{
		return m_index == m_pBuckets->Size();
	}

	template<typename TData>
	constexpr void ConstHashSetIterator<TData>::FindNextValidIterator()
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