#pragma once

namespace jpt
{
	template<typename Vector>
	class vector_iterator
	{
	public:
		using ValueType = typename Vector::ValueType;

	private:
		ValueType* m_ptr;

	public:
		// Member functions
		constexpr vector_iterator(ValueType* ptr);

		// Meant to be virtual
		vector_iterator& operator++();
		vector_iterator operator++(int32);
		vector_iterator& operator--();
		vector_iterator operator--(int32);

		ValueType& operator[](size_t index) { return *(m_ptr + index); }
		ValueType* operator->() { return m_ptr; }
		ValueType& operator*() { return *m_ptr; }
		bool operator==(const vector_iterator& other) const { return m_ptr == other.m_ptr; }
		bool operator!=(const vector_iterator& other) const { return m_ptr != other.m_ptr; }
	};

	template<typename Vector>
	inline constexpr vector_iterator<Vector>::vector_iterator(ValueType* ptr)
		: m_ptr(ptr)
	{
	}

	template<typename Vector>
	inline vector_iterator<Vector>& vector_iterator<Vector>::operator++()
	{
		m_ptr++;
		return *this;
	}

	template<typename Vector>
	inline vector_iterator<Vector> vector_iterator<Vector>::operator++(int32)
	{
		vector_iterator iterator = *this;
		++(*this);
		return iterator;
	}

	template<typename Vector>
	inline vector_iterator<Vector>& vector_iterator<Vector>::operator--()
	{
		m_ptr--;
		return *this;
	}

	template<typename Vector>
	inline vector_iterator<Vector> vector_iterator<Vector>::operator--(int32)
	{
		vector_iterator iterator = *this;
		--(*this);
		return iterator;
	}

	template<class _ValueType>
	class vector
	{
	public:
		using ValueType = _ValueType;
		using iterator = vector_iterator<vector<ValueType>>;

	private:
		ValueType* m_pBuffer = nullptr;
		size_t m_size		 = 0;
		size_t m_capacity	 = 0;
		static constexpr size_t kCapacityMultiplier = 2;

	public:
		vector() = default;
		vector(const std::initializer_list<ValueType>& initializerList);
		vector(const jpt::vector<ValueType>& other);
		vector(jpt::vector<ValueType>&& other) noexcept;
		vector& operator=(const jpt::vector<ValueType>& other);
		vector& operator=(jpt::vector<ValueType>&& other) noexcept;
		~vector();

		// Element access
		ValueType& operator[](size_t index) { return m_pBuffer[index]; }
		const ValueType& operator[](size_t index) const { return m_pBuffer[index]; }
		ValueType& back() { return m_pBuffer[m_size - 1]; }
		const ValueType& back() const { return m_pBuffer[m_size - 1]; }
		ValueType& front() { return m_pBuffer[0]; }
		const ValueType& front() const { return m_pBuffer[0]; }
		ValueType* data(){ return m_pBuffer; }
		const ValueType* data() const { return m_pBuffer; }

		// Capacity
		bool empty() const { return m_size == 0; }
		size_t size() const { return m_size; }
		size_t capacity() const { return m_capacity; }
		void reserve(size_t inCapacity);

		// Iterators
		iterator begin() noexcept { return iterator(m_pBuffer); }
		const iterator begin() const noexcept { return iterator(m_pBuffer); }
		const iterator cbegin() const noexcept { return iterator(m_pBuffer); }
		iterator end() noexcept { return iterator(m_pBuffer + m_size); }
		const iterator end() const noexcept { return iterator(m_pBuffer + m_size); }
		const iterator cend() const noexcept { return iterator(m_pBuffer + m_size); }

		// Modifiers
		void clear();
		void push_back(const ValueType& value);
		void push_back(ValueType&& value);
		template<class... Args> void emplace_back(Args&&... value);
		void pop_back();
		void erase(size_t index);
		void insert(size_t index, const ValueType& value);
		void insert(size_t index, ValueType&& value);

	private:
		// Create a new data buffer with a new capacity, move the existing data over
		// - inCapacity: The new buffer's size
		void UpdateBufferWithNewCapacity(size_t inCapacity);
		
		// Copy the data from another vector, create them on heap and point to it
		void CopyVector(const jpt::vector<ValueType>& other);

		// Takes over ownership of data from another vector
		void TakeVector(jpt::vector<ValueType>&& other);

		// Shift the elements inside buffer
		// - index: Where to start shifting
		// - isToEnd: True if we are shifting from begin to end, false if we are shifting from end to begin
		void ShiftBuffer(size_t index, bool isToEnd);
	};

	template<class _ValueType>
	inline vector<_ValueType>::vector(const std::initializer_list<ValueType>& initializerList)
	{
		m_size = initializerList.size();

		UpdateBufferWithNewCapacity(m_size);
		JPT_ASSERT(m_pBuffer, "m_pBuffer should not be nullptr");

		size_t i = 0;
		for (auto itr = initializerList.begin(); itr != initializerList.end(); ++itr)
		{
			m_pBuffer[i] = *itr;
			++i;
		}
	}

	template<class _ValueType>
	inline vector<_ValueType>::vector(const jpt::vector<ValueType>& other)
	{
		CopyVector(other);
	}

	template<class _ValueType>
	inline vector<_ValueType>::vector(jpt::vector<ValueType>&& other) noexcept
	{
		TakeVector(jpt::move(other));
	}

	template<class _ValueType>
	inline vector<_ValueType>& vector<_ValueType>::operator=(const jpt::vector<ValueType>& other)
	{
		if (this == &other)
		{
			return *this;
		}

		clear();
		CopyVector(other);
		return *this;
	}

	template<class _ValueType>
	inline vector<_ValueType>& vector<_ValueType>::operator=(jpt::vector<ValueType>&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		clear();
		TakeVector(jpt::move(other));
		return *this;
	}

	template<class _ValueType>
	inline vector<_ValueType>::~vector()
	{
		clear();
	}

	template<class _ValueType>
	inline void vector<_ValueType>::reserve(size_t inCapacity)
	{
		if (inCapacity > m_capacity)
		{
			UpdateBufferWithNewCapacity(inCapacity);
		}
	}

	template<class _ValueType>
	inline void vector<_ValueType>::clear()
	{
		if constexpr (!std::is_trivially_destructible_v<ValueType>)
		{
			for (size_t i = 0; i < m_size; ++i)
			{
				m_pBuffer[i].~ValueType();
			}
		}

		JPT_SAFE_DELETE_ARRAY(m_pBuffer);
		m_size = 0;
		m_capacity = 0;
	}

	template<class _ValueType>
	inline void vector<_ValueType>::push_back(const _ValueType& value)
	{
		if (m_size >= m_capacity)
		{
			UpdateBufferWithNewCapacity(m_size * kCapacityMultiplier);
		}

		::new(m_pBuffer + m_size) _ValueType(value);
		++m_size;
	}

	template<class _ValueType>
	inline void vector<_ValueType>::push_back(_ValueType&& value)
	{
		if (m_size >= m_capacity)
		{
			UpdateBufferWithNewCapacity(m_size * kCapacityMultiplier);
		}

		::new(m_pBuffer + m_size) _ValueType(jpt::move(value));
		++m_size;
	}

	template<class _ValueType>
	template<class ...Args>
	inline void vector<_ValueType>::emplace_back(Args&& ...value)
	{
		if (m_size >= m_capacity)
		{
			UpdateBufferWithNewCapacity(m_size * kCapacityMultiplier);
		}

		::new(m_pBuffer + m_size) _ValueType(std::forward<Args>(value)...);
		++m_size;
	}

	template<class _ValueType>
	inline void vector<_ValueType>::pop_back()
	{
		JPT_ASSERT(!empty(), "Calling pop_back() on a empty vector");

		--m_size;
		if constexpr (!std::is_trivially_destructible_v<ValueType>)
		{
			m_pBuffer[m_size].~ValueType();
		}
	}

	template<class _ValueType>
	inline void vector<_ValueType>::erase(size_t index)
	{
		JPT_ASSERT(index < m_size, "Calling erase() with an invalid index");

		--m_size;
		if constexpr (!std::is_trivially_destructible_v<ValueType>)
		{
			m_pBuffer[index].~ValueType();
		}

		ShiftBuffer(index, false);
	}

	template<class _ValueType>
	inline void vector<_ValueType>::insert(size_t index, const ValueType& value)
	{
		JPT_ASSERT(index <= m_size, "Calling insert() with an invalid index");

		if (m_size >= m_capacity)
		{
			UpdateBufferWithNewCapacity(m_size * kCapacityMultiplier);
		}

		ShiftBuffer(index, true);

		new(m_pBuffer + index) ValueType(value);
		++m_size;
	}

	template<class _ValueType>
	inline void vector<_ValueType>::insert(size_t index, ValueType&& value)
	{
		JPT_ASSERT(index <= m_size, "Calling insert() with an invalid index");

		if (m_size >= m_capacity)
		{
			UpdateBufferWithNewCapacity(m_size * kCapacityMultiplier);
		}

		ShiftBuffer(index, true);

		new(m_pBuffer + index) ValueType(jpt::move(value));
		++m_size;
	}

	template<class _ValueType>
	inline void vector<_ValueType>::UpdateBufferWithNewCapacity(size_t inCapacity)
	{
		ValueType* pNewBuffer = new ValueType[inCapacity];

		if (m_pBuffer)
		{
			if constexpr (std::is_trivially_copyable_v<ValueType>)
			{
				std::memmove(pNewBuffer, m_pBuffer, m_size * sizeof(ValueType));
			}
			else
			{
				JPT_ASSERT(pNewBuffer, "pNewBuffer should be initialized");
				for (size_t i = 0; i < m_size; ++i)
				{
					pNewBuffer[i] = jpt::move(m_pBuffer[i]);
				}
			}

			delete[] m_pBuffer;
		}

		m_pBuffer = pNewBuffer;
		m_capacity = inCapacity;
	}

	template<class _ValueType>
	inline void vector<_ValueType>::CopyVector(const jpt::vector<ValueType>& other)
	{
		m_size = other.size();
		UpdateBufferWithNewCapacity(other.capacity());

		JPT_ASSERT(m_pBuffer, "m_pBuffer shouldn't be nullptr");

		if constexpr (std::is_trivially_copyable_v<ValueType>)
		{
			memcpy(m_pBuffer, other.data(), m_size * sizeof(ValueType));
		}
		else
		{
			for (size_t i = 0; i < m_size; ++i)
			{
				m_pBuffer[i] = other.m_pBuffer[i];
			}
		}
	}

	template<class _ValueType>
	inline void vector<_ValueType>::TakeVector(jpt::vector<ValueType>&& other)
	{
		m_pBuffer  = other.m_pBuffer;
		m_size     = other.m_size;
		m_capacity = other.m_capacity;

		other.m_pBuffer  = nullptr;
		other.m_size     = 0;
		other.m_capacity = 0;
	}

	template<class _ValueType>
	inline void vector<_ValueType>::ShiftBuffer(size_t index, bool isToEnd)
	{
		if (isToEnd)
		{
			if constexpr (std::is_trivially_copyable_v<ValueType>)
			{
				std::memmove(m_pBuffer + index + 1, m_pBuffer + index, (m_size - index) * sizeof(ValueType));
			}
			else
			{
				for (size_t i = m_size; i > index; --i)
				{
					m_pBuffer[i] = jpt::move(m_pBuffer[i - 1]);
				}
			}
		}
		else
		{
			if constexpr (std::is_trivially_copyable_v<ValueType>)
			{
				std::memmove(m_pBuffer + index, m_pBuffer + index + 1, (m_size - index) * sizeof(ValueType));
			}
			else
			{
				for (size_t i = index; i < m_size; ++i)
				{
					m_pBuffer[i] = jpt::move(m_pBuffer[i + 1]);
				}
			}
		}
	}
}