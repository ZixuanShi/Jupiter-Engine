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
		using TFunc = Function<TReturn(TArgs...)>;

	private:
		DynamicArray<TFunc> m_functions;

	public:
		/** Adds a new function to this Slot and returns it's index as handle */
		constexpr size_t Add(TFunc&& function);			// Accepts R-value to free functions and lambdas
		constexpr size_t Add(const TFunc& function);	// Accepts L-value to existing jpt::Function instance

		/** Erases a function from this Slot with O(1) time
			@param	index		The index of the function to erase from this Slot */
		constexpr void Erase(size_t index);

		/** Enumerate Slot, try to find and erase this function.
			@note	When possible, use handle and erase with index instead for performance */
		constexpr bool Erase(TFunc&& function);

		/** Clears all the functions in this slot. Reset array to empty */
		constexpr void Clear();

		/** Calls all functions in this Slot with the given arguments */
		constexpr void operator()(TArgs&&... args);
		constexpr void CallAll(TArgs&&... args);

		/** Calls a function in this Slot with the given arguments
			@param	index		The handle of the function to call from this Slot */
		constexpr TReturn Call(size_t index, TArgs&&... args);

		/** Calls all functions in this Slot with the given arguments and returns the results
			@return		Strictly matches the member functions layout. Will insert empty result if the Function at index is Not Set */
		constexpr DynamicArray<TReturn> ReturnAll(TArgs&&... args);
	};

	template<class TReturn, class ...TArgs>
	constexpr size_t Slot<TReturn(TArgs...)>::Add(TFunc&& function)
	{
		for (size_t i = 0; i < m_functions.Size(); ++i)
		{
			if (!m_functions[i].IsSet())
			{
				m_functions[i] = Move(function);
				return i;
			}
		}

		m_functions.EmplaceBack(Move(function));
		return m_functions.Size() - 1;
	}

	template<class TReturn, class ...TArgs>
	constexpr size_t Slot<TReturn(TArgs...)>::Add(const TFunc& function)
	{
		for (size_t i = 0; i < m_functions.Size(); ++i)
		{
			if (!m_functions[i].IsSet())
			{
				m_functions[i] = function;
				return i;
			}
		}

		m_functions.EmplaceBack(function);
		return m_functions.Size() - 1;
	}

	template<class TReturn, class ...TArgs>
	constexpr void Slot<TReturn(TArgs...)>::Erase(size_t index)
	{
		JPT_ASSERT(index < m_functions.Size());
		m_functions[index].Clear();
	}

	template<class TReturn, class ...TArgs>
	constexpr bool Slot<TReturn(TArgs...)>::Erase(TFunc&& function)
	{
		for (size_t i = 0; i < m_functions.Size(); ++i)
		{
			if (m_functions[i] == function)
			{
				m_functions[i].Clear();
				return true;
			}
		}

		return false;
	}

	template<class TReturn, class ...TArgs>
	constexpr void Slot<TReturn(TArgs...)>::Clear()
	{
		m_functions.Clear();
	}

	template<class TReturn, class ...TArgs>
	constexpr void Slot<TReturn(TArgs...)>::operator()(TArgs && ...args)
	{
		CallAll(Forward<TArgs>(args)...);
	}

	template<class TReturn, class ...TArgs>
	constexpr void Slot<TReturn(TArgs...)>::CallAll(TArgs && ...args)
	{
		for (auto& function : m_functions)
		{
			if (function.IsSet())
			{
				function(Forward<TArgs>(args)...);
			}
		}
	}

	template<class TReturn, class ...TArgs>
	constexpr TReturn Slot<TReturn(TArgs...)>::Call(size_t index, TArgs && ...args)
	{
		return m_functions[index](Forward<TArgs>(args)...);
	}

	template<class TReturn, class ...TArgs>
	constexpr DynamicArray<TReturn> Slot<TReturn(TArgs...)>::ReturnAll(TArgs && ...args)
	{
		DynamicArray<TReturn> results;
		results.Reserve(m_functions.Size());

		for (auto& function : m_functions)
		{
			if (function.IsSet())
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