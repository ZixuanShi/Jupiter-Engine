// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	/** Hash Table implementatio.Collision handled by Chainning */
	template<class _KeyType, class _ValueType>
	class unordered_map
	{
	public:
		using KeyType     = _KeyType;
		using ValueType   = _ValueType;
		using ItemType    = jpt::pair<KeyType, ValueType>;
		using BucketType  = jpt::list<ItemType>;
		using BucketsType = jpt::vector<BucketType>;	/**< Vector of Linked List as data buffer.Should be replaced by Vector of Red - Black Tree when it's implemented */

	private:
		template<class UnorderedMapType>
		class unordered_map_iterator
		{
		private:
			BucketsType* m_pBuckets = nullptr;
			size_t m_bucketIndex = 0;
			BucketType::iterator m_itemsIterator;

		public:
			unordered_map_iterator(BucketsType* pBuckets, size_t bucketIndex, const typename BucketType::iterator& iterator)
				: m_pBuckets(pBuckets)
				, m_bucketIndex(bucketIndex)
				, m_itemsIterator(iterator)
			{
				if (!pBuckets)
				{
					return;
				}

				if (!m_pBuckets->at(m_bucketIndex).empty())
				{
					return;
				}

				// If the passed in bucketIndex at pBuckets is empty, find the next appropriate one in the map
				// Increase index until we found an appropriate bucket, or the end of this map
				while (m_bucketIndex < m_pBuckets->size())
				{
					++m_bucketIndex;

					if (!m_pBuckets->at(m_bucketIndex).empty())
					{
						// Found a appropriate index, set itemsIterator to its begin() and stop the loop
						m_itemsIterator = m_pBuckets->at(m_bucketIndex).begin();
						break;
					}
				}
			}

			UnorderedMapType::ItemType* operator->() { return &m_itemsIterator; }
			UnorderedMapType::ItemType& operator*() { return *m_itemsIterator; }
			bool operator==(const unordered_map_iterator& other) const { return m_bucketIndex == other.m_bucketIndex && m_itemsIterator == other.m_itemsIterator; }
			bool operator!=(const unordered_map_iterator& other) const { return m_bucketIndex != other.m_bucketIndex || m_itemsIterator != other.m_itemsIterator; }

			unordered_map_iterator& operator++()
			{
				++m_itemsIterator;

				if (m_itemsIterator != m_pBuckets->at(m_bucketIndex).end())
				{
					return *this;
				}

				while (m_bucketIndex < m_pBuckets->size())
				{
					++m_bucketIndex;

					if (!m_pBuckets->at(m_bucketIndex).empty())
					{
						m_itemsIterator = m_pBuckets->at(m_bucketIndex).begin();
						break;
					}
				}

				return *this;
			}
		};

	public:
		using iterator = unordered_map_iterator<unordered_map<KeyType, ValueType>>;

	private:
		BucketsType m_buckets;

		// Note: The m_buckets.size() will possibly be different than this unordered_map's actual size due to chainning. 
		// When retrieving the size of this unordered_map, we must use m_size or this->size();
		size_t m_size = 0;

	public:
		unordered_map() = default;
		unordered_map(const unordered_map& other);
		unordered_map(unordered_map&& other);
		unordered_map& operator=(const unordered_map& other);
		unordered_map& operator=(unordered_map&& other);
		~unordered_map();

		// Iterators
		iterator begin() noexcept
		{
			if (empty())
			{
				return iterator(nullptr, 0, typename BucketType::iterator(nullptr));
			}

			return iterator(&m_buckets, 0, m_buckets[0].begin()); 
		}

		const iterator begin() const noexcept
		{ 
			if (empty())
			{
				return iterator(nullptr, 0, typename BucketType::iterator(nullptr));
			}

			return iterator(&m_buckets, 0, m_buckets[0].begin());
		}

		iterator end() noexcept 
		{
			if (empty())
			{
				return iterator(nullptr, 0, typename BucketType::iterator(nullptr));
			}

			return iterator(&m_buckets, m_buckets.size() - 1, m_buckets.back().end());
		}

		const iterator end() const noexcept 
		{ 
			if (empty())
			{
				return iterator(nullptr, 0, typename BucketType::iterator(nullptr));
			}

			return iterator(&m_buckets, m_buckets.size() - 1, m_buckets.back().end()); 
		}

		// Capacity
		bool empty() const { return m_size == 0; }

		// Modifiers
		void clear();
		void insert(const ItemType& item);
		void erase(const KeyType& key);

		// Lookup
		ValueType& operator[](const KeyType& key);
		bool contains(const KeyType& key) const;
		iterator find(const KeyType& key);

	private:
		/** @return The index of which bucket this key should locate */
		size_t GetBucketIndex(const KeyType& key) const;

		/* Copy construct every item in another map */
		void CopyMap(const unordered_map& other);

		/* Move every item in another map to this one */
		void TakeMap(unordered_map&& other);
	};

	template<class _KeyType, class _ValueType>
	inline unordered_map<_KeyType, _ValueType>::unordered_map(const unordered_map& other)
	{
		CopyMap(other);
	}

	template<class _KeyType, class _ValueType>
	inline unordered_map<_KeyType, _ValueType>::unordered_map(unordered_map&& other)
	{
		TakeMap(jpt::move(other));
	}

	template<class _KeyType, class _ValueType>
	inline unordered_map<_KeyType, _ValueType>& unordered_map<_KeyType, _ValueType>::operator=(const unordered_map& other)
	{
		if (this == &other)
		{
			return *this;
		}

		clear();
		CopyMap(jpt::move(other));
		return *this;
	}

	template<class _KeyType, class _ValueType>
	inline unordered_map<_KeyType, _ValueType>& unordered_map<_KeyType, _ValueType>::operator=(unordered_map&& other)
	{
		if (this == &other)
		{
			return *this;
		}

		clear();
		TakeMap(jpt::move(other));
		return *this;
	}

	template<class _KeyType, class _ValueType>
	inline unordered_map<_KeyType, _ValueType>::~unordered_map()
	{
		clear();
	}

	template<class _KeyType, class _ValueType>
	inline void unordered_map<_KeyType, _ValueType>::clear()
	{
		m_buckets.clear();
		m_size = 0;
	}

	template<class _KeyType, class _ValueType>
	inline void unordered_map<_KeyType, _ValueType>::insert(const ItemType& item)
	{
		static constexpr size_t kInitialBucketSize = 32;
		if (empty())
		{
			m_buckets.resize(kInitialBucketSize);
		}

		const size_t bucketIndex = GetBucketIndex(item.first);
		m_buckets[bucketIndex].push_back(item);
		++m_size;
	}

	template<class _KeyType, class _ValueType>
	inline void unordered_map<_KeyType, _ValueType>::erase(const KeyType& key)
	{
		JPT_ASSERT(contains(key), "Calling erase with a key that doesn't exist");

		const size_t bucketIndex = GetBucketIndex(key);
		jpt::list<ItemType>& items = m_buckets[bucketIndex];

		size_t i = 0;
		for (ItemType& item : items)
		{
			if (item.first == key)
			{
				items.erase(i);
				break;
			}
			++i;
		}

		--m_size;
	}

	template<class _KeyType, class _ValueType>
	inline unordered_map<_KeyType, _ValueType>::ValueType& unordered_map<_KeyType, _ValueType>::operator[](const KeyType& key)
	{
		// If the current unordered_map is empty, directly go to insert it below
		if (!empty())
		{
			const size_t bucketIndex = GetBucketIndex(key);
			jpt::list<ItemType>& items = m_buckets[bucketIndex];

			for (ItemType& item : items)
			{
				if (item.first == key)
				{
					return item.second;
				}
			}
		}

		this->insert({ key, ValueType() });
		return this->operator[](key);
	}

	template<class _KeyType, class _ValueType>
	inline bool unordered_map<_KeyType, _ValueType>::contains(const KeyType& key) const
	{
		const size_t bucketIndex = GetBucketIndex(key);
		const jpt::list<ItemType>& items = m_buckets[bucketIndex];

		for (const ItemType& item : items)
		{
			if (item.first == key)
			{
				return true;
			}
		}

		return false;
	}

	template<class _KeyType, class _ValueType>
	inline unordered_map<_KeyType, _ValueType>::iterator unordered_map<_KeyType, _ValueType>::find(const KeyType& key)
	{
		const size_t bucketIndex = GetBucketIndex(key);
		jpt::list<ItemType>& items = m_buckets[bucketIndex];

		for (auto itr = items.begin(); itr != items.end(); ++itr)
		{
			if (itr->first == key)
			{
				return iterator(&m_buckets, bucketIndex, itr);
			}
		}

		return end();
	}

	template<class _KeyType, class _ValueType>
	inline size_t unordered_map<_KeyType, _ValueType>::GetBucketIndex(const KeyType& key) const
	{
		const size_t hashValue = jpt::hash<KeyType>()(key);
		return hashValue % m_buckets.size();
	}

	template<class _KeyType, class _ValueType>
	inline void unordered_map<_KeyType, _ValueType>::CopyMap(const unordered_map& other)
	{
		for (const auto& [k, v] : other)
		{
			insert({ k, v });
		}
	}

	template<class _KeyType, class _ValueType>
	inline void unordered_map<_KeyType, _ValueType>::TakeMap(unordered_map&& other)
	{
		m_buckets = jpt::move(other.m_buckets);
		m_size = other.m_size;

		other.clear();
	}
}