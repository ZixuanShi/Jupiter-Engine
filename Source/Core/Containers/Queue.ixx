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
		constexpr Iterator begin() noexcept;
		constexpr Iterator end() noexcept;
		constexpr ConstIterator begin()  const noexcept;
		constexpr ConstIterator end()    const noexcept;
		constexpr ConstIterator cbegin() const noexcept;
		constexpr ConstIterator cend()   const noexcept;

		// Accessing
		constexpr       TData& Front() noexcept;
		constexpr const TData& Front() const noexcept;
		constexpr       TData& Back() noexcept;
		constexpr const TData& Back() const noexcept;

		// Capacity
		constexpr bool IsEmpty() const noexcept;
		constexpr size_t Count() const noexcept;

		// Adding
		constexpr void Enqueue(const TData& value);
		constexpr void Enqueue(TData&& value);
		template <typename... TArgs> constexpr void Emplace(TArgs&&... args);

		// Erasing
		constexpr void Dequeue();
		constexpr void Clear();
	};

	template<typename TData>
	constexpr Queue<TData>::Iterator Queue<TData>::begin() noexcept
	{
		return m_container.begin();
	}

	template<typename TData>
	constexpr Queue<TData>::Iterator Queue<TData>::end() noexcept
	{
		return m_container.end();
	}

	template<typename TData>
	constexpr Queue<TData>::ConstIterator Queue<TData>::begin() const noexcept
	{
		return m_container.begin();
	}

	template<typename TData>
	constexpr Queue<TData>::ConstIterator Queue<TData>::end() const noexcept
	{
		return m_container.end();
	}

	template<typename TData>
	constexpr Queue<TData>::ConstIterator Queue<TData>::cbegin() const noexcept
	{
		return m_container.begin();
	}

	template<typename TData>
	constexpr Queue<TData>::ConstIterator Queue<TData>::cend() const noexcept
	{
		return m_container.end();
	}

	template<typename TData>
	constexpr Queue<TData>::TData& Queue<TData>::Front() noexcept
	{
		return m_container.Front();
	}

	template<typename TData>
	constexpr const Queue<TData>::TData& Queue<TData>::Front() const noexcept
	{
		return m_container.Front();
	}

	template<typename TData>
	constexpr Queue<TData>::TData& Queue<TData>::Back() noexcept
	{
		return m_container.Back();
	}

	template<typename TData>
	constexpr const Queue<TData>::TData& Queue<TData>::Back() const noexcept
	{
		return m_container.Back();
	}

	template<typename TData>
	constexpr bool Queue<TData>::IsEmpty() const noexcept
	{
		return m_container.IsEmpty();
	}

	template<typename TData>
	constexpr size_t Queue<TData>::Count() const noexcept
	{
		return m_container.Count();
	}

	template<typename TData>
	constexpr void Queue<TData>::Enqueue(const TData& value)
	{
		m_container.AddBack(value);
	}

	template<typename TData>
	constexpr void Queue<TData>::Enqueue(TData&& value)
	{
		m_container.AddBack(Move(value));
	}

	template<typename TData>
	template<typename ...TArgs>
	constexpr void Queue<TData>::Emplace(TArgs && ...args)
	{
		m_container.EmplaceBack(Forward<TArgs>(args)...);
	}

	template<typename TData>
	constexpr void Queue<TData>::Dequeue()
	{
		m_container.PopFront();
	}

	template<typename TData>
	constexpr void Queue<TData>::Clear()
	{
		m_container.Clear();
	}
}