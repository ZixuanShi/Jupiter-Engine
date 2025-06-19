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
    template<typename _TData, size_t kCapacity>
    class StaticArray
    {
    public:
        using TData         = _TData;
        using Iterator      = TData*;
        using ConstIterator = const TData*;

    private:
        TData m_buffer[kCapacity];

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

    //----------------------------------------------------------------------------------------------
    // Non-member functions
    //----------------------------------------------------------------------------------------------
    template<typename TData, size_t kCapacity>
    constexpr bool operator==(const StaticArray<TData, kCapacity>& a, const StaticArray<TData, kCapacity>& b) noexcept
    {
        for (size_t i = 0; i < kCapacity; ++i)
        {
            if (a[i] != b[i])
            {
                return false;
            }
        }

        return true;
    }

    //----------------------------------------------------------------------------------------------
    // Member function definitions
    //----------------------------------------------------------------------------------------------
    template<typename TData, size_t kCapacity>
    constexpr StaticArray<TData, kCapacity>::StaticArray(const std::initializer_list<TData>& initializerList)
    {
        JPT_ASSERT(initializerList.size() == kCapacity, "Initializer list size doesn't match");
        CopyData(initializerList.begin());
    }

    template<typename TData, size_t kCapacity>
    constexpr StaticArray<TData, kCapacity>::StaticArray(const TData& data)
    {
        Fill(data);
    }

    template<typename TData, size_t kCapacity>
    constexpr StaticArray<TData, kCapacity>::StaticArray(const StaticArray& other)
    {
        CopyData(other.ConstBuffer());
    }

    template<typename TData, size_t kCapacity>
    constexpr StaticArray<TData, kCapacity>::StaticArray(StaticArray&& other) noexcept
    {
        MoveData(Move(other));
    }

    template<typename TData, size_t kCapacity>
    StaticArray<TData, kCapacity>& StaticArray<TData, kCapacity>::operator=(const StaticArray& other) noexcept
    {
        if (this != &other)
        {
            CopyData(other.ConstBuffer());
        }

        return *this;
    }

    template<typename TData, size_t kCapacity>
    StaticArray<TData, kCapacity>& StaticArray<TData, kCapacity>::operator=(StaticArray&& other) noexcept
    {
        if (this != &other)
        {
            MoveData(Move(other));
        }

        return *this;
    }

    template<typename TData, size_t kCapacity>
    constexpr StaticArray<TData, kCapacity>::~StaticArray()
    {
        if constexpr (!IsTriviallyDestructible<TData>)
        {
            for (size_t i = 0; i < kCapacity; ++i)
            {
                m_buffer[i].~TData();
            }
        }
    }

    template<typename TData, size_t kCapacity>
    constexpr const TData* StaticArray<TData, kCapacity>::ConstBuffer() const noexcept
    {
        return m_buffer;
    }

    template<typename TData, size_t kCapacity>
    constexpr TData* StaticArray<TData, kCapacity>::Buffer() noexcept
    {
        return m_buffer;
    }

    template<typename TData, size_t kCapacity>
    constexpr TData& StaticArray<TData, kCapacity>::Front() noexcept
    {
        return m_buffer[0];
    }

    template<typename TData, size_t kCapacity>
    constexpr const TData& StaticArray<TData, kCapacity>::Front() const noexcept
    {
        return m_buffer[0];
    }

    template<typename TData, size_t kCapacity>
    constexpr TData& StaticArray<TData, kCapacity>::Back() noexcept
    {
        return m_buffer[kCapacity - 1];
    }

    template<typename TData, size_t kCapacity>
    constexpr const TData& StaticArray<TData, kCapacity>::Back() const noexcept
    {
        return m_buffer[kCapacity - 1];
    }

    template<typename TData, size_t kCapacity>
    constexpr TData& StaticArray<TData, kCapacity>::operator[](size_t index) noexcept
    {
        JPT_ASSERT(index < kCapacity, "Index out of bounds");
        return m_buffer[index];
    }

    template<typename TData, size_t kCapacity>
    constexpr const TData& StaticArray<TData, kCapacity>::operator[](size_t index) const noexcept
    {
        JPT_ASSERT(index < kCapacity, "Index out of bounds");
        return m_buffer[index];
    }

    template<typename TData, size_t kCapacity>
    constexpr StaticArray<TData, kCapacity>::Iterator StaticArray<TData, kCapacity>::begin() noexcept
    {
        return Iterator(m_buffer);
    }

    template<typename TData, size_t kCapacity>
    constexpr StaticArray<TData, kCapacity>::Iterator StaticArray<TData, kCapacity>::end() noexcept
    {
        return Iterator(m_buffer + kCapacity);
    }

    template<typename TData, size_t kCapacity>
    constexpr StaticArray<TData, kCapacity>::ConstIterator StaticArray<TData, kCapacity>::begin() const noexcept
    {
        return ConstIterator(m_buffer);
    }

    template<typename TData, size_t kCapacity>
    constexpr StaticArray<TData, kCapacity>::ConstIterator StaticArray<TData, kCapacity>::cbegin() const noexcept
    {
        return ConstIterator(m_buffer);
    }

    template<typename TData, size_t kCapacity>
    constexpr StaticArray<TData, kCapacity>::ConstIterator StaticArray<TData, kCapacity>::end() const noexcept
    {
        return ConstIterator(m_buffer + kCapacity);
    }

    template<typename TData, size_t kCapacity>
    constexpr StaticArray<TData, kCapacity>::ConstIterator StaticArray<TData, kCapacity>::cend() const noexcept
    {
        return ConstIterator(m_buffer + kCapacity);
    }

    template<typename TData, size_t kCapacity>
    constexpr size_t StaticArray<TData, kCapacity>::Count() const  noexcept
    {
        return kCapacity;
    }

    template<typename TData, size_t kCapacity>
    constexpr size_t StaticArray<TData, kCapacity>::Size() const  noexcept
    {
        return kCapacity * sizeof(TData);
    }

    template<typename TData, size_t kCapacity>
    constexpr void StaticArray<TData, kCapacity>::Fill(const TData& value)
    {
        for (size_t i = 0; i < kCapacity; ++i)
        {
            m_buffer[i] = value;
        }
    }

    template<typename TData, size_t kCapacity>
    constexpr void StaticArray<TData, kCapacity>::CopyData(const TData* pBegin)
    {
        if constexpr (IsTriviallyCopyable<TData>)
        {
            MemCpy(m_buffer, pBegin, kCapacity * sizeof(TData));
        }
        else
        {
            for (size_t i = 0; i < kCapacity; ++i, ++pBegin)
            {
                m_buffer[i] = *pBegin;
            }
        }
    }

    template<typename TData, size_t kCapacity>
    constexpr void StaticArray<TData, kCapacity>::MoveData(StaticArray&& other)
    {
        for (size_t i = 0; i < kCapacity; ++i)
        {
            m_buffer[i] = Move(other.m_buffer[i]);
        }
    }
}