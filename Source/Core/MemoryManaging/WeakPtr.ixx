// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"

export module jpt.WeakPtr;

import jpt.TypeDefs;
import jpt_private.ReferenceCounter;

export namespace jpt
{
	template<typename TData>
	class StrongPtr;

	/** Holds a non-owning ("weak") reference to an object that is managed by jpt::StrongPtr.
		It must be converted to jpt::StrongPtr in order to access the referenced object */
	template<typename TData>
	class WeakPtr
	{
		friend class StrongPtr<TData>;

	private:
		TData* m_pPtr = nullptr;
		jpt_private::ReferenceCounter* m_pRefCounter = nullptr;

	public:
		constexpr WeakPtr() noexcept = default;
		constexpr WeakPtr(const WeakPtr& other);
		constexpr WeakPtr(WeakPtr&& other) noexcept;
		constexpr WeakPtr(const StrongPtr<TData>& shared);
		WeakPtr& operator=(const WeakPtr& other);
		WeakPtr& operator=(const StrongPtr<TData>& shared);
		WeakPtr& operator=(WeakPtr&& other) noexcept;
		constexpr ~WeakPtr();

		/** Releases the ownership of the managed object */
		constexpr void Reset() { InternalReset(nullptr); }

		/** @return		number of StrongPtr objects referring to the same managed object */
		constexpr int32 GetRefCount() const;

		/** @return		true if the managed object has already been deleted, false otherwise. */
		constexpr bool IsExpired() const;

		/** @return		Object Ptr if this is not Expired, nullptr otherwise */
		constexpr TData* GetIfValid() const;

		/** @return		Reference or pointer to the managed object if not expired */
		constexpr TData& operator*() const noexcept  { return *GetIfValid(); }
		constexpr TData* operator->() const noexcept { return GetIfValid(); }

	private:
		constexpr void InternalReset(TData* pPtr);
		constexpr void IncrementWeakRef();
	};

	template<typename TData>
	constexpr WeakPtr<TData>::WeakPtr(const WeakPtr& other)
		: m_pPtr(other.m_pPtr)
		, m_pRefCounter(other.m_pRefCounter)
	{
		IncrementWeakRef();
	}

	template<typename TData>
	constexpr WeakPtr<TData>::WeakPtr(WeakPtr&& other) noexcept
		: m_pPtr(other.m_pPtr)
		, m_pRefCounter(other.m_pRefCounter)
	{
		other.m_pPtr = nullptr;
		other.m_pRefCounter = nullptr;
	}

	template<typename TData>
	WeakPtr<TData>& WeakPtr<TData>::operator=(const WeakPtr& other)
	{
		if (this != &other)
		{
			InternalReset(other.m_pPtr);
			m_pRefCounter = other.m_pRefCounter;
			IncrementWeakRef();
		}

		return *this;
	}

	template<typename TData>
	WeakPtr<TData>& WeakPtr<TData>::operator=(const StrongPtr<TData>& shared)
	{
		InternalReset(shared.m_pPtr);
		m_pRefCounter = shared.m_pRefCounter;
		IncrementWeakRef();

		return *this;
	}

	template<typename TData>
	WeakPtr<TData>& WeakPtr<TData>::operator=(WeakPtr&& other) noexcept
	{
		if (this != &other)
		{
			InternalReset(other.m_pPtr);
			m_pRefCounter = other.m_pRefCounter;

			other.m_pPtr = nullptr;
			other.m_pRefCounter = nullptr;
		}

		return *this;
	}

	template<typename TData>
	constexpr WeakPtr<TData>::WeakPtr(const StrongPtr<TData>& shared)
		: m_pPtr(shared.m_pPtr)
		, m_pRefCounter(shared.m_pRefCount)
	{
		IncrementWeakRef();
	}

	template<typename TData>
	constexpr WeakPtr<TData>::~WeakPtr()
	{
		InternalReset(nullptr);
	}

	template<typename TData>
	constexpr int32 WeakPtr<TData>::GetRefCount() const
	{
		if (m_pRefCounter)
		{
			return m_pRefCounter->GetStrongRefs();
		}

		return 0;
	}

	template<typename TData>
	constexpr bool WeakPtr<TData>::IsExpired() const
	{
		if (m_pRefCounter)
		{
			return GetRefCount() == 0;
		}

		return true;
	}

	template<typename TData>
	constexpr TData* WeakPtr<TData>::GetIfValid() const
	{
		if (!IsExpired())
		{
			return m_pPtr;
		}

		return nullptr;
	}

	template<typename TData>
	constexpr void WeakPtr<TData>::InternalReset(TData* pPtr)
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

	template<typename TData>
	constexpr void WeakPtr<TData>::IncrementWeakRef()
	{
		if (m_pRefCounter)
		{
			m_pRefCounter->IncrementWeakRef();
		}
	}
}