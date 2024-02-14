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

		/** Assign function/lambda */
		template<class TFunc> constexpr Function(const TFunc& func) requires NotSameType<TFunc, Function>;
		template<class TFunc> constexpr Function(TFunc&& func)      requires NotSameType<TFunc, Function>;
		template<class TFunc> constexpr Function& operator=(const TFunc& func) requires NotSameType<TFunc, Function>;
		template<class TFunc> constexpr Function& operator=(TFunc&& func)      requires NotSameType<TFunc, Function>;

		/** Call the function */
		template<typename T> constexpr TReturn operator()(const T& param);
		template<typename T> constexpr TReturn Call(const T& param);
		constexpr TReturn operator()(TArgs&&... args);
		constexpr TReturn Call(TArgs&&... args);
		
		/** Utilities query */
		constexpr void Clear() { m_pFunction = nullptr; }
		constexpr bool IsSet() const { return m_pFunction != nullptr; }
		constexpr bool operator==(const Function& other) const;
	};

	template<class TReturn, class ...TArgs>
	template<class TFunc>
	constexpr Function<TReturn(TArgs...)>::Function(const TFunc& func) requires NotSameType<TFunc, Function>
		: m_pFunction(func)
	{
	}

	template<class TReturn, class ...TArgs>
	template<class TFunc>
	constexpr Function<TReturn(TArgs...)>& Function<TReturn(TArgs...)>::operator=(const TFunc& func) requires NotSameType<TFunc, Function>
	{
		m_pFunction = func;
		return *this;
	}

	template<class TReturn, class ...TArgs>
	template<class TFunc>
	constexpr Function<TReturn(TArgs...)>::Function(TFunc&& func) requires NotSameType<TFunc, Function>
		: m_pFunction(Move(func))
	{
	}

	template<class TReturn, class ...TArgs>
	template<class TFunc>
	constexpr Function<TReturn(TArgs...)>& Function<TReturn(TArgs...)>::operator=(TFunc&& func) requires NotSameType<TFunc, Function>
	{
		m_pFunction = Move(func);
		return *this;
	}

	template<class TReturn, class ...TArgs>
	template<typename T>
	constexpr TReturn Function<TReturn(TArgs...)>::operator()(const T& param)
	{
		return Call(param);
	}

	template<class TReturn, class ...TArgs>
	template<typename T>
	constexpr TReturn Function<TReturn(TArgs...)>::Call(const T& param)
	{
		JPT_ASSERT(IsSet(), "m_pFunction is nullptr");
		return m_pFunction(param);
	}

	template<class TReturn, class... TArgs>
	constexpr TReturn Function<TReturn(TArgs...)>::operator()(TArgs&&... args)
	{
		return Call(Forward<TArgs>(args)...);
	}

	template<class TReturn, class ...TArgs>
	constexpr TReturn Function<TReturn(TArgs...)>::Call(TArgs&& ...args)
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