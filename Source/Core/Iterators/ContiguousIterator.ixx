// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.ContiguousIterator;

import jpt.TypeDefs;

export namespace jpt_private
{
	/** Iterator for linear & contiguous data in memory, for example, Static Array, Dynamic Array, String
		Containers that is linear but non-contiguous shouldn't use this, like LinkedList, should use LinearNodeBasedIterator instead */
	template<typename TData>
	class ContiguousIterator
	{
	private:
		TData* m_pPtr = nullptr;

	public:
		constexpr ContiguousIterator() = default;
		constexpr ContiguousIterator(TData* pPtr) : m_pPtr(pPtr) {}

		constexpr ContiguousIterator& operator++();
		constexpr ContiguousIterator operator++(int32);

		constexpr ContiguousIterator& operator+=(size_t offset);
		constexpr ContiguousIterator operator+(size_t offset);

		constexpr ContiguousIterator& operator--();
		constexpr ContiguousIterator operator--(int32);

		constexpr ContiguousIterator& operator-=(size_t offset);
		constexpr ContiguousIterator operator-(size_t offset);

		constexpr TData& operator[](size_t index) { return *(m_pPtr + index); }
		constexpr TData* operator->() { return m_pPtr; }
		constexpr TData& operator*()  { return *m_pPtr; }

		constexpr bool operator< (const ContiguousIterator& other) const { return m_pPtr <  other.m_pPtr; }
		constexpr bool operator<=(const ContiguousIterator& other) const { return m_pPtr <= other.m_pPtr; }

		constexpr bool operator> (const ContiguousIterator& other) const { return m_pPtr >  other.m_pPtr; }
		constexpr bool operator>=(const ContiguousIterator& other) const { return m_pPtr >= other.m_pPtr; }

		constexpr bool operator==(const ContiguousIterator& other) const { return m_pPtr == other.m_pPtr; }
		constexpr bool operator!=(const ContiguousIterator& other) const { return m_pPtr != other.m_pPtr; }
	};

	template<typename TData>
	constexpr ContiguousIterator<TData>& ContiguousIterator<TData>::operator++()
	{
		++m_pPtr;
		return *this;
	}

	template<typename TData>
	constexpr ContiguousIterator<TData> ContiguousIterator<TData>::operator++(int32)
	{
		ContiguousIterator iterator = *this;
		++m_pPtr;
		return iterator;
	}

	template<typename TData>
	constexpr ContiguousIterator<TData>& ContiguousIterator<TData>::operator+=(size_t offset)
	{
		m_pPtr += offset;
		return *this;
	}

	template<typename TData>
	constexpr ContiguousIterator<TData> ContiguousIterator<TData>::operator+(size_t offset)
	{
		ContiguousIterator iterator = *this;
		return iterator += offset;
	}

	template<typename TData>
	constexpr ContiguousIterator<TData>& ContiguousIterator<TData>::operator--()
	{
		--m_pPtr;
		return *this;
	}

	template<typename TData>
	constexpr ContiguousIterator<TData> ContiguousIterator<TData>::operator--(int32)
	{
		ContiguousIterator iterator = *this;
		--m_pPtr;
		return iterator;
	}

	template<typename TData>
	constexpr ContiguousIterator<TData>& ContiguousIterator<TData>::operator-=(size_t offset)
	{
		m_pPtr -= offset;
		return *this;
	}

	template<typename TData>
	constexpr ContiguousIterator<TData> ContiguousIterator<TData>::operator-(size_t offset)
	{
		ContiguousIterator iterator = *this;
		return iterator -= offset;
	}

	template<typename TData>
	class ConstContiguousIterator
	{
	private:
		const TData* m_pPtr = nullptr;

	public:
		constexpr ConstContiguousIterator() = default;
		constexpr ConstContiguousIterator(const TData* pPtr) : m_pPtr(pPtr) {}

		constexpr ConstContiguousIterator& operator++();
		constexpr ConstContiguousIterator operator++(int32);
		
		constexpr ConstContiguousIterator& operator+=(size_t offset);
		constexpr ConstContiguousIterator operator+(size_t offset);
		
		constexpr ConstContiguousIterator& operator--();
		constexpr ConstContiguousIterator operator--(int32);
		
		constexpr ConstContiguousIterator& operator-=(size_t offset);
		constexpr ConstContiguousIterator operator-(size_t offset);
		
		constexpr const TData& operator[](size_t index) const { return *(m_pPtr + index); }
		constexpr const TData* operator->() const { return m_pPtr; }
		constexpr const TData& operator*() const { return *m_pPtr; }
		
		constexpr bool operator< (const ConstContiguousIterator& other) const { return m_pPtr < other.m_pPtr; }
		constexpr bool operator<=(const ConstContiguousIterator& other) const { return m_pPtr <= other.m_pPtr; }
		
		constexpr bool operator> (const ConstContiguousIterator& other) const { return m_pPtr > other.m_pPtr; }
		constexpr bool operator>=(const ConstContiguousIterator& other) const { return m_pPtr >= other.m_pPtr; }
		
		constexpr bool operator==(const ConstContiguousIterator& other) const { return m_pPtr == other.m_pPtr; }
		constexpr bool operator!=(const ConstContiguousIterator& other) const { return m_pPtr != other.m_pPtr; }
	};

	template<typename TData>
	constexpr ConstContiguousIterator<TData>& ConstContiguousIterator<TData>::operator++()
	{
		++m_pPtr;
		return *this;
	}

	template<typename TData>
	constexpr ConstContiguousIterator<TData> ConstContiguousIterator<TData>::operator++(int32)
	{
		ConstContiguousIterator iterator = *this;
		++m_pPtr;
		return iterator;
	}

	template<typename TData>
	constexpr ConstContiguousIterator<TData>& ConstContiguousIterator<TData>::operator+=(size_t offset)
	{
		m_pPtr += offset;
		return *this;
	}

	template<typename TData>
	constexpr ConstContiguousIterator<TData> ConstContiguousIterator<TData>::operator+(size_t offset)
	{
		ConstContiguousIterator iterator = *this;
		return iterator += offset;
	}

	template<typename TData>
	constexpr ConstContiguousIterator<TData>& ConstContiguousIterator<TData>::operator--()
	{
		--m_pPtr;
		return *this;
	}

	template<typename TData>
	constexpr ConstContiguousIterator<TData> ConstContiguousIterator<TData>::operator--(int32)
	{
		ConstContiguousIterator iterator = *this;
		--m_pPtr;
		return iterator;
	}

	template<typename TData>
	constexpr ConstContiguousIterator<TData>& ConstContiguousIterator<TData>::operator-=(size_t offset)
	{
		m_pPtr -= offset;
		return *this;
	}

	template<typename TData>
	constexpr ConstContiguousIterator<TData> ConstContiguousIterator<TData>::operator-(size_t offset)
	{
		ConstContiguousIterator iterator = *this;
		return iterator -= offset;
	}
}