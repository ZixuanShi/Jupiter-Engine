// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <type_traits>

export module jpt.TypeTraits;

import jpt.Constants;

namespace jpt_private
{
#pragma region Removing Qualifiers
	template<typename T> struct RemoveReference              { using Type = T;  };
	template<typename T> struct RemoveReference<T&>          { using Type = T;  };
	template<typename T> struct RemoveReference<T&&>         { using Type = T;  };
	template<typename T> struct RemovePointer                { using Type = T;  };
	template<typename T> struct RemovePointer<T*>            { using Type = T;  };
	template<typename T> struct RemovePointer<const T*>      { using Type = T;  };
	template<typename T> struct RemoveConst                  { using Type = T;  };
	template<typename T> struct RemoveConst<const T>         { using Type = T;  };
	template<typename T> struct RValueToLValueReference      { using Type = T;  };
	template<typename T> struct RValueToLValueReference<T&&> { using Type = T&; };
	template<typename T> struct Decay                        { using Type = T;  };
	template<typename T> struct Decay<T&>                    { using Type = T;  };
	template<typename T> struct Decay<T&&>                   { using Type = T;  };
	template<typename T> struct Decay<const T>               { using Type = T;  };
	template<typename T> struct Decay<const T&>              { using Type = T;  };
	template<typename T> struct Decay<const T&&>             { using Type = T;  };
#pragma endregion

#pragma region Adding Qualifiers
	template<typename T> struct AddLValueReference { using Type = T&;               };
	template<typename T> struct AddRValueReference { using Type = T&&;              };
	template<typename T> struct AddConst           { using Type = const T;          };
	template<typename T> struct AddVolatile        { using Type = volatile T;       };
	template<typename T> struct AddConstVolatile   { using Type = const volatile T; };
#pragma endregion
}

export namespace jpt
{
#pragma region Removing Qualifiers
	template<typename T> using TRemoveReference         = typename jpt_private::RemoveReference<T>::Type;
	template<typename T> using TRemovePointer           = typename jpt_private::RemovePointer<T>::Type;
	template<typename T> using TRemoveConst             = typename jpt_private::RemoveConst<T>::Type;
	template<typename T> using TRValueToLValueReference = typename jpt_private::RValueToLValueReference<T>::Type;
	template<typename T> using TDecay                   = typename jpt_private::Decay<T>::Type;

#pragma endregion

#pragma region Adding Qualifiers
	template<typename T> using TAddLValueReference = typename jpt_private::AddLValueReference<T>::Type;
	template<typename T> using TAddRValueReference = typename jpt_private::AddRValueReference<T>::Type;
	template<typename T> using TAddConst           = typename jpt_private::AddConst<T>::Type;
	template<typename T> using TAddVolatile        = typename jpt_private::AddVolatile<T>::Type;
	template<typename T> using TAddConstVolatile   = typename jpt_private::AddConstVolatile<T>::Type;

#pragma endregion

#pragma region Type Identification
	template<typename T> constexpr bool IsLValueRef      = false;
	template<typename T> constexpr bool IsLValueRef<T&>  = true;
	template<typename T> constexpr bool IsRValueRef      = false;
	template<typename T> constexpr bool IsRValueRef<T&&> = true;
	template<typename T> constexpr bool IsRef            = false;
	template<typename T> constexpr bool IsRef<T&>        = true;
	template<typename T> constexpr bool IsRef<T&&>       = true;

	// Same type checking
	// Example: jpt::AreSameType<int32, float>
	template<typename A, typename B> constexpr bool AreSameType       = false;
	template<typename A>             constexpr bool AreSameType<A, A> = true;

	/** @return true if the Type is within any of the passed ...Types
		@example:
				int foo = 42;
				double bar = 42.0;
				jpt::IsAnyOf<decltype(foo), float, char, int>; // true
				jpt::IsAnyOf<decltype(bar), float, char, int>; // false	*/
	template<typename T, typename ...TOthers> constexpr bool IsAnyOf  = (AreSameType<T, TOthers> || ...);

	// Is Array
	/** @note	This doesn't work on heap allocated array
		@example	
			int32 numArray[] = { 0,1 };	
			jpt::IsArray<decltype(numArray)> // true
		
		@example
			int32* numArray = new int[2];
			jpt::IsArray<decltype(numArray)>;	// false */
	template<typename T>           constexpr bool IsArray       = false;
	template<typename T>           constexpr bool IsArray<T[]>  = true;
	template<typename T, size_t N> constexpr bool IsArray<T[N]> = true;
#pragma endregion

#pragma region Type Properties

	template<typename T>
	constexpr bool IsTrivial = std::is_trivial_v<T>;

	template<typename T>
	constexpr bool IsSmall = sizeof(T) <= kSmallDataSize;

#pragma endregion

#pragma region EnableIf
	template<bool kCondition, typename ReturnType = void> struct EnableIf {};
	template<typename _ReturnType>                        struct EnableIf<true, _ReturnType> { using ReturnType = _ReturnType; };
	template<bool kCondition, typename ReturnType = void> using  TEnableIf = EnableIf<kCondition, ReturnType>::ReturnType;
#pragma endregion
}