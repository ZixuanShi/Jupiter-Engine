// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

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
		constexpr Function(FunctionType* pFunction) : m_pFunction(pFunction) {}

		template<class TFunc>
		constexpr Function& operator=(TFunc&& func);

		constexpr TReturn operator()(TArgs&&... args);
	};

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
		return m_pFunction(Forward<TArgs>(args)...);
	}
}