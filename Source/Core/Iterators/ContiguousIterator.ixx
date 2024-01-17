// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.ContiguousIterator;

import jpt.TypeDefs;

export namespace jpt_private
{
	/** Iterator for linear & contiguous data in memory, for example, Static Array, Dynamic Array, String
		Containers that is linear but non-contiguous shouldn't use this, like LinkedList, should use LinearNodeBasedIterator instead */
	template<typename _DataT>
	class ContiguousIterator
	{
	public:
		using DataT = _DataT;

	private:
		DataT* m_pPtr = nullptr;

	public:
		ContiguousIterator() = default;
		ContiguousIterator(DataT* pPtr) : m_pPtr(pPtr) {}

		ContiguousIterator& operator++();
		ContiguousIterator operator++(int32);

		ContiguousIterator& operator+=(size_t offset);
		ContiguousIterator operator+(size_t offset);

		ContiguousIterator& operator--();
		ContiguousIterator operator--(int32);

		ContiguousIterator& operator-=(size_t offset);
		ContiguousIterator operator-(size_t offset);

		DataT& operator[](size_t index) { return *(m_pPtr + index); }
		DataT* operator->() { return m_pPtr; }
		DataT& operator*() { return *m_pPtr; }

		bool operator< (const ContiguousIterator& other) const { return m_pPtr <  other.m_pPtr; }
		bool operator<=(const ContiguousIterator& other) const { return m_pPtr <= other.m_pPtr; }

		bool operator> (const ContiguousIterator& other) const { return m_pPtr >  other.m_pPtr; }
		bool operator>=(const ContiguousIterator& other) const { return m_pPtr >= other.m_pPtr; }

		bool operator==(const ContiguousIterator& other) const { return m_pPtr == other.m_pPtr; }
		bool operator!=(const ContiguousIterator& other) const { return m_pPtr != other.m_pPtr; }
	};

	template<typename DataT>
	ContiguousIterator<DataT>& ContiguousIterator<DataT>::operator++()
	{
		++m_pPtr;
		return *this;
	}

	template<typename DataT>
	ContiguousIterator<DataT> ContiguousIterator<DataT>::operator++(int32)
	{
		ContiguousIterator iterator = *this;
		++m_pPtr;
		return iterator;
	}

	template<typename DataT>
	ContiguousIterator<DataT>& ContiguousIterator<DataT>::operator+=(size_t offset)
	{
		m_pPtr += offset;
		return *this;
	}

	template<typename DataT>
	ContiguousIterator<DataT> ContiguousIterator<DataT>::operator+(size_t offset)
	{
		ContiguousIterator iterator = *this;
		return iterator += offset;
	}

	template<typename DataT>
	ContiguousIterator<DataT>& ContiguousIterator<DataT>::operator--()
	{
		--m_pPtr;
		return *this;
	}

	template<typename DataT>
	ContiguousIterator<DataT> ContiguousIterator<DataT>::operator--(int32)
	{
		ContiguousIterator iterator = *this;
		--m_pPtr;
		return iterator;
	}

	template<typename DataT>
	ContiguousIterator<DataT>& ContiguousIterator<DataT>::operator-=(size_t offset)
	{
		m_pPtr -= offset;
		return *this;
	}

	template<typename DataT>
	ContiguousIterator<DataT> ContiguousIterator<DataT>::operator-(size_t offset)
	{
		ContiguousIterator iterator = *this;
		return iterator -= offset;
	}
}