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
        static std::atomic<Id> s_typeIdCounter;

    public:
        template<typename T>
        static Id GetId()
        {
            static const Id typeId = s_typeIdCounter.fetch_add(1, std::memory_order_relaxed);
            return typeId;
        }

        static size_t GetTypeCount()
        {
            return s_typeIdCounter.load(std::memory_order_relaxed);
        }
    };

    std::atomic<Id> TypeRegistry::s_typeIdCounter = 0;
}