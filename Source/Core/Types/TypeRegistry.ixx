// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <atomic>

export module jpt.TypeRegistry;

import jpt.TypeDefs;

export namespace jpt
{
	class TypeRegistry
	{
	private:
		static std::atomic<size_t> s_typeIdCounter;

	public:
		template<typename T>
		static size_t Id()
		{
			static const size_t typeId = s_typeIdCounter.fetch_add(1, std::memory_order_relaxed);
			return typeId;
		}
	};

	std::atomic<size_t> TypeRegistry::s_typeIdCounter = 0;
}