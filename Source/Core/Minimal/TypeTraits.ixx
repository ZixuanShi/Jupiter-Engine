// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.TypeTraits;

export namespace jpt
{
#pragma region Removing Qualifiers
	template<typename T> struct RemoveReference              { using Type = T;  };
	template<typename T> struct RemoveReference<T&>          { using Type = T;  };
	template<typename T> struct RemoveReference<T&&>         { using Type = T;  };
	template<typename T> struct RemoveConst                  { using Type = T;  };
	template<typename T> struct RemoveConst<const T>         { using Type = T;  };
	template<typename T> struct RTValueoLValueReference      { using Type = T;  };
	template<typename T> struct RTValueoLValueReference<T&&> { using Type = T&; };
	template<typename T> struct Decay                        { using Type = T;  };
	template<typename T> struct Decay<T&>                    { using Type = T;  };
	template<typename T> struct Decay<T&&>                   { using Type = T;  };
	template<typename T> struct Decay<const T>               { using Type = T;  };
	template<typename T> struct Decay<const T&>              { using Type = T;  };
	template<typename T> struct Decay<const T&&>             { using Type = T;  };
#pragma endregion

#pragma region Adding Qualifiers
	template<typename T> struct AddLValueReference           { using Type = T&; };
	template<typename T> struct AddRValueReference           { using Type = T&&; };
	template<typename T> struct AddConst                     { using Type = const T; };
	template<typename T> struct AddVolatile                  { using Type = volatile T; };
#pragma endregion

#pragma region Type Properties
	template<typename T> constexpr bool IsLValueRef      = false;
	template<typename T> constexpr bool IsLValueRef<T&>  = true;
	template<typename T> constexpr bool IsRValueRef      = false;
	template<typename T> constexpr bool IsRValueRef<T&&> = true;
	template<typename T> constexpr bool IsRef            = false;
	template<typename T> constexpr bool IsRef<T&>        = true;
	template<typename T> constexpr bool IsRef<T&&>       = true;

	// Same type checking
	// Example: jpt::IsSameType<int32, float>
	template<typename A, typename B> constexpr bool IsSameType       = false;
	template<typename A>             constexpr bool IsSameType<A, A> = true;

	/** @return true if the Type is within any of the passed ...Types
		@example:
				int foo = 42;
				double bar = 42.0;
				jpt::IsAnyOf<decltype(foo), float, char, int>; // true
				jpt::IsAnyOf<decltype(bar), float, char, int>; // false	*/
	template<typename T, typename ...TOthers>
	constexpr bool IsAnyOf = (IsSameType<T, TOthers> || ...);

	/** @return		Whether the given type is empty or not. */
	template<typename T>
	constexpr bool IsEmptyObj = __is_empty(T);

	template<typename T, typename... Args>
	constexpr bool IsConstructible = __is_constructible(T, Args...);

	template<typename T, typename... Args>
	constexpr bool IsTriviallyConstructible = __is_trivially_constructible(T, Args...);

	template<typename FromT, typename ToT>
	constexpr bool IsConvertibleTo = __is_convertible_to(FromT, ToT);

	template<typename T>
	constexpr bool IsTriviallyCopyable = __is_trivially_copyable(T);

	template<typename T>
	constexpr bool IsTriviallyMoveConstructible = __is_trivially_constructible(T, T);

	template<typename T>
	constexpr bool IsTriviallyMoveAssignable = __is_trivially_assignable(typename AddLValueReference<T>::Type, T);

	template<typename T>
	constexpr bool IsTriviallyDestructible = __is_trivially_destructible(T);

	template<typename T>
	constexpr bool IsTrivial = __is_trivially_constructible(T) && __is_trivially_copyable(T);

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
}