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
		using TFunctionPointer = TReturn(*)(TArgs...);

	private:
		TFunctionPointer m_function = nullptr;

	public:
		constexpr Function() = default;

		template<class TFunction>
		constexpr void Connect(TFunction&& function);

		constexpr TReturn operator()(TArgs... args) const;

		constexpr void Disconnect() { m_function = nullptr; }
		constexpr bool IsConnected() const { return m_function != nullptr; }
	};

	template<class TReturn, class ...TArgs>
	template<class TFunction>
	constexpr void Function<TReturn(TArgs...)>::Connect(TFunction&& function)
	{
		m_function = Move(function);
	}

	template<class TReturn, class ...TArgs>
	constexpr TReturn Function<TReturn(TArgs...)>::operator()(TArgs ...args) const
	{
		JPT_ASSERT(m_function != nullptr, "Function is not connected");
		return m_function(Forward<TArgs>(args)...);
	}
}