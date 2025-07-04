// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Stack;

import jpt.DynamicArray;
import jpt.Utilities;

export namespace jpt
{
    template<typename _TData>
    class Stack
    {
    public:
        using TData = _TData;
        using Iterator = typename DynamicArray<TData>::Iterator;
        using ConstIterator = typename DynamicArray<TData>::ConstIterator;

    private:
        DynamicArray<TData> m_container;

    public:
        // Adding
        constexpr void Push(const TData& value);
        constexpr void Push(TData&& value);
        template <typename... TArgs> constexpr void Emplace(TArgs&&... args);

        // Erasing
        constexpr void Pop();
        constexpr void Clear();

        // Accessing
        constexpr       TData& Peek() noexcept;
        constexpr const TData& Peek() const noexcept;

        // Iterators
        constexpr Iterator begin() noexcept;
        constexpr Iterator end()   noexcept;
        constexpr ConstIterator begin()  const noexcept;
        constexpr ConstIterator end()    const noexcept;
        constexpr ConstIterator cbegin() const noexcept;
        constexpr ConstIterator cend()   const noexcept;

        // Capacity
        constexpr bool IsEmpty() const noexcept;
        constexpr size_t Count() const noexcept;
        constexpr size_t Capacity() const noexcept;

        // Modifiers
        constexpr void Reserve(size_t capacity);
    };

    template<typename TData>
    constexpr void Stack<TData>::Push(const TData& value)
    {
        m_container.Add(value);
    }

    template<typename TData>
    constexpr void Stack<TData>::Push(TData&& value)
    {
        m_container.Add(Move(value));
    }

    template<typename TData>
    template<typename ...TArgs>
    constexpr void Stack<TData>::Emplace(TArgs && ...args)
    {
        m_container.EmplaceBack(Forward<TArgs>(args)...);
    }

    template<typename TData>
    constexpr void Stack<TData>::Pop()
    {
        m_container.Pop();
    }

    template<typename TData>
    constexpr void Stack<TData>::Clear()
    {
        m_container.Clear();
    }

    template<typename TData>
    constexpr Stack<TData>::TData& Stack<TData>::Peek() noexcept
    {
        return m_container.Back();
    }

    template<typename TData>
    constexpr const Stack<TData>::TData& Stack<TData>::Peek() const noexcept
    {
        return m_container.Back();
    }

    template<typename TData>
    constexpr Stack<TData>::Iterator Stack<TData>::begin() noexcept
    {
        return m_container.begin();
    }

    template<typename TData>
    constexpr Stack<TData>::Iterator Stack<TData>::end() noexcept
    {
        return m_container.end();
    }

    template<typename TData>
    constexpr Stack<TData>::ConstIterator Stack<TData>::begin() const noexcept
    {
        return m_container.begin();
    }

    template<typename TData>
    constexpr Stack<TData>::ConstIterator Stack<TData>::end() const noexcept
    {
        return m_container.end();
    }

    template<typename TData>
    constexpr Stack<TData>::ConstIterator Stack<TData>::cbegin() const noexcept
    {
        return m_container.begin();
    }

    template<typename TData>
    constexpr Stack<TData>::ConstIterator Stack<TData>::cend() const noexcept
    {
        return m_container.end();
    }

    template<typename TData>
    constexpr bool Stack<TData>::IsEmpty() const noexcept
    {
        return m_container.IsEmpty();
    }

    template<typename TData>
    constexpr size_t Stack<TData>::Count() const noexcept
    {
        return m_container.Count();
    }

    template<typename TData>
    constexpr size_t Stack<TData>::Capacity() const noexcept
    {
        return m_container.Capacity();
    }

    template<typename TData>
    constexpr void Stack<TData>::Reserve(size_t capacity)
    {
        m_container.Reserve(capacity);
    }

}