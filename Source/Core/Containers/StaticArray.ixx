// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

#include <string>
#include <initializer_list>

export module jpt.StaticArray;

import jpt.Utilities;

export namespace jpt
{
    /** A sequence container that encapsulates fixed size arrays */
    template<typename _TData, size_t kCount>
    class StaticArray
    {
    public:
        using TData         = _TData;
        using Iterator      = TData*;
        using ConstIterator = const TData*;

    private:
        TData m_buffer[kCount];

    public:
        constexpr StaticArray() = default;
        constexpr StaticArray(const std::initializer_list<TData>& initializerList);
        constexpr StaticArray(const TData& data);
        constexpr StaticArray(const StaticArray& other);
        constexpr StaticArray(StaticArray&& other) noexcept;
        StaticArray& operator=(const StaticArray& other) noexcept;
        StaticArray& operator=(StaticArray&& other) noexcept;
        constexpr ~StaticArray();

    public:
        // Accessing
        constexpr const TData* ConstBuffer() const noexcept;
        constexpr       TData* Buffer() noexcept;
        constexpr       TData& Front()  noexcept;
        constexpr const TData& Front() const noexcept;
        constexpr       TData& Back()  noexcept;
        constexpr const TData& Back()  const noexcept;
        constexpr       TData& operator[](size_t index) noexcept;
        constexpr const TData& operator[](size_t index) const noexcept;

        // Iterators
        constexpr Iterator begin() noexcept;
        constexpr Iterator end()   noexcept;
        constexpr ConstIterator begin()  const noexcept;
        constexpr ConstIterator cbegin() const noexcept;
        constexpr ConstIterator end()    const noexcept;
        constexpr ConstIterator cend()   const noexcept;

        // Capacity
        constexpr size_t Count() const noexcept;
        constexpr size_t Size()  const noexcept;

        // Modifiers
        constexpr void Fill(const TData& value);

    private:
        constexpr void CopyData(const TData* pBegin);
        constexpr void MoveData(StaticArray&& other);
    };

    template<typename TData, size_t kCount>
    constexpr bool operator==(const StaticArray<TData, kCount>& a, const StaticArray<TData, kCount>& b) noexcept
    {
        for (size_t i = 0; i < kCount; ++i)
        {
            if (a[i] != b[i])
            {
                return false;
            }
        }

        return true;
    }

    template<typename TData, size_t kCount>
    constexpr StaticArray<TData, kCount>::StaticArray(const std::initializer_list<TData>& initializerList)
    {
        JPT_ASSERT(initializerList.size() == kCount, "Initializer list size doesn't match");
        CopyData(initializerList.begin());
    }

    template<typename TData, size_t kCount>
    constexpr StaticArray<TData, kCount>::StaticArray(const TData& data)
    {
        Fill(data);
    }

    template<typename TData, size_t kCount>
    constexpr StaticArray<TData, kCount>::StaticArray(const StaticArray& other)
    {
        CopyData(other.ConstBuffer());
    }

    template<typename TData, size_t kCount>
    constexpr StaticArray<TData, kCount>::StaticArray(StaticArray&& other) noexcept
    {
        MoveData(Move(other));
    }

    template<typename TData, size_t kCount>
    StaticArray<TData, kCount>& StaticArray<TData, kCount>::operator=(const StaticArray& other) noexcept
    {
        if (this != &other)
        {
            CopyData(other.ConstBuffer());
        }

        return *this;
    }

    template<typename TData, size_t kCount>
    StaticArray<TData, kCount>& StaticArray<TData, kCount>::operator=(StaticArray&& other) noexcept
    {
        if (this != &other)
        {
            MoveData(Move(other));
        }

        return *this;
    }

    template<typename TData, size_t kCount>
    constexpr StaticArray<TData, kCount>::~StaticArray()
    {
        if constexpr (!IsTriviallyDestructible<TData>)
        {
            for (size_t i = 0; i < kCount; ++i)
            {
                m_buffer[i].~TData();
            }
        }
    }

    template<typename TData, size_t kCount>
    constexpr const TData* StaticArray<TData, kCount>::ConstBuffer() const noexcept
    {
        return m_buffer;
    }

    template<typename TData, size_t kCount>
    constexpr TData* StaticArray<TData, kCount>::Buffer() noexcept
    {
        return m_buffer;
    }

    template<typename TData, size_t kCount>
    constexpr TData& StaticArray<TData, kCount>::Front() noexcept
    {
        return m_buffer[0];
    }

    template<typename TData, size_t kCount>
    constexpr const TData& StaticArray<TData, kCount>::Front() const noexcept
    {
        return m_buffer[0];
    }

    template<typename TData, size_t kCount>
    constexpr TData& StaticArray<TData, kCount>::Back() noexcept
    {
        return m_buffer[kCount - 1];
    }

    template<typename TData, size_t kCount>
    constexpr const TData& StaticArray<TData, kCount>::Back() const noexcept
    {
        return m_buffer[kCount - 1];
    }

    template<typename TData, size_t kCount>
    constexpr TData& StaticArray<TData, kCount>::operator[](size_t index) noexcept
    {
        JPT_ASSERT(index < kCount, "Index out of bounds");
        return m_buffer[index];
    }

    template<typename TData, size_t kCount>
    constexpr const TData& StaticArray<TData, kCount>::operator[](size_t index) const noexcept
    {
        JPT_ASSERT(index < kCount, "Index out of bounds");
        return m_buffer[index];
    }

    template<typename TData, size_t kCount>
    constexpr StaticArray<TData, kCount>::Iterator StaticArray<TData, kCount>::begin() noexcept
    {
        return Iterator(m_buffer);
    }

    template<typename TData, size_t kCount>
    constexpr StaticArray<TData, kCount>::Iterator StaticArray<TData, kCount>::end() noexcept
    {
        return Iterator(m_buffer + kCount);
    }

    template<typename TData, size_t kCount>
    constexpr StaticArray<TData, kCount>::ConstIterator StaticArray<TData, kCount>::begin() const noexcept
    {
        return ConstIterator(m_buffer);
    }

    template<typename TData, size_t kCount>
    constexpr StaticArray<TData, kCount>::ConstIterator StaticArray<TData, kCount>::cbegin() const noexcept
    {
        return ConstIterator(m_buffer);
    }

    template<typename TData, size_t kCount>
    constexpr StaticArray<TData, kCount>::ConstIterator StaticArray<TData, kCount>::end() const noexcept
    {
        return ConstIterator(m_buffer + kCount);
    }

    template<typename TData, size_t kCount>
    constexpr StaticArray<TData, kCount>::ConstIterator StaticArray<TData, kCount>::cend() const noexcept
    {
        return ConstIterator(m_buffer + kCount);
    }

    template<typename TData, size_t kCount>
    constexpr size_t StaticArray<TData, kCount>::Count() const  noexcept
    {
        return kCount;
    }

    template<typename TData, size_t kCount>
    constexpr size_t StaticArray<TData, kCount>::Size() const  noexcept
    {
        return kCount * sizeof(TData);
    }

    template<typename TData, size_t kCount>
    constexpr void StaticArray<TData, kCount>::Fill(const TData& value)
    {
        for (size_t i = 0; i < kCount; ++i)
        {
            m_buffer[i] = value;
        }
    }

    template<typename TData, size_t kCount>
    constexpr void StaticArray<TData, kCount>::CopyData(const TData* pBegin)
    {
        if constexpr (IsTriviallyCopyable<TData>)
        {
            MemCpy(m_buffer, pBegin, kCount * sizeof(TData));
        }
        else
        {
            for (size_t i = 0; i < kCount; ++i, ++pBegin)
            {
                m_buffer[i] = *pBegin;
            }
        }
    }

    template<typename TData, size_t kCount>
    constexpr void StaticArray<TData, kCount>::MoveData(StaticArray&& other)
    {
        for (size_t i = 0; i < kCount; ++i)
        {
            m_buffer[i] = Move(other.m_buffer[i]);
        }
    }
}