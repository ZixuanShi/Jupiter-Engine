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

	template<typename _TData>
	constexpr Stack<_TData>::TData& Stack<_TData>::Peek()
	{
		return m_container.Back();
	}

	template<typename _TData>
	constexpr const Stack<_TData>::TData& Stack<_TData>::Peek() const
	{
		return m_container.Back();
	}

	template<typename _TData>
	constexpr bool Stack<_TData>::IsEmpty() const
	{
		return m_container.IsEmpty();
	}

	template<typename _TData>
	constexpr size_t Stack<_TData>::Count() const
	{
		return m_container.Count();
	}

	template<typename _TData>
	constexpr size_t Stack<_TData>::Capacity() const
	{
		return m_container.Capacity();
	}

	template<typename _TData>
	constexpr void Stack<_TData>::Reserve(size_t capacity)
	{
		m_container.Reserve(capacity);
	}

	template<typename _TData>
	constexpr void Stack<_TData>::Push(const TData& value)
	{
		m_container.AddBack(value);
	}

	template<typename _TData>
	constexpr void Stack<_TData>::Push(TData&& value)
	{
		m_container.AddBack(Move(value));
	}

	template<typename _TData>
	template<typename ...TArgs>
	constexpr void Stack<_TData>::Emplace(TArgs && ...args)
	{
		m_container.EmplaceBack(Forward<TArgs>(args)...);
	}

	template<typename _TData>
	constexpr void Stack<_TData>::Pop()
	{
		m_container.PopBack();
	}

	template<typename _TData>
	constexpr void Stack<_TData>::Clear()
	{
		m_container.Clear();
	}
}