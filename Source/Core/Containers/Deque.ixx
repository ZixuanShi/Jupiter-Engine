// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

#include <initializer_list>

export module jpt.Deque;

import jpt.Allocator;
import jpt.Utilities;
import jpt.TypeDefs;

export namespace jpt
{
	/** Double-ended queue. O(1) for adding/removing from the begin/end, and random accessing. Implemented by an array of fix-sized arrays */
	template<typename _TData, typename TAllocator = Allocator<_TData>>
	class Deque
	{
	public:
		using TData = _TData;

	private:
		static constexpr Index kChunkSize = 16;

	private:
		TData** m_ppMap = nullptr;  /**< Array to fixed-sized chunks. Where the data is stored on heap. Not contiguous */

		Index m_mapSize = 0;  /**< Number of chunks allocated. JPT_ARRAY_COUNT(m_ppMap) */
		Index m_size = 0;     /**< Number of elements in the deque */
		Index m_front = 0;    /**< Index of the first element */

	public:
		constexpr void PushFront(const TData& data);

	private:
		
	};
}