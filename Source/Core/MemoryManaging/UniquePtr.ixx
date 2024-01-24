// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.UniquePtr;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt_private.Deleter;

export namespace jpt
{
	/** Owns and manages another object through a pointer and disposes of that object when the unique_ptr goes out of scope. */
	export template<typename DataT, class DeleterT = jpt_private::DefaultDelete<DataT>>
	class UniquePtr
	{
	private:
		DataT* m_pPtr = nullptr;  /**< The pointer this UniquePtr is going to guard */
		DeleterT m_deleter;		  /**< The deleter used to destroy managed object, default to jpt_private::DefaultDelete<DataT> */

	public:
		constexpr UniquePtr() noexcept = default;
		explicit UniquePtr(DataT* pPtr) noexcept;
		constexpr UniquePtr(DataT* pPtr, const DeleterT& deleter) noexcept;
		constexpr UniquePtr(UniquePtr&& other) noexcept;
		UniquePtr& operator=(UniquePtr<DataT, DeleterT>&& other) noexcept;
		~UniquePtr();

		constexpr UniquePtr(const UniquePtr&) = delete;
		UniquePtr& operator=(const UniquePtr&) = delete;

		/** Replaces the managed object with the new pPtr */
		void Reset(DataT* pPtr = nullptr);

		/** Releases the ownership of the managed object, if any
			@return		Pointer to the managed object or nullptr if there was no managed object 
			@note		The caller is responsible for cleaning up the object (e.g. by use of get_deleter()).*/
		DataT* Release() noexcept;

		/** @return		The stored deleter object */
		DeleterT& GetDeleter() noexcept { return m_deleter; }
		const DeleterT& GetDeleter() const noexcept { return m_deleter; }

		/** @return		Reference or pointer to the managed object */
		constexpr DataT& operator*()  const noexcept { return *m_pPtr; }
		constexpr DataT* operator->() const noexcept { return  m_pPtr; }

		/** @return		true if *this owns an object, false otherwise */
		constexpr bool IsValid() const noexcept { return m_pPtr != nullptr; }
		constexpr operator bool() const noexcept { return IsValid(); }
	};

	/** Constructs a non-array type T. The arguments args are passed to the constructor of T. 
		This overload participates in overload resolution only if T is not an array type. 
		The function is equivalent to: UniquePtr<T>(new T(std::forward<Args>(args)...)) */
	template<typename DataT, class... ArgsT>
	[[nodiscard]] constexpr UniquePtr<DataT> MakeUnique(ArgsT&&...args)
	{
		return UniquePtr<DataT>(new DataT(jpt::Forward<ArgsT>(args)...));
	}

	template<class DataT1, class DleterT1, class DataT2, class DleterT2 >
	bool operator==(const UniquePtr<DataT1, DleterT1>& a, const UniquePtr<DataT2, DleterT2>& b)
	{
		return a.Get() == b.Get();
	}

	template<typename DataT, class DeleterT>
	UniquePtr<DataT, DeleterT>::UniquePtr(DataT* pPtr) noexcept
		: m_pPtr(pPtr)
	{
	}

	template<typename DataT, class DeleterT>
	constexpr UniquePtr<DataT, DeleterT>::UniquePtr(DataT* pPtr, const DeleterT& deleter) noexcept
		: m_pPtr(pPtr)
		, m_deleter(deleter)
	{
	}

	template<typename DataT, class DeleterT>
	constexpr UniquePtr<DataT, DeleterT>::UniquePtr(UniquePtr<DataT, DeleterT>&& other) noexcept
		: m_pPtr(other.Get())
		, m_deleter(Move(other.GetDeleter()))
	{
	}

	template<typename DataT, class DeleterT>
	UniquePtr<DataT, DeleterT>& UniquePtr<DataT, DeleterT>::operator=(UniquePtr<DataT, DeleterT>&& other) noexcept
	{
		if (this != &other)
		{
			Reset(other.m_pPtr);
			other.m_pPtr = nullptr;
			m_deleter = Move(other.GetDeleter());
		}

		return *this;
	}

	template<typename DataT, class DeleterT>
	UniquePtr<DataT, DeleterT>::~UniquePtr()
	{
		Reset();
	}

	template<typename DataT, class DeleterT>
	void UniquePtr<DataT, DeleterT>::Reset(DataT* pPtr /*= nullptr*/)
	{
		// If the old pointer was non-empty, deletes the previously managed object
		if (m_pPtr != pPtr)
		{
			m_deleter(m_pPtr);
			m_pPtr = pPtr;
		}
	}

	template<typename DataT, class DeleterT>
	DataT* UniquePtr<DataT, DeleterT>::Release() noexcept
	{
		DataT* dataPtr = m_pPtr;
		m_pPtr = nullptr;
		return dataPtr;
	}

	export template<typename DataT, typename DeleterT>
		class UniquePtr<DataT[], DeleterT>
	{
	private:
		DataT* m_pPtr = nullptr;  /**< The pointer this UniquePtr is going to guard */
		DeleterT m_deleter;		  /**< The deleter used to destroy managed object, default to jpt_private::DefaultDelete<DataT> */

	public:
		constexpr UniquePtr() noexcept = default;
		constexpr UniquePtr(DataT* pPtr) noexcept : m_pPtr(pPtr) {}
		constexpr UniquePtr(DataT* pPtr, const DeleterT& deleter) noexcept : m_pPtr(pPtr), m_deleter(deleter) {}
		constexpr UniquePtr(UniquePtr&& uniquePtr) noexcept : m_pPtr(uniquePtr.Get()), m_deleter(Move(uniquePtr.GetDeleter())) {}	
		UniquePtr& operator=(UniquePtr<DataT, DeleterT>&& other) noexcept
		{
			if (this != &other)
			{
				Reset(other.m_pPtr);
				other.m_pPtr = nullptr;
				m_deleter = Move(other.GetDeleter());
			}

			return *this;
		}
		~UniquePtr() { Reset(); }

		constexpr UniquePtr(const UniquePtr&) = delete;
		UniquePtr& operator=(const UniquePtr&) = delete;

		/** Replaces the managed object with the new pPtr */
		void Reset(DataT* pPtr = nullptr)
		{
			// If the old pointer was non-empty, deletes the previously managed object
			if (m_pPtr != pPtr)
			{
				m_deleter(m_pPtr);
				m_pPtr = pPtr;
			}
		}

		/** Releases the ownership of the managed object, if any
			@return		Pointer to the managed object or nullptr if there was no managed object
			@note		The caller is responsible for cleaning up the object (e.g. by use of get_deleter()).*/
		DataT* Release() noexcept
		{
			DataT* dataPtr = m_pPtr;
			m_pPtr = nullptr;
			return dataPtr;
		}

		/** @return		The stored deleter object */
		DeleterT& GetDeleter() noexcept { return m_deleter; }
		const DeleterT& GetDeleter() const noexcept { return m_deleter; }

		/** @return		Pointer to the managed object or nullptr if no object is owned */
		DataT* Get() const noexcept { return m_pPtr; }

		/** @return		Reference or pointer to the managed object */
		constexpr DataT& operator*() const noexcept { return *Get(); }
		constexpr DataT* operator->() const noexcept { return Get(); }

		/** @return		true if *this owns an object, false otherwise */
		constexpr bool IsValid() const noexcept { return m_pPtr != nullptr; }
		constexpr operator bool() const noexcept { return IsValid(); }
	};
}