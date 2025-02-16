// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

#include <initializer_list>

export module jpt.Deque;

import jpt.Allocator;
import jpt.Constants;
import jpt.Utilities;
import jpt.TypeDefs;

export namespace jpt
{
	/** Double-ended queue. O(1) for adding/removing from the begin/end, and random accessing. Implemented by an array of fix-sized arrays */
	template<typename _TData>
	class Deque
	{
	private:
		static constexpr Index kChunkSize = 8;

	public:
		using TData = _TData;
		using Chunk = TData*;	// TODO: Consider. Could be TData[kChunkSize]

	private:
		Chunk* m_pChunks = nullptr;          /**< Array to fixed-sized chunks stores data */
		Index m_chunkCount = 0;              /**< Number of chunks allocated. JPT_ARRAY_COUNT(m_pChunks) */
		Index m_frontIndex = kInvalidIndex;  /**< Index of the front element. Start in the middle */
		Index m_count = 0;                   /**< Number of total elements in the deque */

		// m_backIndex: 
		// m_totalSlots: m_chunkCount * kChunkSize

	public:
		constexpr Deque() = default;
		constexpr ~Deque();

	public:
		// Adding
		constexpr void PushFront(const TData& data);
		constexpr void PushBack(const TData& data);

		// Erasing
		constexpr void Clear();

		// Capacity
		constexpr Index Count() const noexcept;
		constexpr bool IsEmpty() const noexcept;

		// Accessing
		constexpr TData& operator[](Index index) noexcept;
		constexpr const TData& operator[](Index index) const noexcept;

	private:
		/** Adds the first element to uninitialized deque */
		constexpr void Init(const TData& data);

		constexpr void Grow();
	};

	template<typename TData>
	constexpr Deque<TData>::~Deque()
	{
		Clear();
	}

	template<typename _TData>
	constexpr void Deque<_TData>::PushFront(const TData& data)
	{
		if (IsEmpty())
		{
			Init(data);
			return;
		}

		// If the front index is at the beginning of the chunk, we need to allocate a new chunk
		if (m_frontIndex == 0)
		{
			Grow();
		}

		--m_frontIndex;

		const Index chunkIndex = m_frontIndex / kChunkSize;
		const Index offset = m_frontIndex % kChunkSize;
		m_pChunks[chunkIndex][offset] = data;

		++m_count;
	}

	template<typename TData>
	constexpr void Deque<TData>::PushBack(const TData& data)
	{
		if (IsEmpty())
		{
			Init(data);
			return;
		}

		Index backIndex = m_frontIndex + m_count;

		if (backIndex >= m_chunkCount * kChunkSize)
		{
			Grow();
			backIndex = m_frontIndex + m_count;  // m_frontIndex could have changed. Recalculate
		}

		const Index chunkIndex = backIndex / kChunkSize;
		const Index offset = backIndex % kChunkSize;
		m_pChunks[chunkIndex][offset] = data;

		++m_count;
	}

	template<typename TData>
	constexpr void Deque<TData>::Clear()
	{
		// Deallocate all chunks
		for (size_t i = 0; i < m_chunkCount; ++i)
		{
			Allocator<TData>::DeallocateArray(m_pChunks[i]);
			m_pChunks[i] = nullptr;
		}

		// Deallocate the array
		Allocator<Chunk>::DeallocateArray(m_pChunks);
		m_pChunks = nullptr;

		// Reset
		m_chunkCount = 0;
		m_frontIndex = kInvalidIndex;
		m_count = 0;
	}

	template<typename TData>
	constexpr Index Deque<TData>::Count() const noexcept
	{
		return m_count;
	}

	template<typename TData>
	constexpr bool Deque<TData>::IsEmpty() const noexcept
	{
		return m_count == 0;
	}

	template<typename TData>
	constexpr TData& Deque<TData>::operator[](Index index) noexcept
	{
		JPT_ASSERT(index < m_count, "Index out of bounds");

		const Index chunkIndex = index / kChunkSize;
		const Index offset     = index % kChunkSize;
		return m_pChunks[chunkIndex][offset];
	}
	template<typename TData>
	constexpr const TData& Deque<TData>::operator[](Index index) const noexcept
	{
		JPT_ASSERT(index < m_count, "Index out of bounds");

		const Index chunkIndex = index / kChunkSize;
		const Index offset     = index % kChunkSize;
		return m_pChunks[chunkIndex][offset];
	}

	template<typename TData>
	constexpr void Deque<TData>::Init(const TData& data)
	{
		JPT_ASSERT(m_pChunks == nullptr && IsEmpty(), "Deque already initialized");

		m_chunkCount = 1;
		m_pChunks = Allocator<Chunk>::AllocateArray(m_chunkCount);
		m_pChunks[0] = Allocator<TData>::AllocateArray(kChunkSize);

		m_frontIndex = kChunkSize / 2; // Start in the middle
		m_pChunks[0][m_frontIndex] = data;
		m_count = 1;
	}

	template<typename TData>
	constexpr void Deque<TData>::Grow()
	{
		static constexpr Index kGrowMultiplier = 2;

		const Index newChunkCount = m_chunkCount * kGrowMultiplier + 1;
		Chunk* pNewChunks = Allocator<Chunk>::AllocateArray(newChunkCount);

		// Allocate new chunks
		for (Index i = 0; i < newChunkCount; ++i)
		{
			pNewChunks[i] = Allocator<TData>::AllocateArray(kChunkSize);
		}

		// Copy the old data to the new chunks
		const Index middleIndex = newChunkCount / 2;
		const Index newFrontIndex = middleIndex * kChunkSize + (kChunkSize / 2);

		for (Index i = 0; i < m_count; ++i)
		{
			const Index oldChunkIndex = (m_frontIndex + i)  / kChunkSize;
			const Index oldOffset     = (m_frontIndex + i)  % kChunkSize;
			const Index newChunkIndex = (newFrontIndex + i) / kChunkSize;
			const Index newOffset     = (newFrontIndex + i) % kChunkSize;

			pNewChunks[newChunkIndex][newOffset] = m_pChunks[oldChunkIndex][oldOffset];
		}

		// Deallocate the old chunks
		Clear();

		// Update the deque
		m_pChunks = pNewChunks;
		m_chunkCount = newChunkCount;
		m_frontIndex = newFrontIndex;
	}
}