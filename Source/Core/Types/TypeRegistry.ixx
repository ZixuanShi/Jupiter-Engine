// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <atomic>

export module jpt.TypeRegistry;

import jpt.TypeDefs;

export namespace jpt
{
	class TypeRegistry
	{
	public:
		using TypeId = size_t;

	private:
		static std::atomic<TypeId> s_typeIdCounter;

	public:
		template<typename T>
		static TypeId Id()
		{
			static const TypeId typeId = s_typeIdCounter.fetch_add(1, std::memory_order_relaxed);
			return typeId;
		}

		static TypeId GetTypeCount()
		{
			return s_typeIdCounter.load(std::memory_order_relaxed);
		}
	};

	std::atomic<TypeRegistry::TypeId> TypeRegistry::s_typeIdCounter = 0;
}