// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <type_traits>

export module jpt.Object.Pool;

import jpt.Allocator;
import jpt.Constants;
import jpt.TypeDefs;
import jpt.Pair;
import jpt.Mutex;
import jpt.LockGuard;
import jpt.DynamicArray;

export namespace jpt
{
    /** Pre-Allocate a pool of objects. 
        @example: 
            jpt::ObjectPool<jpt::String> stringPool;
            
            stringPool.Init(8);
            
            auto handle = stringPool.Acquire();
            (*handle.first) = "Hello, World!";

            JPT_LOG(*handle.first);

            stringPool.Release(handle.second);

            stringPool.Shutdown(); */
    template<typename TObject>
    class ObjectPool
    {
    public:
        using TRaw   = std::remove_pointer_t<TObject>;
        using Handle = Pair<TRaw*, const Index>;

    private:
        struct ObjectData
        {
            TObject instance;
            Index nextFreeIndex = kInvalidValue<Index>;
            bool inUse = false;
        };

    private:
        Mutex m_mutex;
        DynamicArray<ObjectData> m_objects;
        Index m_firstFreeIndex = kInvalidValue<Index>;
        size_t m_usedCount = 0;

    public:
        void Init(size_t count);
        void Shutdown();

        Handle Acquire();
        void Release(Index index);

        TRaw* operator[](Index index);
        const TRaw* operator[](Index index) const;

        bool IsFull() const;
        size_t UsedCount() const;
    };

    template<typename TObject>
    void ObjectPool<TObject>::Init(size_t count)
    {
        m_objects.Resize(count);

        for (Index i = 0; i < count; ++i)
        {
            m_objects[i].nextFreeIndex = i + 1;
        }
        m_objects[count - 1].nextFreeIndex = kInvalidValue<Index>;
        m_firstFreeIndex = 0;

        if constexpr (std::is_pointer_v<TObject>)
        {
            for (ObjectData& objectData : m_objects)
            {
                objectData.instance = Allocator<TRaw>::Allocate();
            }
        }
        else
        {
            for (ObjectData& objectData : m_objects)
            {
                objectData.instance = TObject();
            }
        }
    }

    template<typename TObject>
    void ObjectPool<TObject>::Shutdown()
    {
        if constexpr (std::is_pointer_v<TObject>)
        {
            for (ObjectData& objectData : m_objects)
            {
                delete objectData.instance;
                objectData.instance = nullptr;
            }
        }

        m_usedCount = 0;
        m_firstFreeIndex = kInvalidValue<Index>;
        m_objects.Clear();
    }

    template<typename TObject>
    ObjectPool<TObject>::Handle ObjectPool<TObject>::Acquire()
    {
        LockGuard lock(m_mutex);

        JPT_ASSERT(!IsFull(), "ObjectPool is full");
        JPT_ASSERT(m_firstFreeIndex != kInvalidValue<Index>, "No free index available");

        // Acquire the first free index
        const Index acquiredIndex = m_firstFreeIndex;
        ObjectData& objectData = m_objects[acquiredIndex];

        // Update the first free index
        m_firstFreeIndex = objectData.nextFreeIndex;

        objectData.inUse = true;
        objectData.nextFreeIndex = kInvalidValue<Index>;
        ++m_usedCount;

        if constexpr (std::is_pointer_v<TObject>)
        {
            return { objectData.instance, acquiredIndex };
        }
        else
        {
            return { &objectData.instance, acquiredIndex };
        }
    }

    template<typename TObject>
    void ObjectPool<TObject>::Release(Index index)
    {
        LockGuard lock(m_mutex);

        JPT_ASSERT(m_usedCount > 0, "ObjectPool is empty");
        JPT_ASSERT(index < m_objects.Count(), "Index out of range");

        ObjectData& objectData = m_objects[index];
        JPT_ASSERT(objectData.inUse, "Object is not in use");
        JPT_ASSERT(objectData.nextFreeIndex == kInvalidValue<Index>, "Object is already free");

        // Add the index to the free list
        objectData.nextFreeIndex = m_firstFreeIndex;
        m_firstFreeIndex = index;

        --m_usedCount;
        objectData.inUse = false;
    }

    template<typename TObject>
    typename ObjectPool<TObject>::TRaw* ObjectPool<TObject>::operator[](Index index)
    {
        return m_objects[index].instance;
    }

    template<typename TObject>
    const typename ObjectPool<TObject>::TRaw* ObjectPool<TObject>::operator[](Index index) const
    {
        return m_objects[index].instance;
    }

    template<typename TObject>
    bool ObjectPool<TObject>::IsFull() const
    {
        return m_usedCount == m_objects.Count();
    }

    template<typename TObject>
    size_t ObjectPool<TObject>::UsedCount() const
    {
        return m_usedCount;
    }
}