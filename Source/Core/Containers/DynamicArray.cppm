// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

#include <initializer_list>
#include <string>

export module jpt.DynamicArray;

import jpt.Allocator;
import jpt.Concepts;
import jpt.Constants;
import jpt.Serializer;
import jpt.TypeTraits;
import jpt.Utilities;
import jpt.Math;

export namespace jpt
{
    /** A sequence container that encapsulates dynamic size arrays. */
    template<typename _TData, typename TAllocator = Allocator<_TData>>
    class DynamicArray
    {
    public:
        using TData         = _TData;
        using Iterator      = TData*;
        using ConstIterator = const TData*;

    private:
        TData* m_pBuffer  = nullptr;
        size_t m_count    = 0;
        size_t m_capacity = 0;

    public:
        constexpr DynamicArray() = default;
        constexpr DynamicArray(const std::initializer_list<TData>& list);
        constexpr DynamicArray(const DynamicArray& other);
        constexpr DynamicArray(DynamicArray&& other) noexcept;
        DynamicArray& operator=(const DynamicArray& other) noexcept;
        DynamicArray& operator=(DynamicArray&& other) noexcept;
        constexpr DynamicArray(size_t count, const TData& data = TData());
        constexpr DynamicArray(Iterator begin, Iterator end);
        constexpr ~DynamicArray();

    public:
        // Adding
        constexpr void Add(size_t index, const TData& data);
        constexpr void Add(size_t index, TData&& data);
        constexpr void Add(const TData& data);
        constexpr void Add(TData&& data);
        template<typename ...TArgs>
        constexpr TData& Emplace(size_t index, TArgs&&... args);
        template<typename ...TArgs>
        constexpr TData& EmplaceBack(TArgs&&... args);
        constexpr DynamicArray& operator+=(const DynamicArray& other);

        // Erasing
        constexpr Iterator Erase(size_t index);
        constexpr Iterator Erase(Iterator iterator);
        constexpr void Pop();
        constexpr void Clear();

        // Accessing
        constexpr const TData* ConstBuffer() const noexcept;
        constexpr       TData* Buffer()            noexcept;
        constexpr       TData& Front()       noexcept;
        constexpr const TData& Front() const noexcept;
        constexpr       TData& Back()       noexcept;
        constexpr const TData& Back() const noexcept;
        constexpr       TData& operator[](size_t index)       noexcept;
        constexpr const TData& operator[](size_t index) const noexcept;
        constexpr       TData& At(size_t index)       noexcept;
        constexpr const TData& At(size_t index) const noexcept;

        // Iterators
        constexpr Iterator begin() noexcept;
        constexpr Iterator end()   noexcept;
        constexpr ConstIterator begin()  const noexcept;
        constexpr ConstIterator end()    const noexcept;
        constexpr ConstIterator cbegin() const noexcept;
        constexpr ConstIterator cend()   const noexcept;

        // Capacity
        constexpr size_t Count()    const noexcept;    // How many current elements are in the array
        constexpr size_t Size()     const noexcept;    // How many bytes this array is
        constexpr size_t Capacity() const noexcept;    // How many elements can be stored in the array
        constexpr bool   IsEmpty()  const noexcept;

        // Modifiers
        constexpr void Resize(size_t count, const TData& data = TData());
        constexpr void Reverse();
        constexpr void Reserve(size_t capacity);
        constexpr void ShrinkToFit();

        // Searching
        constexpr bool Has(const TData& data) const noexcept;

        // Serialization
        void Serialize(Serializer& serializer) const;
        void Deserialize(Serializer& serializer);

    private:
        /** Create a new data buffer with a new capacity, move the existing data over */
        constexpr void UpdateBuffer(size_t capacity);
        
        /** Copy other's data through it's iterators. TOtherContainer should be guaranteed to provide iterator interfaces */
        constexpr void CopyData(const TData* pBegin, size_t size);

        /** Moves the data from a deprecating dynamic array */
        constexpr void MoveData(DynamicArray&& other);

        /** Moves data towards a direction starting at given index for the distance provided */
        constexpr void ShiftDataToEnd(size_t index, size_t distance = 1);
        constexpr void ShiftDataToBegin(size_t index, size_t distance = 1);

        /** Called before inserting operation */
        constexpr void UpdateBufferForAdd(size_t index);
    };

    template<typename TData, typename TAllocator>
    constexpr bool operator==(const DynamicArray<TData, TAllocator>& a, const DynamicArray<TData, TAllocator>& b)
    {
        if (a.Count() != b.Count())
        {
            return false;
        }

        for (size_t i = 0; i < a.Count(); ++i)
        {
            if (a[i] != b[i])
            {
                return false;
            }
        }

        return true;
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>::DynamicArray(const std::initializer_list<TData>& list)
    {
        CopyData(list.begin(), list.size());
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>::DynamicArray(const DynamicArray& other)
    {
        CopyData(other.ConstBuffer(), other.Count());
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>::DynamicArray(DynamicArray&& other) noexcept
    {
        MoveData(Move(other));
    }

    template<typename TData, typename TAllocator>
    DynamicArray<TData, TAllocator>& DynamicArray<TData, TAllocator>::operator=(const DynamicArray& other) noexcept
    {
        if (this != &other)
        {
            Clear();
            CopyData(other.ConstBuffer(), other.Count());
        }

        return *this;
    }

    template<typename TData, typename TAllocator>
    DynamicArray<TData, TAllocator>& DynamicArray<TData, TAllocator>::operator=(DynamicArray&& other) noexcept
    {
        if (this != &other)
        {
            Clear();
            MoveData(Move(other));
        }

        return *this;
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>::DynamicArray(size_t count, const TData& data /* = TData()*/)
    {
        Resize(count, data);
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>::DynamicArray(Iterator begin, Iterator end)
    {
        CopyData(begin, end - begin);
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>::~DynamicArray()
    {
        Clear();
    }
    
    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::Add(size_t index, const TData& data)
    {
        UpdateBufferForAdd(index);
        TAllocator::Construct(m_pBuffer + index, data);
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::Add(size_t index, TData&& data)
    {
        UpdateBufferForAdd(index);
        TAllocator::Construct(m_pBuffer + index, Move(data));
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::Add(const TData& data)
    {
        Add(m_count, data);
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::Add(TData&& data)
    {
        Add(m_count, Move(data));
    }

    template<typename TData, typename TAllocator>
    template<typename ...TArgs>
    constexpr DynamicArray<TData, TAllocator>::TData& DynamicArray<TData, TAllocator>::Emplace(size_t index, TArgs&& ...args)
    {
        UpdateBufferForAdd(index);
        TAllocator::Construct(m_pBuffer + index, Forward<TArgs>(args)...);
        return m_pBuffer[index];
    }

    template<typename TData, typename TAllocator>
    template<typename ...TArgs>
    constexpr DynamicArray<TData, TAllocator>::TData& DynamicArray<TData, TAllocator>::EmplaceBack(TArgs&& ...args)
    {
        return Emplace(m_count, Forward<TArgs>(args)...);
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>& DynamicArray<TData, TAllocator>::operator+=(const DynamicArray& other)
    {
        Reserve(m_count + other.Count());
        for (size_t i = 0; i < other.Count(); ++i)
        {
            EmplaceBack(other[i]);
        }

        return *this;
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>::Iterator DynamicArray<TData, TAllocator>::Erase(size_t index)
    {
        JPT_ASSERT(index <= m_count, "Calling Erase() with an invalid index");

        if constexpr (!IsTriviallyDestructible<TData>)
        {
            TAllocator::Destruct(m_pBuffer + index);
        }

        --m_count;
        ShiftDataToBegin(index);

        return m_pBuffer + index;
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>::Iterator DynamicArray<TData, TAllocator>::Erase(Iterator iterator)
    {
        const size_t index = iterator - m_pBuffer;
        return Erase(index);
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::Pop()
    {
        if constexpr (!IsTriviallyDestructible<TData>)
        {
            TAllocator::Destruct(m_pBuffer + m_count - 1);
        }
        --m_count;
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::Clear()
    {
        if constexpr (!IsTriviallyDestructible<TData>)
        {
            for (size_t i = 0; i < m_count; ++i)
            {
                TAllocator::Destruct(m_pBuffer + i);
            }
        }

        JPT_SAFE_DELETE_ARRAY(m_pBuffer);
        m_count = 0;
        m_capacity = 0;
    }

    template<typename TData, typename TAllocator>
    constexpr const TData* DynamicArray<TData, TAllocator>::ConstBuffer() const noexcept
    {
        return m_pBuffer;
    }

    template<typename TData, typename TAllocator>
    constexpr TData* DynamicArray<TData, TAllocator>::Buffer() noexcept
    {
        return m_pBuffer;
    }

    template<typename TData, typename TAllocator>
    constexpr TData& DynamicArray<TData, TAllocator>::Front() noexcept
    {
        return m_pBuffer[0];
    }

    template<typename TData, typename TAllocator>
    constexpr const TData& DynamicArray<TData, TAllocator>::Front() const noexcept
    {
        return m_pBuffer[0];
    }

    template<typename TData, typename TAllocator>
    constexpr TData& DynamicArray<TData, TAllocator>::Back() noexcept
    {
        return m_pBuffer[m_count - 1];
    }

    template<typename TData, typename TAllocator>
    constexpr const TData& DynamicArray<TData, TAllocator>::Back() const noexcept
    {
        return m_pBuffer[m_count - 1];
    }

    template<typename TData, typename TAllocator>
    constexpr TData& DynamicArray<TData, TAllocator>::operator[](size_t index) noexcept
    {
        JPT_ASSERT(index < m_count); 
        return m_pBuffer[index];
    }

    template<typename TData, typename TAllocator>
    constexpr const TData& DynamicArray<TData, TAllocator>::operator[](size_t index) const noexcept
    {
        JPT_ASSERT(index < m_count); 
        return m_pBuffer[index];
    }

    template<typename TData, typename TAllocator>
    constexpr TData& DynamicArray<TData, TAllocator>::At(size_t index) noexcept
    {
        JPT_ASSERT(index < m_count); 
        return m_pBuffer[index];
    }

    template<typename TData, typename TAllocator>
    constexpr const TData& DynamicArray<TData, TAllocator>::At(size_t index) const noexcept
    {
        JPT_ASSERT(index < m_count); 
        return m_pBuffer[index];
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>::Iterator DynamicArray<TData, TAllocator>::begin() noexcept
    {
        return Iterator(m_pBuffer);
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>::Iterator DynamicArray<TData, TAllocator>::end() noexcept
    {
        return Iterator(m_pBuffer + m_count);
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>::ConstIterator DynamicArray<TData, TAllocator>::begin() const noexcept
    {
        return ConstIterator(m_pBuffer);
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>::ConstIterator DynamicArray<TData, TAllocator>::end() const noexcept
    {
        return ConstIterator(m_pBuffer + m_count);
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>::ConstIterator DynamicArray<TData, TAllocator>::cbegin() const noexcept
    {
        return ConstIterator(m_pBuffer);
    }

    template<typename TData, typename TAllocator>
    constexpr DynamicArray<TData, TAllocator>::ConstIterator DynamicArray<TData, TAllocator>::cend() const noexcept
    {
        return ConstIterator(m_pBuffer + m_count);
    }

    template<typename TData, typename TAllocator>
    constexpr size_t DynamicArray<TData, TAllocator>::Count() const noexcept
    {
        return m_count;
    }

    template<typename TData, typename TAllocator>
    constexpr size_t DynamicArray<TData, TAllocator>::Size() const noexcept
    {
        return m_count * sizeof(TData);
    }

    template<typename TData, typename TAllocator>
    constexpr size_t DynamicArray<TData, TAllocator>::Capacity() const noexcept
    {
        return m_capacity;
    }

    template<typename TData, typename TAllocator>
    constexpr bool DynamicArray<TData, TAllocator>::IsEmpty() const noexcept
    {
        return m_count == 0;
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::Resize(size_t count, const TData& data /*= TData()*/)
    {
        // If size if less than m_count, shrink
        if (count < m_count)
        {
            for (size_t i = count; i < m_count; ++i)
            {
                Pop();
            }
        }
        // If size if greater than m_count, grow
        else if (count > m_count)
        {
            Reserve(count);
            for (size_t i = m_count; i < count; ++i)
            {
                EmplaceBack(data);
            }
        }

        m_count = count;
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::Reverse()
    {
        for (size_t i = 0; i < Count() / 2; ++i)
        {
            Swap(m_pBuffer[i], m_pBuffer[Count() - i - 1]);
        }
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::Reserve(size_t capacity)
    {
        if (m_capacity < capacity)
        {
            UpdateBuffer(capacity);
        }
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::ShrinkToFit()
    {
        UpdateBuffer(m_count);
    }

    template<typename TData, typename TAllocator>
    constexpr bool DynamicArray<TData, TAllocator>::Has(const TData& data) const noexcept
    {
        for (size_t i = 0; i < m_count; ++i)
        {
            if (m_pBuffer[i] == data)
            {
                return true;
            }
        }

        return false;
    }

    template<typename TData, typename TAllocator>
    void DynamicArray<TData, TAllocator>::Serialize(Serializer& serializer) const
    {
        serializer.Write(m_count);
        serializer.Write(m_capacity);

        if constexpr (IsSerializeOverridden<TData>)
        {
            for (size_t i = 0; i < m_count; ++i)
            {
                serializer.Write(m_pBuffer[i]);
            }
        }
        else
        {
            serializer.Write(reinterpret_cast<const char*>(m_pBuffer), m_count * sizeof(TData));
        }
    }

    template<typename TData, typename TAllocator>
    void DynamicArray<TData, TAllocator>::Deserialize(Serializer& serializer)
    {
        Clear();

        size_t count = 0;
        size_t capacity = 0;
        serializer.Read(count);
        serializer.Read(capacity);

        Resize(count);

        if constexpr (IsSerializeOverridden<TData>)
        {
            for (size_t i = 0; i < count; ++i)
            {
                serializer.Read(m_pBuffer[i]);
            }
        }
        else
        {
            serializer.Read(reinterpret_cast<char*>(m_pBuffer), count * sizeof(TData));
        }

        m_count = count;
        m_capacity = capacity;
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::UpdateBuffer(size_t capacity)
    {
        static constexpr size_t kMinCapacity = 4;
        capacity = Max(capacity, kMinCapacity);

        TData* pNewBuffer = TAllocator::AllocateArray(capacity);

        if (m_pBuffer)
        {
            if constexpr (IsTriviallyMoveAssignable<TData> && IsTriviallyMoveConstructible<TData>)
            {
                MemMove(pNewBuffer, m_pBuffer, m_count * sizeof(TData));
            }
            else
            {
                for (size_t i = 0; i < m_count; ++i)
                {
                    pNewBuffer[i] = Move(m_pBuffer[i]);
                }
            }

            JPT_DELETE_ARRAY(m_pBuffer);
        }

        m_pBuffer  = pNewBuffer;
        m_capacity = capacity;
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::MoveData(DynamicArray&& other)
    {
        m_pBuffer  = other.m_pBuffer;
        m_capacity = other.m_capacity;
        m_count    = other.m_count;

        other.m_pBuffer  = nullptr;
        other.m_capacity = 0;
        other.m_count    = 0;
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::ShiftDataToEnd(size_t index, size_t distance /*= 1*/)
    {
        JPT_ASSERT(index <= m_count, "Distance went beyond the bound of this vector. Use reserve first");

        if constexpr (IsTriviallyMoveAssignable<TData> && IsTriviallyMoveConstructible<TData>)
        {
            MemMove(m_pBuffer + index + distance, m_pBuffer + index, (m_count - index) * sizeof(TData));
        }
        else
        {
            for (size_t i = m_count; i > index; --i)
            {
                m_pBuffer[i] = Move(m_pBuffer[i - distance]);
            }
        }
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::ShiftDataToBegin(size_t index, size_t distance)
    {
        JPT_ASSERT(index - distance >= 0, "Distance went beyond the start of this vector. Use smaller index or distance");

        if constexpr (IsTriviallyMoveAssignable<TData> && IsTriviallyMoveConstructible<TData>)
        {
            MemMove(m_pBuffer + index, m_pBuffer + index + distance, (m_count - index) * sizeof(TData));
        }
        else
        {
            for (size_t i = index; i < m_count; ++i)
            {
                m_pBuffer[i] = Move(m_pBuffer[i + distance]);
            }
        }
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::UpdateBufferForAdd(size_t index)
    {
        static constexpr size_t kGrowMultiplier = 2;

        JPT_ASSERT(index <= m_count, "Calling DynamicArray::Insert() with an invalid index");

        if (m_count >= m_capacity)
        {
            UpdateBuffer(m_count * kGrowMultiplier);
        }

        ShiftDataToEnd(index);
        ++m_count;
    }

    template<typename TData, typename TAllocator>
    constexpr void DynamicArray<TData, TAllocator>::CopyData(const TData* pBegin, size_t size)
    {
        m_count = size;
        UpdateBuffer(m_count);

        if constexpr (IsTriviallyCopyable<TData>)
        {
            MemCpy(m_pBuffer, pBegin, m_count * sizeof(TData));
        }
        else
        {
            for (size_t i = 0; i < size; ++i, ++pBegin)
            {
                m_pBuffer[i] = *pBegin;
            }
        }
    }
}