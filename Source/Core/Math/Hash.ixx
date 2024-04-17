// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <concepts>
#include <string>

export module jpt.Hash;

import jpt.Concepts;
import jpt.TypeDefs;

export namespace jpt
{
	template<typename T>
	concept Hashable = requires(T t)
	{
		{ t.Hash() } -> std::convertible_to<uint64>; 
	};

	template<Hashable T> requires NonTrivial<T>
	constexpr uint64 Hash(const T& object)
	{
		return object.Hash();
	}

	template<Hashable T> requires Trivial<T>
	constexpr uint64 Hash(T object)
	{
		return object.Hash();
	}

	template<Primitive T>
	constexpr uint64 Hash(T object)
	{
		return std::hash<T>()(object);
	}

	/** constexpr compile time hash functions, 32 and 64 bit
		@str: should be a null terminated string literal */
	constexpr uint32 StringHash32(const char* const str, const uint32 value = 0x811c9dc5)         noexcept { return (str[0] == '\0') ? value : StringHash32(&str[1], (value ^ uint32(str[0])) * 0x1000193); }
	constexpr uint64 StringHash64(const char* const str, const uint64 value = 0xcbf29ce484222325) noexcept { return (str[0] == '\0') ? value : StringHash64(&str[1], (value ^ uint64(str[0])) * 0x100000001b3); }
	
	constexpr uint64 Hash(const char* cStr)
	{
		return StringHash64(cStr);
	}
}