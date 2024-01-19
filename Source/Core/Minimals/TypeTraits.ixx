// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.TypeTraits;

export namespace jpt
{
	// Rvalue and Lvalue Utils
	template<typename T> struct RemoveReference              { using Type = T;  };
	template<typename T> struct RemoveReference<T&>          { using Type = T;  };
	template<typename T> struct RemoveReference<T&&>         { using Type = T;  };
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
	template<typename Type, typename ...Types>
	constexpr bool IsAnyOf = (IsSameType<Type, Types> || ...);

	/** @return		Whether the given type is empty or not. */
	template<typename T>
	constexpr bool IsEmptyObj = __is_empty(T);

	// enable_if implementation [Deprecated. Use Concepts instead]
	template<bool kCondition, typename ReturnType = void> struct enable_if {};
	template<typename _ReturnType>                        struct enable_if<true, _ReturnType> { using ReturnType = _ReturnType; };
	template<bool kCondition, typename ReturnType = void> using enable_if_t = enable_if<kCondition, ReturnType>::ReturnType;

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
}