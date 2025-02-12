// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

#include <initializer_list>

export module jpt.Deque;

import jpt.Allocator;
import jpt.Utilities;

export namespace jpt
{
	/** Double-ended queue. O(1) for adding/removing from the begin/end, and random accessing. Implemented by an array of fix-sized arrays */
	template<typename _TData, typename TAllocator = Allocator<_TData>>
	class Deque
	{
	public:
		using TData = _TData;

	private:
		static constexpr size_t kChunkSize = 16;

	private:
		TData** m_ppMap = nullptr;	/**< Array to arrays. Contains data map */



	public:
	};
}