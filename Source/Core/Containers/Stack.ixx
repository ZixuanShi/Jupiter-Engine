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
		DynamicArray<TData> m_values;

	public:
		Iterator begin() { return m_values.begin(); }
		Iterator end()   { return m_values.end();   }
		ConstIterator begin()  const { return m_values.begin(); }
		ConstIterator end()    const { return m_values.end();   }
		ConstIterator cbegin() const { return m_values.begin(); }
		ConstIterator cend()   const { return m_values.end();   }

		constexpr void Add(const TData& value);
		constexpr void Add(TData&& value);

		template <typename... TArgs> constexpr void Emplace(TArgs&&... args);

		constexpr TData Pop();
		constexpr void Clear();

		constexpr TData& Peek();
		constexpr const TData& Peek() const;

		constexpr bool IsEmpty() const;
		constexpr size_t Size() const;
		constexpr size_t Capacity() const;
		constexpr void Reserve(size_t capacity);
	};

	template<typename _TData>
	constexpr void Stack<_TData>::Add(const TData& value)
	{
		m_values.AddBack(value);
	}

	template<typename _TData>
	constexpr void Stack<_TData>::Add(TData&& value)
	{
		m_values.AddBack(Move(value));
	}

	template<typename _TData>
	constexpr Stack<_TData>::TData Stack<_TData>::Pop()
	{
		TData value = m_values.Back();
		m_values.PopBack();
		return value;
	}

	template<typename _TData>
	constexpr Stack<_TData>::TData& Stack<_TData>::Peek()
	{
		return m_values.Back();
	}

	template<typename _TData>
	constexpr const Stack<_TData>::TData& Stack<_TData>::Peek() const
	{
		return m_values.Back();
	}

	template<typename _TData>
	constexpr bool Stack<_TData>::IsEmpty() const
	{
		return m_values.IsEmpty();
	}

	template<typename _TData>
	constexpr size_t Stack<_TData>::Size() const
	{
		return m_values.Size();
	}

	template<typename _TData>
	constexpr size_t Stack<_TData>::Capacity() const
	{
		return m_values.Capacity();
	}

	template<typename _TData>
	constexpr void Stack<_TData>::Reserve(size_t capacity)
	{
		m_values.Reserve(capacity); 
	}

	template<typename _TData>
	constexpr void Stack<_TData>::Clear()
	{
		m_values.Clear();
	}

	template<typename _TData>
	template<typename ...TArgs>
	constexpr void Stack<_TData>::Emplace(TArgs && ...args)
	{
		m_values.EmplaceBack(Forward<TArgs>(args)...);
	}
}