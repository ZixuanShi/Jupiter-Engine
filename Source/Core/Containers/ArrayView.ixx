// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <initializer_list>

export module jpt.ArrayView;

import jpt.Concepts;
import jpt.StaticArray;
import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.TypeTraits;

export namespace jpt
{
    /** Non-owning view to arrays. Think of this StringView but for arrays. std::span */
    template<typename _TData>
    class ArrayView
    {
    public:
        using TData = _TData;
        using Iterator = TData*;
        using ConstIterator = const TData*;

    private:
        const TData* m_pBuffer = nullptr;
        Index m_count = 0;

    public:
        constexpr ArrayView() noexcept = default;
        constexpr ArrayView(const std::initializer_list<TData>& initializerList);
        constexpr ArrayView(const DynamicArray<TData>& arr) noexcept;
        template<Index N> constexpr ArrayView(const StaticArray<TData, N>& arr) noexcept;
        template<Index N> constexpr ArrayView(TData (&arr)[N]) noexcept;
        ArrayView& operator=(const std::initializer_list<TData>& initializerList) noexcept;
        ArrayView& operator=(const DynamicArray<TData>& arr) noexcept;
        template<Index N> ArrayView& operator=(const StaticArray<TData, N>& arr) noexcept;
        template<Index N> ArrayView& operator=(TData(&arr)[N]) noexcept;

    public:
        // Iterators
        constexpr Iterator begin() noexcept;
        constexpr Iterator end()   noexcept;
        constexpr ConstIterator begin()  const noexcept;
        constexpr ConstIterator end()    const noexcept;
        constexpr ConstIterator cbegin() const noexcept;
        constexpr ConstIterator cend()   const noexcept;

        // Accessing
        constexpr       TData& operator[](Index index)       noexcept;
        constexpr const TData& operator[](Index index) const noexcept;

        // Capacity
        constexpr Index Count() const noexcept;
        constexpr bool IsEmpty() const noexcept;
    };

    template<typename TData>
    constexpr ArrayView<TData>::ArrayView(const std::initializer_list<TData>& initializerList)
        : m_pBuffer(initializerList.begin())
        , m_count(static_cast<Index>(initializerList.size()))
    {
    }

    template<typename _TData>
    constexpr ArrayView<_TData>::ArrayView(const DynamicArray<TData>& arr) noexcept
        : m_pBuffer(arr.ConstBuffer())
        , m_count(arr.Count())
    {
    }

    template<typename TData>
    template<Index N>
    constexpr ArrayView<TData>::ArrayView(const StaticArray<TData, N>& arr) noexcept
        : m_pBuffer(arr.ConstBuffer())
        , m_count(N)
    {
    }

    template<typename TData>
    template<Index N>
    constexpr ArrayView<TData>::ArrayView(TData(&arr)[N]) noexcept
        : m_pBuffer(arr)
        , m_count(N)
    {
    }

    template<typename TData>
    ArrayView<TData>& ArrayView<TData>::operator=(const std::initializer_list<TData>& initializerList) noexcept
    {
        if (m_pBuffer != initializerList.begin())
        {
            m_pBuffer = initializerList.begin();
            m_count = static_cast<Index>(initializerList.size());
        }

        return *this;
    }

    template<typename TData>
    ArrayView<TData>& ArrayView<TData>::operator=(const DynamicArray<TData>& arr) noexcept
    {
        if (m_pBuffer != arr.ConstBuffer())
        {
            m_pBuffer = arr.ConstBuffer();
            m_count = arr.Count();
        }

        return *this;
    }

    template<typename TData>
    template<Index N>
    ArrayView<TData>& ArrayView<TData>::operator=(const StaticArray<TData, N>& arr) noexcept
    {
        if (m_pBuffer != arr.ConstBuffer())
        {
            m_pBuffer = arr.ConstBuffer();
            m_count = arr.Count();
        }

        return *this;
    }

    template<typename TData>
    template<Index N>
    ArrayView<TData>& ArrayView<TData>::operator=(TData(&arr)[N]) noexcept
    {
        if (m_pBuffer != arr)
        {
            m_pBuffer = arr;
            m_count = N;
        }

        return *this;
    }

    template<typename TData>
    constexpr ArrayView<TData>::Iterator ArrayView<TData>::begin() noexcept
    {
        return Iterator(m_pBuffer);
    }

    template<typename TData>
    constexpr ArrayView<TData>::Iterator ArrayView<TData>::end() noexcept
    {
        return Iterator(m_pBuffer + m_count);
    }

    template<typename TData>
    constexpr ArrayView<TData>::ConstIterator ArrayView<TData>::begin() const noexcept
    {
        return ConstIterator(m_pBuffer);
    }

    template<typename TData>
    constexpr ArrayView<TData>::ConstIterator ArrayView<TData>::end() const noexcept
    {
        return ConstIterator(m_pBuffer + m_count);
    }

    template<typename TData>
    constexpr ArrayView<TData>::ConstIterator ArrayView<TData>::cbegin() const noexcept
    {
        return ConstIterator(m_pBuffer);
    }

    template<typename TData>
    constexpr ArrayView<TData>::ConstIterator ArrayView<TData>::cend() const noexcept
    {
        return ConstIterator(m_pBuffer + m_count);
    }

    template<typename TData>
    constexpr TData& ArrayView<TData>::operator[](Index index) noexcept
    {
        JPT_ASSERT(index < m_count);
        return m_pBuffer[index];
    }

    template<typename TData>
    constexpr const TData& ArrayView<TData>::operator[](Index index) const noexcept
    {
        JPT_ASSERT(index < m_count);
        return m_pBuffer[index];
    }

    template<typename TData>
    constexpr size_t ArrayView<TData>::Count() const noexcept
    {
        return m_count;
    }

    template<typename TData>
    constexpr bool ArrayView<TData>::IsEmpty() const noexcept
    {
        return m_count == 0;
    }
}