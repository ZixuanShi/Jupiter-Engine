#pragma once

#include "Core/MemoryManagement/MemoryOperators.h"
#include "Core/Debugging/DebuggingMacros.h"

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
		ValueType* m_pBuffer;
		size_t m_size;
		size_t m_capacity;

	public:
		constexpr explicit vector();
		constexpr explicit vector(const std::initializer_list<ValueType>& initializerList);
		constexpr ~vector();

		// Element access
		constexpr ValueType& operator[](size_t index) { return m_pBuffer[index]; }
		constexpr const ValueType& operator[](size_t index) const { return m_pBuffer[index]; }

		// Capacity
		constexpr size_t size() const { return m_size; }
		constexpr size_t capacity() const { return m_capacity; }

		// Iterators
		constexpr iterator begin() noexcept { return iterator(m_pBuffer); }
		constexpr const iterator begin() const noexcept { return iterator(m_pBuffer); }
		constexpr const iterator cbegin() const noexcept { return iterator(m_pBuffer); }
		constexpr iterator end() noexcept { return iterator(m_pBuffer + m_size); }
		constexpr const iterator end() const noexcept { return iterator(m_pBuffer + m_size); }
		constexpr const iterator cend() const noexcept { return iterator(m_pBuffer + m_size); }

		// Modifiers
		constexpr void clear();

	private:
		constexpr void UpdateBufferWithNewCapacity(size_t inCapacity);
	};

	template<class _ValueType>
	constexpr inline vector<_ValueType>::vector()
		: m_pBuffer(nullptr)
		, m_size(0)
		, m_capacity(0)
	{
	}

	template<class _ValueType>
	constexpr inline vector<_ValueType>::vector(const std::initializer_list<ValueType>& initializerList)
		: m_pBuffer(nullptr)
		, m_size(initializerList.size())
		, m_capacity(m_size)
	{
		UpdateBufferWithNewCapacity(m_capacity);
		JPT_ASSERT(m_pBuffer, "m_pBuffer should not be nullptr");

		size_t i = 0;
		for (auto itr = initializerList.begin(); itr != initializerList.end(); ++itr)
		{
			m_pBuffer[i] = *itr;
			++i;
		}
	}

	template<class _ValueType>
	constexpr inline vector<_ValueType>::~vector()
	{
		clear();
	}

	template<class _ValueType>
	constexpr inline void vector<_ValueType>::clear()
	{
		JPT_SAFE_DELETE_ARRAY(m_pBuffer);
		m_size = 0;
	}

	template<class _ValueType>
	constexpr inline void vector<_ValueType>::UpdateBufferWithNewCapacity(size_t inCapacity)
	{
		ValueType* pNewBuffer = new ValueType[inCapacity];

		if (m_pBuffer)
		{
			memcpy(pNewBuffer, m_pBuffer, m_size * sizeof(ValueType));
			JPT_DELETE_ARRAY(m_pBuffer);
		}

		m_pBuffer = pNewBuffer;
		m_capacity = inCapacity;
	}
}