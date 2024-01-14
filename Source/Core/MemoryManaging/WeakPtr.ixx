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
		jpt_private::ReferenceCounter* m_pRefCounter = nullptr;

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
		void Reset() { Reset(nullptr); }

		/** @return		number of StrongPtr objects referring to the same managed object */
		int32 GetRefCount() const { return m_pRefCounter ? m_pRefCounter->GetSharedRefs() : 0; }

		/** @return		true if the managed object has already been deleted, false otherwise. */
		bool IsExpired() const { return m_pRefCounter ? GetRefCount() == 0 : true; }

		/** @return		a shared_ptr that manages the referenced object */
		StrongPtr<DataT> Lock() const;

	private:
		void Reset(DataT* pPtr);
		void IncrementWeakRef();
	};

	template<typename DataT>
	WeakPtr<DataT>::WeakPtr(const WeakPtr& other)
		: m_pPtr(other.m_pPtr)
		, m_pRefCounter(other.m_pRefCounter)
	{
		IncrementWeakRef();
	}

	template<typename DataT>
	WeakPtr<DataT>::WeakPtr(WeakPtr&& other) noexcept
		: m_pPtr(other.m_pPtr)
		, m_pRefCounter(other.m_pRefCounter)
	{
		other.m_pPtr = nullptr;
		other.m_pRefCounter = nullptr;
	}

	template<typename DataT>
	WeakPtr<DataT>& WeakPtr<DataT>::operator=(const WeakPtr& other)
	{
		if (this != &other)
		{
			Reset(other.m_pPtr);
			m_pRefCounter = other.m_pRefCounter;
			IncrementWeakRef();
		}

		return *this;
	}

	template<typename DataT>
	WeakPtr<DataT>& WeakPtr<DataT>::operator=(const StrongPtr<DataT>& shared)
	{
		Reset(shared.m_pPtr);
		m_pRefCounter = shared.m_pRefCounter;
		IncrementWeakRef();

		return *this;
	}

	template<typename DataT>
	WeakPtr<DataT>& WeakPtr<DataT>::operator=(WeakPtr&& other) noexcept
	{
		if (this != &other)
		{
			Reset(other.m_pPtr);
			m_pRefCounter = other.m_pRefCounter;

			other.m_pPtr = nullptr;
			other.m_pRefCounter = nullptr;
		}

		return *this;
	}

	template<typename DataT>
	WeakPtr<DataT>::WeakPtr(const StrongPtr<DataT>& shared)
		: m_pPtr(shared.m_pPtr)
		, m_pRefCounter(shared.m_pRefCount)
	{
		IncrementWeakRef();
	}

	template<typename DataT>
	WeakPtr<DataT>::~WeakPtr()
	{
		Reset(nullptr);
	}

	template<typename DataT>
	StrongPtr<DataT> WeakPtr<DataT>::Lock() const
	{
		if (m_pPtr && m_pRefCounter)
		{
			m_pRefCounter->IncrementStrongRef();

			StrongPtr<DataT> shared;
			shared.m_pPtr = m_pPtr;
			shared.m_pRefCounter = m_pRefCounter;
			return shared;
		}

		return StrongPtr<DataT>();
	}

	template<typename DataT>
	void WeakPtr<DataT>::Reset(DataT* pPtr)
	{
		m_pPtr = pPtr;

		if (m_pRefCounter)
		{
			m_pRefCounter->DecrementWeakRef();
			if (!m_pRefCounter->HasAnyRef())
			{
				JPT_DELETE(m_pRefCounter);
			}
		}
	}

	template<typename DataT>
	void WeakPtr<DataT>::IncrementWeakRef()
	{
		if (m_pRefCounter)
		{
			m_pRefCounter->IncrementWeakRef();
		}
	}
}