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
import jpt.TypeTraits;

export namespace jpt
{
	/** Fixed-Sized Double-ended queue. O(1) for adding/removing from the begin/end, and random accessing. No heap allocation.
		Implemented as a circular array */
	template<typename _TData, Index kCount = 16, typename TAllocator = Allocator<_TData>>
	class Deque
	{
	public:
		using TData = _TData;

	private:
		TData m_buffer[kCount];		/**< Circular buffer */
		Index m_frontIndex = 0;		/**< Front element index */
		Index m_backIndex = 0;		/**< Back element index */
		Index m_count      = 0;		/**< Number of elements in the deque */

	public:
		constexpr ~Deque();
	};

	template<typename TData, Index kCount, typename TAllocator>
	constexpr Deque<TData, kCount, TAllocator>::~Deque()
	{
		if constexpr (!IsTriviallyDestructible<TData>)
		{
			for (Index i = 0; i < m_count; ++i)
			{
				TAllocator::Destruct(m_buffer + i);
			}
		}
	}
}