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

import jpt_private.IteratorCircularArray;

export namespace jpt
{
    /** Fixed-Sized Double-ended queue. O(1) for adding/removing from the begin/end, and random accessing. No heap allocation.
        Implemented as a circular array */
    template<typename _TData, Index kCapacity = 16, typename TAllocator = Allocator<_TData>>
    class Deque
    {
    public:
        using TData         = _TData;
        using Iterator      = jpt_private::IteratorCircularArray<TData, kCapacity>;
        using ConstIterator = jpt_private::ConstIteratorCircularArray<TData, kCapacity>;

    private:
        TData m_buffer[kCapacity];                   /**< Circular buffer */
        int64 m_frontIndex = kInvalidValue<int64>;   /**< Front element index. Grow towards begin */
        int64 m_backIndex  = kInvalidValue<int64>;   /**< Back element index. Grow towards end */
        Index m_count      = 0;                      /**< Number of elements in the deque */

    public:
        constexpr Deque() = default;
        constexpr Deque(const Deque& other);
        constexpr Deque(Deque&& other) noexcept;
        constexpr Deque& operator=(const Deque& other);
        constexpr Deque& operator=(Deque&& other) noexcept;
        constexpr ~Deque();

    public:
        // Adding
        constexpr void AddFront(const TData& data);
        constexpr void AddBack(const TData& data);

        // Erasing
        constexpr void PopFront();
        constexpr void PopBack();
        constexpr void Clear() noexcept;

        // Accessing
        constexpr       TData& Front()       noexcept;
        constexpr const TData& Front() const noexcept;
        constexpr       TData& Back()        noexcept;
        constexpr const TData& Back()  const noexcept;

        // Iterators
        constexpr Iterator begin() noexcept;
        constexpr Iterator end()   noexcept;
        constexpr ConstIterator begin()  const noexcept;
        constexpr ConstIterator cbegin() const noexcept;
        constexpr ConstIterator end()    const noexcept;
        constexpr ConstIterator cend()   const noexcept;

        // Capacity
        constexpr Index Count()    const noexcept;    /**< Number of current elements in the deque */
        constexpr Index Capacity() const noexcept;    /**< Maximum number of elements the deque can hold */
        constexpr bool  IsEmpty()  const noexcept;    /**< Returns true if the deque is empty */
        constexpr bool  IsFull()   const noexcept;    /**< Returns true if the deque is full */

    private:
        constexpr void CopyData(const Deque& other);
        constexpr void MoveData(Deque&& other) noexcept;

        constexpr void Init(const TData& data);
        constexpr int64 GetNextIndex(int64 index) const noexcept;
    };

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr Deque<TData, kCapacity, TAllocator>::Deque(const Deque& other)
    {
        CopyData(other);
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr Deque<TData, kCapacity, TAllocator>::Deque(Deque&& other) noexcept
    {
        MoveData(Move(other));
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr Deque<TData, kCapacity, TAllocator>& Deque<TData, kCapacity, TAllocator>::operator=(const Deque& other)
    {
        if (this != &other)
        {
            Clear();
            CopyData(other);
        }

        return *this;
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr Deque<TData, kCapacity, TAllocator>& Deque<TData, kCapacity, TAllocator>::operator=(Deque&& other) noexcept
    {
        if (this != &other)
        {
            Clear();
            MoveData(Move(other));
        }

        return *this;
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr Deque<TData, kCapacity, TAllocator>::~Deque()
    {
        Clear();
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr void Deque<TData, kCapacity, TAllocator>::AddFront(const TData& data)
    {
        JPT_ASSERT(!IsFull(), "Deque is full");

        if (IsEmpty())
        {
            Init(data);
            return;
        }

        JPT_ASSERT(m_frontIndex >= 0 && m_frontIndex != kInvalidValue<int64>, "Invalid front index");

        // Update index, wrap around if necessary
        --m_frontIndex;
        if (m_frontIndex < 0)
        {
            m_frontIndex = kCapacity - 1;
        }

        // Write data
        m_buffer[m_frontIndex] = data;
        ++m_count;
    }

    template<typename _TData, Index kCapacity, typename TAllocator>
    constexpr void Deque<_TData, kCapacity, TAllocator>::AddBack(const TData& data)
    {
        JPT_ASSERT(!IsFull(), "Deque is full");

        if (IsEmpty())
        {
            Init(data);
            return;
        }

        JPT_ASSERT(m_backIndex >= 0 && m_backIndex != kInvalidValue<int64>, "Invalid back index");

        // Update index, wrap around if necessary
        ++m_backIndex;
        if (m_backIndex == kCapacity)
        {
            m_backIndex = 0;
        }

        // Write data
        m_buffer[m_backIndex] = data;
        ++m_count;
    }

    template<typename _TData, Index kCapacity, typename TAllocator>
    constexpr void Deque<_TData, kCapacity, TAllocator>::PopFront()
    {
        JPT_ASSERT(!IsEmpty(), "Deque is empty");
        JPT_ASSERT(m_frontIndex >= 0 && m_frontIndex != kInvalidValue<int64>, "Invalid front index");

        // Destruct the front element if it's not trivially destructible
        if constexpr (!IsTriviallyDestructible<TData>)
        {
            TAllocator::Destruct(m_buffer + m_frontIndex);
        }

        // Update front index. Wrap around if necessary, or reset if the deque is empty
        --m_count;

        if (IsEmpty())
        {
            m_frontIndex = kInvalidValue<int64>;
            m_backIndex  = kInvalidValue<int64>;
        }
        else
        {
            ++m_frontIndex;
            if (m_frontIndex == kCapacity)
            {
                m_frontIndex = 0;
            }
        }
    }

    template<typename _TData, Index kCapacity, typename TAllocator>
    constexpr void Deque<_TData, kCapacity, TAllocator>::PopBack()
    {
        JPT_ASSERT(!IsEmpty(), "Deque is empty");
        JPT_ARGS_COUNT(m_backIndex >= 0 && m_backIndex != kInvalidValue<int64>, "Invalid back index");

        // Destruct the back element if it's not trivially destructible
        if constexpr (!IsTriviallyDestructible<TData>)
        {
            TAllocator::Destruct(m_buffer + m_backIndex);
        }

        // Update back index. Wrap around if necessary, or reset if the deque is empty
        --m_count;

        if (IsEmpty())
        {
            m_frontIndex = kInvalidValue<int64>;
            m_backIndex = kInvalidValue<int64>;
        }
        else
        {
            --m_backIndex;
            if (m_backIndex < 0)
            {
                m_backIndex = kCapacity - 1;
            }
        }
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr void Deque<TData, kCapacity, TAllocator>::Clear() noexcept
    {
        if (IsEmpty())
        {
            JPT_ASSERT(m_frontIndex == kInvalidValue<int64> && m_backIndex == kInvalidValue<int64>, "Invalid front/back index");
            return;
        }

        if constexpr (!IsTriviallyDestructible<TData>)
        {
            int64 index = m_frontIndex;
            while (true)
            {
                TAllocator::Destruct(m_buffer + index);
                if (index == m_backIndex)
                {
                    break;
                }
                index = GetNextIndex(index);
            }
        }

        m_frontIndex = kInvalidValue<int64>;
        m_backIndex = kInvalidValue<int64>;
        m_count = 0;
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr TData& Deque<TData, kCapacity, TAllocator>::Front() noexcept
    {
        JPT_ASSERT(!IsEmpty(), "Deque is empty");
        return m_buffer[m_frontIndex];
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr const TData& Deque<TData, kCapacity, TAllocator>::Front() const noexcept
    {
        JPT_ASSERT(!IsEmpty(), "Deque is empty");
        return m_buffer[m_frontIndex];
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr TData& Deque<TData, kCapacity, TAllocator>::Back() noexcept
    {
        JPT_ASSERT(!IsEmpty(), "Deque is empty");
        return m_buffer[m_backIndex];
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr const TData& Deque<TData, kCapacity, TAllocator>::Back() const noexcept
    {
        JPT_ASSERT(!IsEmpty(), "Deque is empty");
        return m_buffer[m_backIndex];
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr Deque<TData, kCapacity, TAllocator>::Iterator Deque<TData, kCapacity, TAllocator>::begin() noexcept
    {
        const int64 endIndex = GetNextIndex(m_backIndex);
        const bool endIsBegin = (m_frontIndex == endIndex);

        return Iterator(m_buffer, m_frontIndex, endIsBegin);
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr Deque<TData, kCapacity, TAllocator>::Iterator Deque<TData, kCapacity, TAllocator>::end() noexcept
    {
        const int64 endIndex = GetNextIndex(m_backIndex);
        const bool endIsBegin = (m_frontIndex == endIndex);

        return Iterator(m_buffer, endIndex, endIsBegin);
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr Deque<TData, kCapacity, TAllocator>::ConstIterator Deque<TData, kCapacity, TAllocator>::begin() const noexcept
    {
        const int64 endIndex = GetNextIndex(m_backIndex);
        const bool endIsBegin = (m_frontIndex == endIndex);

        return ConstIterator(m_buffer, m_frontIndex, endIsBegin);
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr Deque<TData, kCapacity, TAllocator>::ConstIterator Deque<TData, kCapacity, TAllocator>::cbegin() const noexcept
    {
        const int64 endIndex = GetNextIndex(m_backIndex);
        const bool endIsBegin = (m_frontIndex == endIndex);

        return ConstIterator(m_buffer, m_frontIndex, endIsBegin);
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr Deque<TData, kCapacity, TAllocator>::ConstIterator Deque<TData, kCapacity, TAllocator>::end() const noexcept
    {
        const int64 endIndex = GetNextIndex(m_backIndex);
        const bool endIsBegin = (m_frontIndex == endIndex);

        return ConstIterator(m_buffer, endIndex, endIsBegin);
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr Deque<TData, kCapacity, TAllocator>::ConstIterator Deque<TData, kCapacity, TAllocator>::cend() const noexcept
    {
        const int64 endIndex = GetNextIndex(m_backIndex);
        const bool endIsBegin = (m_frontIndex == endIndex);

        return ConstIterator(m_buffer, endIndex, endIsBegin);
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr Index Deque<TData, kCapacity, TAllocator>::Count() const noexcept
    {
        return m_count;
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr Index Deque<TData, kCapacity, TAllocator>::Capacity() const noexcept
    {
        return kCapacity;
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr bool Deque<TData, kCapacity, TAllocator>::IsEmpty() const noexcept
    {
        return m_count == 0;
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr bool Deque<TData, kCapacity, TAllocator>::IsFull() const noexcept
    {
        return m_count == kCapacity;
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr void Deque<TData, kCapacity, TAllocator>::CopyData(const Deque& other)
    {
        m_count = other.m_count;
        m_frontIndex = other.m_frontIndex;
        m_backIndex = other.m_backIndex;

        if (IsEmpty())
        {
            return;
        }

        int64 index = m_frontIndex;
        while (true)
        {
            m_buffer[index] = other.m_buffer[index];

            if (index == m_backIndex)
            {
                break;
            }

            index = GetNextIndex(index);
        }
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr void Deque<TData, kCapacity, TAllocator>::MoveData(Deque&& other) noexcept
    {
        m_count = other.m_count;
        m_frontIndex = other.m_frontIndex;
        m_backIndex = other.m_backIndex;

        // Reset other
        other.m_count = 0;
        other.m_frontIndex = kInvalidValue<int64>;
        other.m_backIndex = kInvalidValue<int64>;

        if (IsEmpty())
        {
            return;
        }

        int64 index = m_frontIndex;
        while (true)
        {
            m_buffer[index] = Move(other.m_buffer[index]);

            if (index == m_backIndex)
            {
                break;
            }

            index = GetNextIndex(index);
        }
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr void Deque<TData, kCapacity, TAllocator>::Init(const TData& data)
    {
        JPT_ASSERT(IsEmpty());

        const int64 startIndex = (kCapacity / 2) - 1;
        m_frontIndex = startIndex;
        m_backIndex  = startIndex;
        m_buffer[startIndex]  = data;

        m_count = 1;
    }

    template<typename TData, Index kCapacity, typename TAllocator>
    constexpr int64 Deque<TData, kCapacity, TAllocator>::GetNextIndex(int64 index) const noexcept
    {
        ++index;
        if (index == kCapacity)
        {
            index = 0;
        }

        return index;
    }
}