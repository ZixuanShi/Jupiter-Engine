// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Queue;

import jpt.LinkedList;
import jpt.Utilities;

export namespace jpt
{
	template<typename _TData>
	class Queue
	{
	public:
		using TData = _TData;
		using Iterator = typename LinkedList<TData>::Iterator;
		using ConstIterator = typename LinkedList<TData>::ConstIterator;

	private:
		LinkedList<TData> m_values;

	public:
		// Iterators
		Iterator begin() { return m_values.begin(); }
		Iterator end() { return m_values.end(); }
		ConstIterator begin()  const { return m_values.begin(); }
		ConstIterator end()    const { return m_values.end(); }
		ConstIterator cbegin() const { return m_values.begin(); }
		ConstIterator cend()   const { return m_values.end(); }

		// Accessing
		constexpr       TData& Front();
		constexpr const TData& Front() const;
		constexpr       TData& Back();
		constexpr const TData& Back() const;

		// Capacity
		constexpr bool IsEmpty() const;
		constexpr size_t Count() const;

		// Adding
		constexpr void Enqueue(const TData& value);
		constexpr void Enqueue(TData&& value);
		template <typename... TArgs> constexpr void Emplace(TArgs&&... args);

		// Erasing
		constexpr void Dequeue();
		constexpr void Clear();
	};

	template<typename _TData>
	constexpr Queue<_TData>::TData& Queue<_TData>::Front()
	{
		return m_values.Front();
	}

	template<typename _TData>
	constexpr const Queue<_TData>::TData& Queue<_TData>::Front() const
	{
		return m_values.Front();
	}

	template<typename _TData>
	constexpr Queue<_TData>::TData& Queue<_TData>::Back()
	{
		return m_values.Back();
	}

	template<typename _TData>
	constexpr const Queue<_TData>::TData& Queue<_TData>::Back() const
	{
		return m_values.Back();
	}

	template<typename _TData>
	constexpr bool Queue<_TData>::IsEmpty() const
	{
		return m_values.IsEmpty();
	}

	template<typename _TData>
	constexpr size_t Queue<_TData>::Count() const
	{
		return m_values.Count();
	}

	template<typename _TData>
	constexpr void Queue<_TData>::Enqueue(const TData& value)
	{
		m_values.AddBack(value);
	}

	template<typename _TData>
	constexpr void Queue<_TData>::Enqueue(TData&& value)
	{
		m_values.AddBack(Move(value));
	}

	template<typename _TData>
	template<typename ...TArgs>
	constexpr void Queue<_TData>::Emplace(TArgs && ...args)
	{
		m_values.EmplaceBack(Forward<TArgs>(args)...);
	}

	template<typename _TData>
	constexpr void Queue<_TData>::Dequeue()
	{
		m_values.PopFront();
	}

	template<typename _TData>
	constexpr void Queue<_TData>::Clear()
	{
		m_values.Clear();
	}
}