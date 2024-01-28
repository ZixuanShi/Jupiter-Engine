// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <initializer_list>

export module jpt.StaticArray;

export namespace jpt
{
	template<typename _TData, size_t kSize>
	class StaticArray
	{
	public:
		using TData = _TData;

	private:
		TData m_buffer[kSize];

	public:
		constexpr StaticArray() = default;
		constexpr StaticArray(const std::initializer_list<TData>& initializerList);
	};

	template<typename TData, size_t kSize>
	constexpr StaticArray<TData, kSize>::StaticArray(const std::initializer_list<TData>& initializerList)
	{
		JPT_ASSERT(initializerList.size() == kSize, "Initializer list size doesn't match");


	}
}