// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Slot;

export import jpt.Function;

import jpt.DynamicArray;
import jpt.Utilities;

export namespace jpt
{
	template<class>
	class Slot;

	template<class TReturn, class... TArgs>
	class Slot<TReturn(TArgs...)>
	{
		using TFunc = Function<TReturn(TArgs...)>;

	private:
		DynamicArray<TFunc> m_functions;

	public:
		constexpr void Add(TFunc&& function);
		constexpr void Erase(TFunc&& function);
		constexpr void Clear();

		constexpr void operator()(TArgs&&... args);
		constexpr void CallAll(TArgs&&... args);
	};

	template<class TReturn, class ...TArgs>
	constexpr void Slot<TReturn(TArgs...)>::Add(TFunc&& function)
	{
		m_functions.EmplaceBack(Forward<TFunc>(function));
	}

	template<class TReturn, class ...TArgs>
	constexpr void Slot<TReturn(TArgs...)>::Erase(TFunc&& function)
	{
		for (size_t i = 0; i < m_functions.Size(); ++i)
		{
			if (m_functions[i] == function)
			{
				m_functions.Erase(i);
				break;
			}
		}
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
			function(Forward<TArgs>(args)...);
		}
	}
}