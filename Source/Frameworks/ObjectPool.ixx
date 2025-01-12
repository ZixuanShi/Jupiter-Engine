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
	/** Pre-Allocate a pool of objects. */
	template<typename TObject>
	class ObjectPool
	{
	private:
		struct ObjectData
		{
			TObject pObject;
			bool inUse = false;
		};

	private:
		Mutex m_mutex;
		DynamicArray<ObjectData> m_objects;
		size_t m_usedCount = 0;

	public:
		void Init(size_t count);
		void Shutdown();

		Pair<std::remove_pointer_t<TObject>*, Index> Acquire();
		void Release(Index index);

		std::remove_pointer_t<TObject>* operator[](Index index) { return m_objects[index].pObject; }
		const std::remove_pointer_t<TObject>* operator[](Index index) const { return m_objects[index].pObject; }

		bool IsFull() const { return m_usedCount == m_objects.Count(); }
		size_t UsedCount() const { return m_usedCount; }
	};

	template<typename TObject>
	void ObjectPool<TObject>::Init(size_t count)
	{
		m_objects.Resize(count);

		if constexpr (std::is_pointer_v<TObject>)
		{
			using T = std::remove_pointer_t<TObject>;
			for (ObjectData& objectData : m_objects)
			{
				objectData.pObject = Allocator<T>::Allocate();
			}
		}
		else
		{
			for (ObjectData& objectData : m_objects)
			{
				objectData.pObject = TObject();
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
				delete objectData.pObject;
				objectData.pObject = nullptr;
			}
		}

		m_usedCount = 0;
		m_objects.Clear();
	}

	template<typename TObject>
	Pair<std::remove_pointer_t<TObject>*, Index> ObjectPool<TObject>::Acquire()
	{
		LockGuard lock(m_mutex);

		JPT_ASSERT(!IsFull(), "ObjectPool is full");

		for (Index i = 0; i < m_objects.Count(); ++i)
		{
			ObjectData& objectData = m_objects[i];
			if (!objectData.inUse)
			{
				++m_usedCount;
				objectData.inUse = true;

				if constexpr (std::is_pointer_v<TObject>)
				{
					return { objectData.pObject, i };
				}
				else
				{
					return { &objectData.pObject, i };
				}
			}
		}

		return { nullptr, kInvalidValue<Index> };
	}

	template<typename TObject>
	void ObjectPool<TObject>::Release(Index index)
	{
		LockGuard lock(m_mutex);

		JPT_ASSERT(m_usedCount > 0, "ObjectPool is empty");
		JPT_ASSERT(index < m_objects.Count(), "Index out of range");

		ObjectData& objectData = m_objects[index];
		JPT_ASSERT(objectData.inUse, "Object is not in use");

		--m_usedCount;
		objectData.inUse = false;
	}
}