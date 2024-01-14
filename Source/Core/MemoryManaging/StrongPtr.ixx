// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Macros.h"

export module jpt.StrongPtr;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt_private.Deleter;
import jpt_private.ReferenceCounter;

namespace jpt
{
	template<typename DataT>
	class WeakPtr;

	/** Retains shared ownership of an object through a pointer. Several StrongPtr objects may own the same object. The object is destroyed and its memory deallocated when either of the following happens: 
		- the last remaining StrongPtr owning the object is destroyed;
		- the last remaining StrongPtr owning the object is assigned another pointer via operator= or Reset(). */
	export template<typename DataT>
	class StrongPtr
	{
		friend class WeakPtr<DataT>;

	private:
		DataT* m_pPtr = nullptr;
		jpt_private::ReferenceCounter* m_pRefCount = nullptr;

	public:
		constexpr StrongPtr() noexcept = default;
		explicit StrongPtr(DataT* pPtr) noexcept;
		StrongPtr(const StrongPtr& other);
		StrongPtr(StrongPtr&& other) noexcept;
		StrongPtr& operator=(const StrongPtr& other);
		StrongPtr& operator=(StrongPtr&& other) noexcept;
		~StrongPtr();

		/** Replaces the managed object with the new pPtr */
		template<typename DeleterT = jpt_private::DefaultDelete<DataT>>
		void Reset(DataT* pPtr = nullptr, const DeleterT& deleter = DeleterT());

		/** @return		Pointer to the managed object or nullptr if no object is owned */
		DataT* Get() const noexcept { return m_pPtr; }

		/** @returns    number of StrongPtr objects referring to the same managed object */
		int32 GetRefCount() const { return m_pRefCount ? m_pRefCount->GetSharedRefs() : 0; }

		/** @return		Reference or pointer to the managed object */
		constexpr DataT& operator*() const noexcept { return *Get(); }
		constexpr DataT* operator->() const noexcept { return Get(); }

		/** @return		true if *this owns an object, false otherwise */
		constexpr bool IsValid() const noexcept { return m_pPtr != nullptr; }
		constexpr operator bool() const noexcept { return IsValid(); }
	};

	export template<typename DataT, class... Args>
	[[nodiscard]] StrongPtr<DataT> MakeStrong(Args&&... args)
	{
		return StrongPtr<DataT>(new DataT(jpt::Forward<Args>(args)...));
	}

	template<typename DataT>
	StrongPtr<DataT>::StrongPtr(DataT* pPtr) noexcept
		: m_pPtr(pPtr)
		, m_pRefCount(new jpt_private::ReferenceCounter(1, 0))
	{
	}

	template<typename DataT>
	StrongPtr<DataT>::StrongPtr(const StrongPtr& other)
		: m_pPtr(other.m_pPtr)
		, m_pRefCount(other.m_pRefCount)
	{
		if (m_pRefCount)
		{
			m_pRefCount->IncrementStrongRef();
		}
	}

	template<typename DataT>
	StrongPtr<DataT>::StrongPtr(StrongPtr&& other) noexcept
		: m_pPtr(other.m_pPtr)
		, m_pRefCount(other.m_pRefCount)
	{
		other.m_pPtr = nullptr;
		other.m_pRefCount = nullptr;
	}

	template<typename DataT>
	StrongPtr<DataT>& StrongPtr<DataT>::operator=(const StrongPtr& other)
	{
		if (this != &other)
		{
			if (m_pRefCount && m_pRefCount->HasAnySharedRef())
			{
				m_pRefCount->DecrementStrongRef();
			}

			m_pPtr = other.m_pPtr;
			m_pRefCount = other.m_pRefCount;

			if (m_pRefCount)
			{
				m_pRefCount->IncrementStrongRef();
			}
		}

		return *this;
	}

	template<typename DataT>
	StrongPtr<DataT>& StrongPtr<DataT>::operator=(StrongPtr&& other) noexcept
	{
		if (this != &other)
		{
			m_pPtr = other.m_pPtr;
			m_pRefCount = other.m_pRefCount;

			other.m_pPtr = nullptr;
			other.m_pRefCount = nullptr;
		}

		return *this;
	}

	template<typename DataT>
	StrongPtr<DataT>::~StrongPtr()
	{
		Reset(nullptr);
	}

	template<typename DataT>
	template<typename DeleterT>
	void StrongPtr<DataT>::Reset(DataT* pPtr, const DeleterT& deleter)
	{
		// If the old pointer was non-empty, deletes the previously managed object
		if (m_pPtr != pPtr)
		{
			m_pRefCount->DecrementStrongRef();

			if (!m_pRefCount->HasAnyRef())
			{
				JPT_DELETE(m_pRefCount);
			}

			if (!m_pRefCount || !m_pRefCount->HasAnySharedRef())
			{
				deleter(m_pPtr);
			}

			m_pPtr = pPtr;

			if (m_pPtr)
			{
				m_pRefCount = new jpt_private::ReferenceCounter(1, 0);
			}
		}
	}
}