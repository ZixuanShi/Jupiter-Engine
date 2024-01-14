// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Macros.h"

export module jpt.WeakPtr;

import jpt.TypeDefs;
import jpt_private.ReferenceCounter;

export namespace jpt
{
	template<typename DataT>
	class StrongPtr;

	/** Holds a non-owning ("weak") reference to an object that is managed by jpt::StrongPtr.
		It must be converted to jpt::StrongPtr in order to access the referenced object */
	template<typename DataT>
	class WeakPtr
	{
	private:
		DataT* m_pPtr = nullptr;
		jpt_private::ReferenceCounter* m_pRefCount = nullptr;

	public:
		constexpr WeakPtr() noexcept = default;
		WeakPtr(const WeakPtr& other);
		WeakPtr(WeakPtr&& other) noexcept;
		WeakPtr(const StrongPtr<DataT>& shared);
		WeakPtr& operator=(const WeakPtr& other);
		WeakPtr& operator=(const StrongPtr<DataT>& shared);
		WeakPtr& operator=(WeakPtr&& other) noexcept;
		~WeakPtr();

		/** Releases the ownership of the managed object */
		void Reset();

		/** @return		number of StrongPtr objects referring to the same managed object */
		int32 GetRefCount() const { return m_pRefCount ? m_pRefCount->GetSharedRefs() : 0; }

		/** @return		true if the managed object has already been deleted, false otherwise. */
		bool IsExpired() const { return m_pRefCount ? GetRefCount() == 0 : true; }

		/** @return		a shared_ptr that manages the referenced object */
		StrongPtr<DataT> Lock() const;
	};

	template<typename DataT>
	WeakPtr<DataT>::WeakPtr(const WeakPtr& other)
		: m_pPtr(other.m_pPtr)
		, m_pRefCount(other.m_pRefCount)
	{
		if (m_pRefCount)
		{
			m_pRefCount->IncrementWeakRef();
		}
	}

	template<typename DataT>
	WeakPtr<DataT>::WeakPtr(WeakPtr&& other) noexcept
		: m_pPtr(other.m_pPtr)
		, m_pRefCount(other.m_pRefCount)
	{
		other.m_pPtr = nullptr;
		other.m_pRefCount = nullptr;
	}

	template<typename DataT>
	WeakPtr<DataT>& WeakPtr<DataT>::operator=(const WeakPtr& other)
	{
		if (this != &other)
		{
			Reset();

			m_pPtr = other.m_pPtr;
			m_pRefCount = other.m_pRefCount;

			if (m_pPtr)
			{
				m_pRefCount->IncrementWeakRef();
			}
		}

		return *this;
	}

	template<typename DataT>
	WeakPtr<DataT>& WeakPtr<DataT>::operator=(const StrongPtr<DataT>& shared)
	{
		Reset();

		m_pPtr = shared.m_pPtr;
		m_pRefCount = shared.m_pRefCount;

		if (m_pRefCount)
		{
			m_pRefCount->IncrementWeakRef();
		}

		return *this;
	}

	template<typename DataT>
	WeakPtr<DataT>& WeakPtr<DataT>::operator=(WeakPtr&& other) noexcept
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
	WeakPtr<DataT>::WeakPtr(const StrongPtr<DataT>& shared)
		: m_pPtr(shared.m_pPtr)
		, m_pRefCount(shared.m_pRefCount)
	{
		if (m_pRefCount)
		{
			m_pRefCount->IncrementWeakRef();
		}
	}

	template<typename DataT>
	WeakPtr<DataT>::~WeakPtr()
	{
		Reset();
	}

	template<typename DataT>
	void WeakPtr<DataT>::Reset()
	{
		m_pPtr = nullptr;
		
		if (m_pRefCount)
		{
			m_pRefCount->DecrementWeakRef();
			if (!m_pRefCount->HasAnyRef())
			{
				JPT_DELETE(m_pRefCount);
			}
		}
	}

	template<typename DataT>
	StrongPtr<DataT> WeakPtr<DataT>::Lock() const
	{
		if (m_pPtr && m_pRefCount)
		{
			m_pRefCount->IncrementStrongRef();

			StrongPtr<DataT> shared;
			shared.m_pPtr = m_pPtr;
			shared.m_pRefCount = m_pRefCount;
			return shared;
		}

		return StrongPtr<DataT>();
	}
}