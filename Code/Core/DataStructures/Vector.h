#pragma once

#include "Core/MemoryManagement/MemoryOperators.h"

namespace jpt
{
	template<class _ValueType>
	class vector
	{
	public:
		using ValueType = _ValueType;

	private:
		ValueType* m_pBuffer;
		size_t m_size;
		size_t m_capacity;

	public:
		vector();
		vector(const std::initializer_list<ValueType>& initializerList);
		~vector();

		ValueType& operator[](size_t index) { return m_pBuffer[index]; }
		const ValueType& operator[](size_t index) const { return m_pBuffer[index]; }
		size_t size() const { return m_size; }
		size_t capacity() const { return m_capacity; }
		void clear();

	private:
		void UpdateBufferWithNewCapacity(size_t inCapacity);
	};

	template<class _ValueType>
	inline vector<_ValueType>::vector()
		: m_pBuffer(nullptr)
		, m_size(0)
		, m_capacity(0)
	{
	}

	template<class _ValueType>
	inline vector<_ValueType>::vector(const std::initializer_list<ValueType>& initializerList)
		: m_pBuffer(nullptr)
		, m_size(initializerList.size())
		, m_capacity(m_size)
	{
		UpdateBufferWithNewCapacity(m_capacity);
		memcpy(m_pBuffer, initializerList.begin(), initializerList.size() * sizeof(ValueType));
	}

	template<class _ValueType>
	inline vector<_ValueType>::~vector()
	{
		clear();
	}

	template<class _ValueType>
	inline void vector<_ValueType>::clear()
	{
		JPT_SAFE_DELETE_ARRAY(m_pBuffer);
		m_size = 0;
	}

	template<class _ValueType>
	inline void vector<_ValueType>::UpdateBufferWithNewCapacity(size_t inCapacity)
	{
		ValueType* pNewBuffer = JPT_NEW_ARRAY(ValueType, inCapacity);

		if (m_pBuffer)
		{
			memcpy(pNewBuffer, m_pBuffer, m_size * sizeof(ValueType));
			JPT_DELETE_ARRAY(m_pBuffer);
		}

		m_pBuffer = pNewBuffer;
		m_capacity = inCapacity;
	}
}