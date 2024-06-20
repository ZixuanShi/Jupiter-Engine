// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Slot;

export import jpt.Function;

import jpt.DynamicArray;
import jpt.Utilities;
import jpt.Math;
import jpt.TypeTraits;

export namespace jpt
{
	template<class>
	class Slot;

	/** Array of Functions as Slot */
	template<class TReturn, class... TArgs>
	class Slot<TReturn(TArgs...)>
	{
		using TFunction = Function<TReturn(TArgs...)>;

	private:
		DynamicArray<TFunction> m_functions;

	public:
		constexpr size_t Add(const TFunction& function);

		template<class T>
		constexpr size_t Add(const T& function) requires (!AreSameType<T, TFunction>);

		template<class TCaller>
		constexpr size_t Add(TCaller* pCaller, TReturn(TCaller::* pMemberFunction)(TArgs...));

		constexpr void Erase(size_t index);

		constexpr bool IsConnected(size_t index) const;

		constexpr void Clear();

		constexpr void Reserve(size_t count);

		constexpr void operator()(TArgs... args) const;
		constexpr DynamicArray<TReturn> ReturnAll(TArgs... args);
	};

	template<class TReturn, class ...TArgs>
	constexpr size_t Slot<TReturn(TArgs...)>::Add(const TFunction& function)
	{
		for (size_t i = 0; i < m_functions.Count(); ++i)
		{
			if (!m_functions[i].IsConnected())
			{
				m_functions[i] = function;
				return i;
			}
		}

		m_functions.Add(function);
		return m_functions.Count() - 1;
	}

	template<class TReturn, class ...TArgs>
	template<class T>
	constexpr size_t Slot<TReturn(TArgs...)>::Add(const T& function) requires (!AreSameType<T, TFunction>)
	{
		for (size_t i = 0; i < m_functions.Count(); ++i)
		{
			if (!m_functions[i].IsConnected())
			{
				m_functions[i].Connect(function);
				return i;
			}
		}

		m_functions.EmplaceBack(function);
		return m_functions.Count() - 1;
	}

	template<class TReturn, class ...TArgs>
	template<class TCaller>
	constexpr size_t Slot<TReturn(TArgs...)>::Add(TCaller* pCaller, TReturn(TCaller::* pMemberFunction)(TArgs...))
	{
		for (size_t i = 0; i < m_functions.Count(); ++i)
		{
			if (!m_functions[i].IsConnected())
			{
				m_functions[i].Connect(pCaller, pMemberFunction);
				return i;
			}
		}

		m_functions.EmplaceBack(pCaller, pMemberFunction);
		return m_functions.Count() - 1;
	}

	template<class TReturn, class ...TArgs>
	constexpr void Slot<TReturn(TArgs...)>::Erase(size_t index)
	{
		JPT_ASSERT(index < m_functions.Count(), "Index out of range");
		m_functions[index].Disconnect();
	}

	template<class TReturn, class ...TArgs>
	constexpr bool Slot<TReturn(TArgs...)>::IsConnected(size_t index) const
	{
		JPT_ASSERT(index < m_functions.Count(), "Index out of range");
		return m_functions[index].IsConnected();
	}

	template<class TReturn, class ...TArgs>
	constexpr void Slot<TReturn(TArgs...)>::Clear()
	{
		m_functions.Clear();
	}

	template<class TReturn, class ...TArgs>
	constexpr void Slot<TReturn(TArgs...)>::Reserve(size_t count)
	{
		m_functions.Reserve(count);
	}

	template<class TReturn, class ...TArgs>
	constexpr void Slot<TReturn(TArgs...)>::operator()(TArgs ...args) const
	{
		for (const TFunction& function : m_functions)
		{
			if (function.IsConnected())
			{
				function(Forward<TArgs>(args)...);
			}
		}
	}

	template<class TReturn, class ...TArgs>
	constexpr DynamicArray<TReturn> Slot<TReturn(TArgs...)>::ReturnAll(TArgs ...args)
	{
		static_assert(!AreSameType<TReturn, void>, "Can't return from void");

		DynamicArray<TReturn> results;

		for (const TFunction& function : m_functions)
		{
			if (function.IsConnected())
			{
				results.EmplaceBack(function(Forward<TArgs>(args)...));
			}
			else
			{
				results.EmplaceBack();
			}
		}

		return results;
	}
}