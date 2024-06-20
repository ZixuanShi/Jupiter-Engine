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
		// Iterators
		Iterator begin() { return m_container.begin(); }
		Iterator end()   { return m_container.end();   }
		ConstIterator begin()  const { return m_container.begin(); }
		ConstIterator end()    const { return m_container.end();   }
		ConstIterator cbegin() const { return m_container.begin(); }
		ConstIterator cend()   const { return m_container.end();   }

		// Accessing
		constexpr       TData& Peek();
		constexpr const TData& Peek() const;

		// Capacity
		constexpr bool IsEmpty() const;
		constexpr size_t Count() const;
		constexpr size_t Capacity() const;
		constexpr void Reserve(size_t capacity);

		// Adding
		constexpr void Push(const TData& value);
		constexpr void Push(TData&& value);
		template <typename... TArgs> constexpr void Emplace(TArgs&&... args);

		// Erasing
		constexpr void Pop();
		constexpr void Clear();
	};

	template<typename TData>
	constexpr Stack<TData>::TData& Stack<TData>::Peek()
	{
		return m_container.Back();
	}

	template<typename TData>
	constexpr const Stack<TData>::TData& Stack<TData>::Peek() const
	{
		return m_container.Back();
	}

	template<typename TData>
	constexpr bool Stack<TData>::IsEmpty() const
	{
		return m_container.IsEmpty();
	}

	template<typename TData>
	constexpr size_t Stack<TData>::Count() const
	{
		return m_container.Count();
	}

	template<typename TData>
	constexpr size_t Stack<TData>::Capacity() const
	{
		return m_container.Capacity();
	}

	template<typename TData>
	constexpr void Stack<TData>::Reserve(size_t capacity)
	{
		m_container.Reserve(capacity);
	}

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
}