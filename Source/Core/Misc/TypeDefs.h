// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

// TypeDefs
using uint8  = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using int32  = int32_t;
using int64  = int64_t;

namespace jpt
{
	template <typename Type> inline const Type kInvalidValue = (std::numeric_limits<Type>::max)();	/**< If the value is not supported, you need to specialize template it in the corresponding file */
	template <typename Type> inline const Type kDefaultValue = static_cast<Type>(0);
	
	// Timing
	using TimingPrecision = double;
	using Clock        = std::chrono::high_resolution_clock;
	using TimePoint    = std::chrono::time_point<Clock>;
	using TimeDuration = std::chrono::duration<TimingPrecision>;
	
	// Rvalue and Lvalue Utils
	template<typename T> struct RemoveReference              { using Type = T; };
	template<typename T> struct RemoveReference<T&>          { using Type = T; };
	template<typename T> struct RemoveReference<T&&>         { using Type = T; };
	template<typename T> struct RemoveConst                  { using Type = T; };
	template<typename T> struct RemoveConst<const T>         { using Type = T; };
	template<typename T> struct IsLValueReferenceType        { static constexpr bool Value = false; };
	template<typename T> struct IsLValueReferenceType<T&>    { static constexpr bool Value = true;  };
	template<typename T> struct IsRValueReferenceType        { static constexpr bool Value = false; };
	template<typename T> struct IsRValueReferenceType<T&&>   { static constexpr bool Value = true;  };
	template<typename T> struct RValueToLValueReference      { using Type = T; };
	template<typename T> struct RValueToLValueReference<T&&> { using Type = T&; };

	// Same type checking
	// Example: jpt::IsSameType<int32, float>::Value
	template<typename A, typename B> struct IsSameType       { static constexpr bool Value = false; };
	template<typename A>             struct IsSameType<A, A> { static constexpr bool Value = true;  };

	// enable_if implementation
	template<bool kCondition, typename ReturnType = void> struct enable_if {};
	template<typename _ReturnType>	                      struct enable_if<true, _ReturnType> { using ReturnType = _ReturnType; };
	template<bool kCondition, typename ReturnType = void> using enable_if_t = enable_if<kCondition, ReturnType>::ReturnType;
}