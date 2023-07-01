// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	/** Encapsulates fixed size arrays of a type */
	template<class _ValueType, size_t kSize>
	class array
	{
	private:
		template<class ArrayType>
		class array_iterator
		{
		private:
			_ValueType* m_pPtr = nullptr;

		public:
			array_iterator(_ValueType* pPtr) : m_pPtr(pPtr) {}

			_ValueType& operator[](size_t index) { return *(m_buffer + index); }
			_ValueType* operator->() { return m_pPtr; }
			_ValueType& operator*() { return *m_pPtr; }
			bool operator==(const array_iterator& other) const { return m_pPtr == other.m_pPtr; }
			bool operator!=(const array_iterator& other) const { return m_pPtr != other.m_pPtr; }

			array_iterator& operator++()
			{
				m_pPtr++;
				return *this;
			}

			array_iterator operator++(int32)
			{
				array_iterator iterator = *this;
				++(*this);
				return iterator;
			}

			array_iterator& operator--()
			{
				m_pPtr--;
				return *this;
			}

			array_iterator operator--(int32)
			{
				array_iterator iterator = *this;
				--(*this);
				return iterator;
			}
		};

	public:
		using ValueType = _ValueType;
		using iterator  = array_iterator<array<ValueType, kSize>>;

	private:
		ValueType m_buffer[kSize];

	public:
		array();
		array(const std::initializer_list<ValueType>& initializerList);
		array(const jpt::array<ValueType, kSize>& other);
		array(jpt::array<ValueType, kSize>&& other) noexcept;
		array& operator=(const jpt::array<ValueType, kSize>& other);
		array& operator=(jpt::array<ValueType, kSize>&& other) noexcept;
		~array();

		// Element access
		ValueType& operator[](size_t index) { return m_buffer[index]; }
		const ValueType& operator[](size_t index) const { return m_buffer[index]; }
		ValueType* data() { return &m_buffer; }
		const ValueType* data() const { return &m_buffer; }
		ValueType& front() { return m_buffer[0]; }
		const ValueType& front() const { return m_buffer[0]; }
		ValueType& back() { return m_buffer[kSize - 1]; }
		const ValueType& back() const { return m_buffer[kSize - 1]; }

		// Iterators
		iterator begin() noexcept { return iterator(m_buffer); }
		const iterator begin() const noexcept { return iterator(m_buffer); }
		iterator end() noexcept { return iterator(m_buffer + kSize); }
		const iterator end() const noexcept { return iterator(m_buffer + kSize); }

		// Capacity
		constexpr size_t size() const { return kSize; }

		// Modifiers
		void clear();
		void fill(const ValueType& value);

	private:
		void CopyArray(const jpt::array<ValueType, kSize>& other);
		void TakeArray(jpt::array<ValueType, kSize>&& other);
	};

	template<class ValueType, size_t kSize>
	inline array<ValueType, kSize>::array()
	{
		fill(ValueType());
	}

	template<class ValueType, size_t kSize>
	inline array<ValueType, kSize>::array(const std::initializer_list<ValueType>& initializerList)
	{
		size_t i = 0;
		for (auto itr = initializerList.begin(); itr != initializerList.end(); ++itr)
		{
			m_buffer[i] = *itr;
			++i;
		}
	}

	template<class ValueType, size_t kSize>
	inline array<ValueType, kSize>::array(const jpt::array<ValueType, kSize>& other)
	{
		CopyArray(other);
	}

	template<class ValueType, size_t kSize>
	inline array<ValueType, kSize>::array(jpt::array<ValueType, kSize>&& other) noexcept
	{
		TakeArray(jpt::move(other));
	}

	template<class ValueType, size_t kSize>
	inline array<ValueType, kSize>& array<ValueType, kSize>::operator=(const jpt::array<ValueType, kSize>& other)
	{
		if (this == &other)
		{
			return;
		}

		CopyArray(other);
		return *this;
	}

	template<class ValueType, size_t kSize>
	inline array<ValueType, kSize>& array<ValueType, kSize>::operator=(jpt::array<ValueType, kSize>&& other) noexcept
	{
		if (this == &other)
		{
			return;
		}

		TakeArray(jpt::move(other));
		return *this;
	}

	template<class ValueType, size_t kSize>
	inline array<ValueType, kSize>::~array()
	{
		clear();
	}

	template<class ValueType, size_t kSize>
	inline void array<ValueType, kSize>::clear()
	{
		if constexpr (!std::is_trivially_destructible_v<ValueType>)
		{
			for (size_t i = 0; i < kSize; ++i)
			{
				m_buffer[i].~ValueType();
			}
		}
	}

	template<class ValueType, size_t kSize>
	inline void array<ValueType, kSize>::fill(const ValueType& value)
	{
		clear();

		for (size_t i = 0; i < kSize; ++i)
		{
			m_buffer[i] = value;
		}
	}

	template<class ValueType, size_t kSize>
	inline void array<ValueType, kSize>::CopyArray(const jpt::array<ValueType, kSize>& other)
	{
		for (size_t i = 0; i < kSize; ++i)
		{
			m_buffer[i] = other.m_buffer[i];
		}
	}

	template<class ValueType, size_t kSize>
	inline void array<ValueType, kSize>::TakeArray(jpt::array<ValueType, kSize>&& other)
	{
		m_buffer = &other.m_buffer;
		other.m_buffer = nullptr;
	}
}