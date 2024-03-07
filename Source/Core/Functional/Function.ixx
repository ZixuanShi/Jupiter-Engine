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

	/** Wrapper for a function that can hold a global function, a lambda, or a member function
		@example: jpt::Function<int32(int32, int32)> func; */
	template<class TReturn, class... TArgs>
	class Function<TReturn(TArgs...)>
	{
	public:
		using TSignature = TReturn(*)(TArgs...);

	private:
		TSignature m_function = nullptr;	/**< Pointer to the function to call */

	public:
		/** Connects a global function or lambda to this jpt::Function
			@example: func.Connect(&Add);
			@example: func.Connect([](int32 a, int32 b) -> int32 { return a - b; }); */
		template<class TFunction>
		constexpr void Connect(TFunction function);

		/** Calls the connected function 
			@example: func(1, 2); */
		constexpr TReturn operator()(TArgs... args) const;

		constexpr void Disconnect();
		constexpr bool IsConnected() const { return m_function != nullptr; }
	};

	template<class TReturn, class ...TArgs>
	template<class TFunction>
	constexpr void Function<TReturn(TArgs...)>::Connect(TFunction function)
	{
		m_function = function;
	}

	template<class TReturn, class ...TArgs>
	constexpr TReturn Function<TReturn(TArgs...)>::operator()(TArgs ...args) const
	{
		JPT_ASSERT(m_function != nullptr, "Function is not connected");
		return m_function(Forward<TArgs>(args)...);
	}

	template<class TReturn, class ...TArgs>
	constexpr void Function<TReturn(TArgs...)>::Disconnect()
	{
		m_function = nullptr;
	}
}