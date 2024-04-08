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
		struct BaseFunction
		{
			virtual ~BaseFunction() {}
			virtual TReturn operator()(TArgs... args) const = 0;
			virtual BaseFunction* Clone() const = 0;
		};

		template<class TFunction>
		struct FunctionData final : public BaseFunction
		{
			TFunction m_function;

			FunctionData(TFunction function)
				: m_function(function)
			{
			}

			virtual TReturn operator()(TArgs... args) const override final
			{
				if constexpr (AreSameType<TReturn, void>)
				{
					m_function(Forward<TArgs>(args)...);
				}
				else
				{
					return m_function(Forward<TArgs>(args)...);
				}
			}

			virtual BaseFunction* Clone() const override final
			{
				return new FunctionData(m_function);
			}
		};

		template<class TCaller>
		struct MemberFunctionData final : public BaseFunction
		{
			TCaller* m_pCaller;
			TReturn(TCaller::*m_pMemberFunction)(TArgs...);

			MemberFunctionData(TCaller* pCaller, TReturn(TCaller::* pMemberFunction)(TArgs...)) 
				: m_pCaller(pCaller)
				, m_pMemberFunction(pMemberFunction) 
			{
			}

			virtual TReturn operator()(TArgs... args) const override final
			{
				if constexpr (AreSameType<TReturn, void>)
				{
					(m_pCaller->*m_pMemberFunction)(Forward<TArgs>(args)...);
				}
				else
				{
					return (m_pCaller->*m_pMemberFunction)(Forward<TArgs>(args)...);
				}
			}

			virtual BaseFunction* Clone() const override final
			{
				return new MemberFunctionData(m_pCaller, m_pMemberFunction);
			}
		};

	private:
		BaseFunction* m_pFunction = nullptr;

	public:
		constexpr Function() = default;
		constexpr Function(const Function& other);
		constexpr Function(Function&& other);
		constexpr Function& operator=(const Function& other);
		constexpr Function& operator=(Function&& other);
		constexpr ~Function();

		template<class TFunction>
		constexpr Function(TFunction function);

		template<class TFunction>
		constexpr Function& operator=(TFunction function);

		template<class TCaller>
		constexpr Function(TCaller* pCaller, TReturn(TCaller::* pMemberFunction)(TArgs...));
	
		/** Connects a global function or lambda to this jpt::Function
			@example: func.Connect(&Add);
			@example: func.Connect([](int32 a, int32 b) -> int32 { return a - b; }); */
		template<class TFunction>
		constexpr void Connect(TFunction function);

		/** Connects a member function to this jpt::Function
			@example: func.Connect(&foo, &Foo::Work); */
		template<class TCaller>
		constexpr void Connect(TCaller* pCaller, TReturn(TCaller::* pMemberFunction)(TArgs...));

		/** Calls the connected function 
			@example: func(1, 2); */
		constexpr TReturn operator()(TArgs... args) const;

		constexpr void Disconnect();
		constexpr bool IsConnected() const;
	};

	template<class TReturn, class ...TArgs>
	constexpr Function<TReturn(TArgs...)>::Function(const Function& other)
	{
		if (other.IsConnected())
		{
			m_pFunction = other.m_pFunction->Clone();
		}
	}

	template<class TReturn, class ...TArgs>
	constexpr Function<TReturn(TArgs...)>::Function(Function&& other)
	{
		m_pFunction = other.m_pFunction;
		other.m_pFunction = nullptr;
	}

	template<class TReturn, class ...TArgs>
	constexpr Function<TReturn(TArgs...)>& Function<TReturn(TArgs...)>::operator=(const Function& other)
	{
		if (this != &other)
		{
			Disconnect();
			if (other.IsConnected())
			{
				m_pFunction = other.m_pFunction->Clone();
			}
		}

		return *this;
	}

	template<class TReturn, class ...TArgs>
	constexpr Function<TReturn(TArgs...)>& Function<TReturn(TArgs...)>::operator=(Function&& other)
	{
		if (this != &other)
		{
			Disconnect();
			m_pFunction = other.m_pFunction;
			other.m_pFunction = nullptr;
		}

		return *this;
	}

	template<class TReturn, class ...TArgs>
	constexpr Function<TReturn(TArgs...)>::~Function()
	{
		Disconnect();
	}

	template<class TReturn, class ...TArgs>
	template<class TFunction>
	constexpr Function<TReturn(TArgs...)>::Function(TFunction function)
	{
		Connect(function);
	}

	template<class TReturn, class ...TArgs>
	template<class TFunction>
	constexpr Function<TReturn(TArgs...)>& Function<TReturn(TArgs...)>::operator=(TFunction function)
	{
		Disconnect();
		Connect(function);
		return *this;
	}

	template<class TReturn, class ...TArgs>
	template<class TCaller>
	constexpr Function<TReturn(TArgs...)>::Function(TCaller* pCaller, TReturn(TCaller::* pMemberFunction)(TArgs...))
	{
		Connect(pCaller, pMemberFunction);
	}

	template<class TReturn, class ...TArgs>
	template<class TFunction>
	constexpr void Function<TReturn(TArgs...)>::Connect(TFunction function)
	{
		Disconnect();
		m_pFunction = new FunctionData<TFunction>(function);
	}

	template<class TReturn, class ...TArgs>
	template<class TCaller>
	constexpr void Function<TReturn(TArgs...)>::Connect(TCaller* pCaller, TReturn(TCaller::*pMemberFunction)(TArgs...))
	{
		Disconnect();
		m_pFunction = new MemberFunctionData<TCaller>(pCaller, pMemberFunction);
	}

	template<class TReturn, class ...TArgs>
	constexpr TReturn Function<TReturn(TArgs...)>::operator()(TArgs...args) const
	{
		JPT_ASSERT(IsConnected(), "Function is not connected");

		if constexpr (AreSameType<TReturn, void>)
		{
			(*m_pFunction)(Forward<TArgs>(args)...);
		}
		else
		{
			return (*m_pFunction)(Forward<TArgs>(args)...);
		}
	}

	template<class TReturn, class ...TArgs>
	constexpr void Function<TReturn(TArgs...)>::Disconnect()
	{
		delete m_pFunction;
		m_pFunction = nullptr;
	}

	template<class TReturn, class ...TArgs>
	constexpr bool Function<TReturn(TArgs...)>::IsConnected() const
	{
		return m_pFunction != nullptr;
	}
}