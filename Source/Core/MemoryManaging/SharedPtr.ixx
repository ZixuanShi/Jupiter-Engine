// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Macros.h"

export module jpt.SharedPtr;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt_private.Deleter;

export namespace jpt
{
	/** Retains shared ownership of an object through a pointer. Several shared_ptr objects may own the same object. The object is destroyed and its memory deallocated when either of the following happens: 
		- the last remaining shared_ptr owning the object is destroyed;
		- the last remaining shared_ptr owning the object is assigned another pointer via operator= or Reset(). */
	template<typename DataT>
	class SharedPtr
	{
	private:
		DataT* m_pPtr = nullptr;
		int32* m_pRefCount = nullptr;

	public:
		constexpr SharedPtr() noexcept = default;
		explicit SharedPtr(DataT* pPtr) noexcept;
		SharedPtr(const SharedPtr& other);
		SharedPtr(SharedPtr&& other) noexcept;
		SharedPtr& operator=(const SharedPtr& other);
		SharedPtr& operator=(SharedPtr&& other) noexcept;
		~SharedPtr();

		/** Replaces the managed object with the new pPtr */
		template<typename DeleterT = jpt_private::DefaultDelete<DataT>>
		void Reset(DataT* pPtr = nullptr, DeleterT deleter = DeleterT());

		/** @return		Pointer to the managed object or nullptr if no object is owned */
		DataT* Get() const noexcept { return m_pPtr; }

		/** @returns    number of SharedPtr objects referring to the same managed object */
		int32 GetRefCount() const { return *m_pRefCount; }

		/** @return		Reference or pointer to the managed object */
		constexpr DataT& operator*() const noexcept { return *Get(); }
		constexpr DataT* operator->() const noexcept { return Get(); }

		/** @return		true if *this owns an object, false otherwise */
		constexpr bool IsValid() const noexcept { return m_pPtr != nullptr; }
		constexpr operator bool() const noexcept { return IsValid(); }
	};

	template<typename DataT, class... Args>
	[[nodiscard]] SharedPtr<DataT> MakeShared(Args&&... args)
	{
		return SharedPtr<DataT>(new DataT(jpt::Forward<Args>(args)...));
	}

	template<typename DataT>
	SharedPtr<DataT>::SharedPtr(DataT* pPtr) noexcept
		: m_pPtr(pPtr)
		, m_pRefCount(new int32(1))
	{
	}

	template<typename DataT>
	SharedPtr<DataT>::SharedPtr(const SharedPtr& other)
		: m_pPtr(other.m_pPtr)
		, m_pRefCount(other.m_pRefCount)
	{
		++*m_pRefCount;
	}

	template<typename DataT>
	SharedPtr<DataT>::SharedPtr(SharedPtr&& other) noexcept
		: m_pPtr(other.m_pPtr)
		, m_pRefCount(other.m_pRefCount)
	{
		other.m_pPtr = nullptr;
		other.m_pRefCount = nullptr;
	}

	template<typename DataT>
	SharedPtr<DataT>& SharedPtr<DataT>::operator=(const SharedPtr& other)
	{
		if (this != &other)
		{
			m_pPtr = other.m_pPtr;
			m_pRefCount = other.m_pRefCount;
			++*m_pRefCount;
		}

		return *this;
	}

	template<typename DataT>
	SharedPtr<DataT>& SharedPtr<DataT>::operator=(SharedPtr&& other) noexcept
	{
		if (this != &other)
		{
			m_pPtr = other.m_pPtr;
			m_pRefCount = other.m_pRefCount;
			++*m_pRefCount;

			other.m_pPtr = nullptr;
			other.m_pRefCount = nullptr;
		}

		return *this;
	}

	template<typename DataT>
	SharedPtr<DataT>::~SharedPtr()
	{
		Reset(nullptr);
	}

	template<typename DataT>
	template<typename DeleterT>
	void SharedPtr<DataT>::Reset(DataT* pPtr, DeleterT deleter)
	{
		// If the old pointer was non-empty, deletes the previously managed object
		if (m_pPtr != pPtr)
		{
			--*m_pRefCount;
			if (*m_pRefCount < 1)
			{
				JPT_DELETE(m_pRefCount);
				deleter(m_pPtr);
			}

			m_pPtr = pPtr;

			if (m_pPtr)
			{
				m_pRefCount = new int32(1);
			}
		}
	}
}