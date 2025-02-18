// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.CircularArrayIterator;

import jpt.TypeDefs;
import jpt.TypeTraits;

export namespace jpt_private
{
	template<typename TData, Index kCapacity>
	class CircularArrayIterator
	{
	private:
		TData* m_pBuffer = nullptr;
		int64 m_index = 0;

		bool m_shouldWrap = false;
		
	public:
		constexpr CircularArrayIterator() = default;
		constexpr CircularArrayIterator(TData* pBuffer, int64 index, bool endIsBegin);

		constexpr CircularArrayIterator& operator++();
		constexpr CircularArrayIterator operator++(int32);

		constexpr CircularArrayIterator& operator+=(size_t offset);
		constexpr CircularArrayIterator operator+(size_t offset);

		constexpr CircularArrayIterator& operator--();
		constexpr CircularArrayIterator operator--(int32);

		constexpr CircularArrayIterator& operator-=(size_t offset);
		constexpr CircularArrayIterator operator-(size_t offset);

		constexpr       TData& operator*()        noexcept { return  m_pBuffer[m_index]; }
		constexpr const TData& operator*()  const noexcept { return  m_pBuffer[m_index]; }
		constexpr       TData* operator->()       noexcept { return &m_pBuffer[m_index]; }
		constexpr const TData* operator->() const noexcept { return &m_pBuffer[m_index]; }

		constexpr bool operator==(const CircularArrayIterator& other) const noexcept;
	};

	template<typename TData, Index kCapacity>
	constexpr CircularArrayIterator<TData, kCapacity>::CircularArrayIterator(TData* pBuffer, int64 index, bool endIsBegin)
		: m_pBuffer(pBuffer)
		, m_index(index)
		, m_shouldWrap(endIsBegin)
	{
	}

	template<typename TData, Index kCapacity>
	constexpr CircularArrayIterator<TData, kCapacity>& CircularArrayIterator<TData, kCapacity>::operator++()
	{
		++m_index;
		
		if (m_index == kCapacity)
		{
			m_index = 0;
			m_shouldWrap = false;
		}
		
		return *this;
	}

	template<typename TData, Index kCapacity>
	constexpr CircularArrayIterator<TData, kCapacity> CircularArrayIterator<TData, kCapacity>::operator++(int32)
	{
		CircularArrayIterator iterator = *this;
		++(*this);
		return iterator;
	}

	template<typename TData, Index kCapacity>
	constexpr CircularArrayIterator<TData, kCapacity>& CircularArrayIterator<TData, kCapacity>::operator+=(size_t offset)
	{
		for (size_t i = 0; i < offset; ++i)
		{
			++(*this);
		}
		return *this;
	}

	template<typename TData, Index kCapacity>
	constexpr CircularArrayIterator<TData, kCapacity> CircularArrayIterator<TData, kCapacity>::operator+(size_t offset)
	{
		CircularArrayIterator iterator = *this;
		return iterator += offset;
	}

	template<typename TData, Index kCapacity>
	constexpr CircularArrayIterator<TData, kCapacity>& CircularArrayIterator<TData, kCapacity>::operator--()
	{
		--m_index;

		if (m_index < 0)
		{
			m_index = kCapacity - 1;
			m_shouldWrap = false;
		}

		return *this;
	}

	template<typename TData, Index kCapacity>
	constexpr CircularArrayIterator<TData, kCapacity> CircularArrayIterator<TData, kCapacity>::operator--(int32)
	{
		CircularArrayIterator iterator = *this;
		--(*this);
		return iterator;
	}

	template<typename TData, Index kCapacity>
	constexpr CircularArrayIterator<TData, kCapacity>& CircularArrayIterator<TData, kCapacity>::operator-=(size_t offset)
	{
		for (size_t i = 0; i < offset; ++i)
		{
			--(*this);
		}
		return *this;
	}

	template<typename TData, Index kCapacity>
	constexpr CircularArrayIterator<TData, kCapacity> CircularArrayIterator<TData, kCapacity>::operator-(size_t offset)
	{
		CircularArrayIterator iterator = *this;
		return iterator -= offset;
	}

	template<typename TData, Index kCapacity>
	constexpr bool CircularArrayIterator<TData, kCapacity>::operator==(const CircularArrayIterator& other) const noexcept
	{
		return !m_shouldWrap &&
			   m_pBuffer == other.m_pBuffer &&
			   m_index   == other.m_index;
	}

	template<typename TData, Index kCapacity>
	class ConstCircularArrayIterator
	{
	private:
		const TData* m_pBuffer = nullptr;
		int64 m_index = 0;
		bool m_shouldWrap = false;

	public:
		constexpr ConstCircularArrayIterator() = default;
		constexpr ConstCircularArrayIterator(const TData* pBuffer, int64 index, bool endIsBegin);

		constexpr ConstCircularArrayIterator& operator++();
		constexpr ConstCircularArrayIterator operator++(int32);

		constexpr ConstCircularArrayIterator& operator+=(size_t offset);
		constexpr ConstCircularArrayIterator operator+(size_t offset);

		constexpr ConstCircularArrayIterator& operator--();
		constexpr ConstCircularArrayIterator operator--(int32);

		constexpr ConstCircularArrayIterator& operator-=(size_t offset);
		constexpr ConstCircularArrayIterator operator-(size_t offset);

		constexpr const TData& operator*()  const noexcept { return  m_pBuffer[m_index]; }
		constexpr const TData* operator->() const noexcept { return &m_pBuffer[m_index]; }

		constexpr bool operator==(const ConstCircularArrayIterator& other) const noexcept;
	};

	template<typename TData, Index kCapacity>
	constexpr ConstCircularArrayIterator<TData, kCapacity>::ConstCircularArrayIterator(const TData* pBuffer, int64 index, bool endIsBegin)
		: m_pBuffer(pBuffer)
		, m_index(index)
		, m_shouldWrap(endIsBegin)
	{
	}

	template<typename TData, Index kCapacity>
	constexpr ConstCircularArrayIterator<TData, kCapacity>& ConstCircularArrayIterator<TData, kCapacity>::operator++()
	{
		++m_index;

		if (m_index == kCapacity)
		{
			m_index = 0;
			m_shouldWrap = false;
		}

		return *this;
	}

	template<typename TData, Index kCapacity>
	constexpr ConstCircularArrayIterator<TData, kCapacity> ConstCircularArrayIterator<TData, kCapacity>::operator++(int32)
	{
		ConstCircularArrayIterator iterator = *this;
		++(*this);
		return iterator;
	}

	template<typename TData, Index kCapacity>
	constexpr ConstCircularArrayIterator<TData, kCapacity>& ConstCircularArrayIterator<TData, kCapacity>::operator+=(size_t offset)
	{
		for (size_t i = 0; i < offset; ++i)
		{
			++(*this);
		}
		return *this;
	}

	template<typename TData, Index kCapacity>
	constexpr ConstCircularArrayIterator<TData, kCapacity> ConstCircularArrayIterator<TData, kCapacity>::operator+(size_t offset)
	{
		ConstCircularArrayIterator iterator = *this;
		return iterator += offset;
	}

	template<typename TData, Index kCapacity>
	constexpr ConstCircularArrayIterator<TData, kCapacity>& ConstCircularArrayIterator<TData, kCapacity>::operator--()
	{
		--m_index;

		if (m_index < 0)
		{
			m_index = kCapacity - 1;
			m_shouldWrap = false;
		}

		return *this;
	}

	template<typename TData, Index kCapacity>
	constexpr ConstCircularArrayIterator<TData, kCapacity> ConstCircularArrayIterator<TData, kCapacity>::operator--(int32)
	{
		ConstCircularArrayIterator iterator = *this;
		--(*this);
		return iterator;
	}

	template<typename TData, Index kCapacity>
	constexpr ConstCircularArrayIterator<TData, kCapacity>& ConstCircularArrayIterator<TData, kCapacity>::operator-=(size_t offset)
	{
		for (size_t i = 0; i < offset; ++i)
		{
			--(*this);
		}
		return *this;
	}

	template<typename TData, Index kCapacity>
	constexpr ConstCircularArrayIterator<TData, kCapacity> ConstCircularArrayIterator<TData, kCapacity>::operator-(size_t offset)
	{
		ConstCircularArrayIterator iterator = *this;
		return iterator -= offset;
	}

	template<typename TData, Index kCapacity>
	constexpr bool ConstCircularArrayIterator<TData, kCapacity>::operator==(const ConstCircularArrayIterator& other) const noexcept
	{
		return !m_shouldWrap &&
			   m_pBuffer == other.m_pBuffer &&
			   m_index   == other.m_index;
	}
}