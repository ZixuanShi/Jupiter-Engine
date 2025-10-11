// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.Iterator_CircularArray;

import jpt.TypeDefs;
import jpt.TypeTraits;

export namespace jpt_private
{
    template<typename TData, Index kCapacity>
    class Iterator_CircularArray
    {
    private:
        TData* m_pBuffer = nullptr;
        int64 m_index = 0;

        bool m_shouldWrap = false;
        
    public:
        constexpr Iterator_CircularArray() = default;
        constexpr Iterator_CircularArray(TData* pBuffer, int64 index, bool endIsBegin);

        constexpr Iterator_CircularArray& operator++();
        constexpr Iterator_CircularArray operator++(int32);

        constexpr Iterator_CircularArray& operator+=(size_t offset);
        constexpr Iterator_CircularArray operator+(size_t offset);

        constexpr Iterator_CircularArray& operator--();
        constexpr Iterator_CircularArray operator--(int32);

        constexpr Iterator_CircularArray& operator-=(size_t offset);
        constexpr Iterator_CircularArray operator-(size_t offset);

        constexpr       TData& operator*()        noexcept { return  m_pBuffer[m_index]; }
        constexpr const TData& operator*()  const noexcept { return  m_pBuffer[m_index]; }
        constexpr       TData* operator->()       noexcept { return &m_pBuffer[m_index]; }
        constexpr const TData* operator->() const noexcept { return &m_pBuffer[m_index]; }

        constexpr bool operator==(const Iterator_CircularArray& other) const noexcept;
    };

    template<typename TData, Index kCapacity>
    constexpr Iterator_CircularArray<TData, kCapacity>::Iterator_CircularArray(TData* pBuffer, int64 index, bool endIsBegin)
        : m_pBuffer(pBuffer)
        , m_index(index)
        , m_shouldWrap(endIsBegin)
    {
    }

    template<typename TData, Index kCapacity>
    constexpr Iterator_CircularArray<TData, kCapacity>& Iterator_CircularArray<TData, kCapacity>::operator++()
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
    constexpr Iterator_CircularArray<TData, kCapacity> Iterator_CircularArray<TData, kCapacity>::operator++(int32)
    {
        Iterator_CircularArray iterator = *this;
        ++(*this);
        return iterator;
    }

    template<typename TData, Index kCapacity>
    constexpr Iterator_CircularArray<TData, kCapacity>& Iterator_CircularArray<TData, kCapacity>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++(*this);
        }
        return *this;
    }

    template<typename TData, Index kCapacity>
    constexpr Iterator_CircularArray<TData, kCapacity> Iterator_CircularArray<TData, kCapacity>::operator+(size_t offset)
    {
        Iterator_CircularArray iterator = *this;
        return iterator += offset;
    }

    template<typename TData, Index kCapacity>
    constexpr Iterator_CircularArray<TData, kCapacity>& Iterator_CircularArray<TData, kCapacity>::operator--()
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
    constexpr Iterator_CircularArray<TData, kCapacity> Iterator_CircularArray<TData, kCapacity>::operator--(int32)
    {
        Iterator_CircularArray iterator = *this;
        --(*this);
        return iterator;
    }

    template<typename TData, Index kCapacity>
    constexpr Iterator_CircularArray<TData, kCapacity>& Iterator_CircularArray<TData, kCapacity>::operator-=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            --(*this);
        }
        return *this;
    }

    template<typename TData, Index kCapacity>
    constexpr Iterator_CircularArray<TData, kCapacity> Iterator_CircularArray<TData, kCapacity>::operator-(size_t offset)
    {
        Iterator_CircularArray iterator = *this;
        return iterator -= offset;
    }

    template<typename TData, Index kCapacity>
    constexpr bool Iterator_CircularArray<TData, kCapacity>::operator==(const Iterator_CircularArray& other) const noexcept
    {
        return !m_shouldWrap &&
               m_pBuffer == other.m_pBuffer &&
               m_index   == other.m_index;
    }

    template<typename TData, Index kCapacity>
    class ConstIterator_CircularArray
    {
    private:
        const TData* m_pBuffer = nullptr;
        int64 m_index = 0;
        bool m_shouldWrap = false;

    public:
        constexpr ConstIterator_CircularArray() = default;
        constexpr ConstIterator_CircularArray(const TData* pBuffer, int64 index, bool endIsBegin);

        constexpr ConstIterator_CircularArray& operator++();
        constexpr ConstIterator_CircularArray operator++(int32);

        constexpr ConstIterator_CircularArray& operator+=(size_t offset);
        constexpr ConstIterator_CircularArray operator+(size_t offset);

        constexpr ConstIterator_CircularArray& operator--();
        constexpr ConstIterator_CircularArray operator--(int32);

        constexpr ConstIterator_CircularArray& operator-=(size_t offset);
        constexpr ConstIterator_CircularArray operator-(size_t offset);

        constexpr const TData& operator*()  const noexcept { return  m_pBuffer[m_index]; }
        constexpr const TData* operator->() const noexcept { return &m_pBuffer[m_index]; }

        constexpr bool operator==(const ConstIterator_CircularArray& other) const noexcept;
    };

    template<typename TData, Index kCapacity>
    constexpr ConstIterator_CircularArray<TData, kCapacity>::ConstIterator_CircularArray(const TData* pBuffer, int64 index, bool endIsBegin)
        : m_pBuffer(pBuffer)
        , m_index(index)
        , m_shouldWrap(endIsBegin)
    {
    }

    template<typename TData, Index kCapacity>
    constexpr ConstIterator_CircularArray<TData, kCapacity>& ConstIterator_CircularArray<TData, kCapacity>::operator++()
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
    constexpr ConstIterator_CircularArray<TData, kCapacity> ConstIterator_CircularArray<TData, kCapacity>::operator++(int32)
    {
        ConstIterator_CircularArray iterator = *this;
        ++(*this);
        return iterator;
    }

    template<typename TData, Index kCapacity>
    constexpr ConstIterator_CircularArray<TData, kCapacity>& ConstIterator_CircularArray<TData, kCapacity>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++(*this);
        }
        return *this;
    }

    template<typename TData, Index kCapacity>
    constexpr ConstIterator_CircularArray<TData, kCapacity> ConstIterator_CircularArray<TData, kCapacity>::operator+(size_t offset)
    {
        ConstIterator_CircularArray iterator = *this;
        return iterator += offset;
    }

    template<typename TData, Index kCapacity>
    constexpr ConstIterator_CircularArray<TData, kCapacity>& ConstIterator_CircularArray<TData, kCapacity>::operator--()
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
    constexpr ConstIterator_CircularArray<TData, kCapacity> ConstIterator_CircularArray<TData, kCapacity>::operator--(int32)
    {
        ConstIterator_CircularArray iterator = *this;
        --(*this);
        return iterator;
    }

    template<typename TData, Index kCapacity>
    constexpr ConstIterator_CircularArray<TData, kCapacity>& ConstIterator_CircularArray<TData, kCapacity>::operator-=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            --(*this);
        }
        return *this;
    }

    template<typename TData, Index kCapacity>
    constexpr ConstIterator_CircularArray<TData, kCapacity> ConstIterator_CircularArray<TData, kCapacity>::operator-(size_t offset)
    {
        ConstIterator_CircularArray iterator = *this;
        return iterator -= offset;
    }

    template<typename TData, Index kCapacity>
    constexpr bool ConstIterator_CircularArray<TData, kCapacity>::operator==(const ConstIterator_CircularArray& other) const noexcept
    {
        return !m_shouldWrap &&
               m_pBuffer == other.m_pBuffer &&
               m_index   == other.m_index;
    }
}