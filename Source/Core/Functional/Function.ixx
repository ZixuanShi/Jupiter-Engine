// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Function;

import jpt.TypeTraits;
import jpt.Utilities;

export namespace jpt
{
	template<class>
	class Function;

	/** Wrapper for a function. Can be used as Callbacks */
	template<class TReturn, class... TArgs>
	class Function<TReturn(TArgs...)>
	{
	public:
		using FunctionType = TReturn(TArgs...);

	private:
		FunctionType* m_pFunction = nullptr;

	public:
		constexpr Function() = default;

		template<class TFunc>
		constexpr Function(TFunc&& pFunction);

		template<class TFunc>
		constexpr Function& operator=(TFunc&& func);

		constexpr TReturn operator()(TArgs&&... args);
		
		constexpr void Clear() { m_pFunction = nullptr; }
		constexpr bool IsSet() const { return m_pFunction != nullptr; }
		constexpr bool operator==(const Function& other) const;
	};

	template<class TReturn, class ...TArgs>
	template<class TFunc>
	constexpr Function<TReturn(TArgs...)>::Function(TFunc&& pFunction)
		: m_pFunction(pFunction) 
	{
	}

	template<class TReturn, class ...TArgs>
	template<class TFunc>
	constexpr Function<TReturn(TArgs...)>& Function<TReturn(TArgs...)>::operator=(TFunc&& func)
	{
		m_pFunction = func;
		return *this;
	}

	template<class TReturn, class... TArgs>
	constexpr TReturn Function<TReturn(TArgs...)>::operator()(TArgs&&... args)
	{
		JPT_ASSERT(IsSet(), "m_pFunction is nullptr");
		return m_pFunction(Forward<TArgs>(args)...);
	}

	template<class TReturn, class ...TArgs>
	constexpr bool Function<TReturn(TArgs...)>::operator==(const Function& other) const
	{
		return m_pFunction == other.m_pFunction;
	}
}