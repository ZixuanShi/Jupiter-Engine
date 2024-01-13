// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.WeakPtr;

import jpt.TypeDefs;

export namespace jpt
{
	template<typename DataT>
	class SharedPtr;

	/** Holds a non-owning ("weak") reference to an object that is managed by jpt::SharedPtr.
		It must be converted to jpt::SharedPtr in order to access the referenced object */
	template<typename DataT>
	class WeakPtr
	{
	private:
		DataT* m_pPtr = nullptr;
		int32* m_pRefCount = nullptr;

	public:
		constexpr WeakPtr() noexcept = default;
		WeakPtr(const WeakPtr& other);
		WeakPtr(WeakPtr&& other) noexcept;
		WeakPtr& operator=(const WeakPtr& other);
		WeakPtr& operator=(WeakPtr&& other) noexcept;
		WeakPtr(const SharedPtr<DataT>& shared);
		~WeakPtr();

		/** Releases the ownership of the managed object */
		void Reset();

		/** @return		number of SharedPtr objects referring to the same managed object */
		int32 GetRefCount() const { return m_pRefCount ? *m_pRefCount : 0; }

		/** @return		true if the managed object has already been deleted, false otherwise. */
		bool IsExpired() const { return m_pRefCount ? GetRefCount() == 0 : true; }

		/** @return		a shared_ptr that manages the referenced object */
		SharedPtr<DataT> Lock() const;
	};

	template<typename DataT>
	WeakPtr<DataT>::WeakPtr(const WeakPtr& other)
		: m_pPtr(other.m_pPtr)
		, m_pRefCount(other.m_pRefCount)
	{
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
			m_pPtr = other.m_pPtr;
			m_pRefCount = other.m_pRefCount;
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
	WeakPtr<DataT>::WeakPtr(const SharedPtr<DataT>& shared)
		: m_pPtr(shared.m_pPtr)
		, m_pRefCount(shared.m_pRefCount)
	{
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
		m_pRefCount = nullptr;
	}

	template<typename DataT>
	SharedPtr<DataT> WeakPtr<DataT>::Lock() const
	{
		++*m_pRefCount;

		SharedPtr<DataT> shared;
		shared.m_pPtr = m_pPtr;
		shared.m_pRefCount = m_pRefCount;

		return shared;
	}
}