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
		LinkedList<TData> m_container;

	public:
		// Iterators
		Iterator begin() { return m_container.begin(); }
		Iterator end() { return m_container.end(); }
		ConstIterator begin()  const { return m_container.begin(); }
		ConstIterator end()    const { return m_container.end(); }
		ConstIterator cbegin() const { return m_container.begin(); }
		ConstIterator cend()   const { return m_container.end(); }

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
		return m_container.Front();
	}

	template<typename _TData>
	constexpr const Queue<_TData>::TData& Queue<_TData>::Front() const
	{
		return m_container.Front();
	}

	template<typename _TData>
	constexpr Queue<_TData>::TData& Queue<_TData>::Back()
	{
		return m_container.Back();
	}

	template<typename _TData>
	constexpr const Queue<_TData>::TData& Queue<_TData>::Back() const
	{
		return m_container.Back();
	}

	template<typename _TData>
	constexpr bool Queue<_TData>::IsEmpty() const
	{
		return m_container.IsEmpty();
	}

	template<typename _TData>
	constexpr size_t Queue<_TData>::Count() const
	{
		return m_container.Count();
	}

	template<typename _TData>
	constexpr void Queue<_TData>::Enqueue(const TData& value)
	{
		m_container.AddBack(value);
	}

	template<typename _TData>
	constexpr void Queue<_TData>::Enqueue(TData&& value)
	{
		m_container.AddBack(Move(value));
	}

	template<typename _TData>
	template<typename ...TArgs>
	constexpr void Queue<_TData>::Emplace(TArgs && ...args)
	{
		m_container.EmplaceBack(Forward<TArgs>(args)...);
	}

	template<typename _TData>
	constexpr void Queue<_TData>::Dequeue()
	{
		m_container.PopFront();
	}

	template<typename _TData>
	constexpr void Queue<_TData>::Clear()
	{
		m_container.Clear();
	}
}