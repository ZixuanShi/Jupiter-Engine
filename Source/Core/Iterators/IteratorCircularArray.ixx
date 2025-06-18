// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.IteratorCircularArray;

import jpt.TypeDefs;
import jpt.TypeTraits;

export namespace jpt_private
{
    template<typename TData, Index kCapacity>
    class IteratorCircularArray
    {
    private:
        TData* m_pBuffer = nullptr;
        int64 m_index = 0;

        bool m_shouldWrap = false;
        
    public:
        constexpr IteratorCircularArray() = default;
        constexpr IteratorCircularArray(TData* pBuffer, int64 index, bool endIsBegin);

        constexpr IteratorCircularArray& operator++();
        constexpr IteratorCircularArray operator++(int32);

        constexpr IteratorCircularArray& operator+=(size_t offset);
        constexpr IteratorCircularArray operator+(size_t offset);

        constexpr IteratorCircularArray& operator--();
        constexpr IteratorCircularArray operator--(int32);

        constexpr IteratorCircularArray& operator-=(size_t offset);
        constexpr IteratorCircularArray operator-(size_t offset);

        constexpr       TData& operator*()        noexcept { return  m_pBuffer[m_index]; }
        constexpr const TData& operator*()  const noexcept { return  m_pBuffer[m_index]; }
        constexpr       TData* operator->()       noexcept { return &m_pBuffer[m_index]; }
        constexpr const TData* operator->() const noexcept { return &m_pBuffer[m_index]; }

        constexpr bool operator==(const IteratorCircularArray& other) const noexcept;
    };

    template<typename TData, Index kCapacity>
    constexpr IteratorCircularArray<TData, kCapacity>::IteratorCircularArray(TData* pBuffer, int64 index, bool endIsBegin)
        : m_pBuffer(pBuffer)
        , m_index(index)
        , m_shouldWrap(endIsBegin)
    {
    }

    template<typename TData, Index kCapacity>
    constexpr IteratorCircularArray<TData, kCapacity>& IteratorCircularArray<TData, kCapacity>::operator++()
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
    constexpr IteratorCircularArray<TData, kCapacity> IteratorCircularArray<TData, kCapacity>::operator++(int32)
    {
        IteratorCircularArray iterator = *this;
        ++(*this);
        return iterator;
    }

    template<typename TData, Index kCapacity>
    constexpr IteratorCircularArray<TData, kCapacity>& IteratorCircularArray<TData, kCapacity>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++(*this);
        }
        return *this;
    }

    template<typename TData, Index kCapacity>
    constexpr IteratorCircularArray<TData, kCapacity> IteratorCircularArray<TData, kCapacity>::operator+(size_t offset)
    {
        IteratorCircularArray iterator = *this;
        return iterator += offset;
    }

    template<typename TData, Index kCapacity>
    constexpr IteratorCircularArray<TData, kCapacity>& IteratorCircularArray<TData, kCapacity>::operator--()
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
    constexpr IteratorCircularArray<TData, kCapacity> IteratorCircularArray<TData, kCapacity>::operator--(int32)
    {
        IteratorCircularArray iterator = *this;
        --(*this);
        return iterator;
    }

    template<typename TData, Index kCapacity>
    constexpr IteratorCircularArray<TData, kCapacity>& IteratorCircularArray<TData, kCapacity>::operator-=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            --(*this);
        }
        return *this;
    }

    template<typename TData, Index kCapacity>
    constexpr IteratorCircularArray<TData, kCapacity> IteratorCircularArray<TData, kCapacity>::operator-(size_t offset)
    {
        IteratorCircularArray iterator = *this;
        return iterator -= offset;
    }

    template<typename TData, Index kCapacity>
    constexpr bool IteratorCircularArray<TData, kCapacity>::operator==(const IteratorCircularArray& other) const noexcept
    {
        return !m_shouldWrap &&
               m_pBuffer == other.m_pBuffer &&
               m_index   == other.m_index;
    }

    template<typename TData, Index kCapacity>
    class ConstIteratorCircularArray
    {
    private:
        const TData* m_pBuffer = nullptr;
        int64 m_index = 0;
        bool m_shouldWrap = false;

    public:
        constexpr ConstIteratorCircularArray() = default;
        constexpr ConstIteratorCircularArray(const TData* pBuffer, int64 index, bool endIsBegin);

        constexpr ConstIteratorCircularArray& operator++();
        constexpr ConstIteratorCircularArray operator++(int32);

        constexpr ConstIteratorCircularArray& operator+=(size_t offset);
        constexpr ConstIteratorCircularArray operator+(size_t offset);

        constexpr ConstIteratorCircularArray& operator--();
        constexpr ConstIteratorCircularArray operator--(int32);

        constexpr ConstIteratorCircularArray& operator-=(size_t offset);
        constexpr ConstIteratorCircularArray operator-(size_t offset);

        constexpr const TData& operator*()  const noexcept { return  m_pBuffer[m_index]; }
        constexpr const TData* operator->() const noexcept { return &m_pBuffer[m_index]; }

        constexpr bool operator==(const ConstIteratorCircularArray& other) const noexcept;
    };

    template<typename TData, Index kCapacity>
    constexpr ConstIteratorCircularArray<TData, kCapacity>::ConstIteratorCircularArray(const TData* pBuffer, int64 index, bool endIsBegin)
        : m_pBuffer(pBuffer)
        , m_index(index)
        , m_shouldWrap(endIsBegin)
    {
    }

    template<typename TData, Index kCapacity>
    constexpr ConstIteratorCircularArray<TData, kCapacity>& ConstIteratorCircularArray<TData, kCapacity>::operator++()
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
    constexpr ConstIteratorCircularArray<TData, kCapacity> ConstIteratorCircularArray<TData, kCapacity>::operator++(int32)
    {
        ConstIteratorCircularArray iterator = *this;
        ++(*this);
        return iterator;
    }

    template<typename TData, Index kCapacity>
    constexpr ConstIteratorCircularArray<TData, kCapacity>& ConstIteratorCircularArray<TData, kCapacity>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++(*this);
        }
        return *this;
    }

    template<typename TData, Index kCapacity>
    constexpr ConstIteratorCircularArray<TData, kCapacity> ConstIteratorCircularArray<TData, kCapacity>::operator+(size_t offset)
    {
        ConstIteratorCircularArray iterator = *this;
        return iterator += offset;
    }

    template<typename TData, Index kCapacity>
    constexpr ConstIteratorCircularArray<TData, kCapacity>& ConstIteratorCircularArray<TData, kCapacity>::operator--()
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
    constexpr ConstIteratorCircularArray<TData, kCapacity> ConstIteratorCircularArray<TData, kCapacity>::operator--(int32)
    {
        ConstIteratorCircularArray iterator = *this;
        --(*this);
        return iterator;
    }

    template<typename TData, Index kCapacity>
    constexpr ConstIteratorCircularArray<TData, kCapacity>& ConstIteratorCircularArray<TData, kCapacity>::operator-=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            --(*this);
        }
        return *this;
    }

    template<typename TData, Index kCapacity>
    constexpr ConstIteratorCircularArray<TData, kCapacity> ConstIteratorCircularArray<TData, kCapacity>::operator-(size_t offset)
    {
        ConstIteratorCircularArray iterator = *this;
        return iterator -= offset;
    }

    template<typename TData, Index kCapacity>
    constexpr bool ConstIteratorCircularArray<TData, kCapacity>::operator==(const ConstIteratorCircularArray& other) const noexcept
    {
        return !m_shouldWrap &&
               m_pBuffer == other.m_pBuffer &&
               m_index   == other.m_index;
    }
}