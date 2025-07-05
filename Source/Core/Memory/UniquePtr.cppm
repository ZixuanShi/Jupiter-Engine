// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.UniquePtr;

import jpt.Allocator;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt_private.Deleter;

export namespace jpt
{
    /** Owns and manages another object through a pointer and disposes of that object when the unique_ptr goes out of scope. */
    export template<typename TData, class TDeleter = jpt_private::DefaultDelete<TData>>
    class UniquePtr
    {
    private:
        TData* m_pPtr = nullptr;  /**< The pointer this UniquePtr is going to guard */
        TDeleter m_deleter;          /**< The deleter used to destroy managed object, default to jpt_private::DefaultDelete<TData> */

    public:
        constexpr UniquePtr() noexcept = default;
        constexpr UniquePtr(TData* pPtr) noexcept;
        constexpr UniquePtr(TData* pPtr, const TDeleter& deleter) noexcept;
        constexpr UniquePtr(UniquePtr&& other) noexcept;
        UniquePtr& operator=(UniquePtr<TData, TDeleter>&& other) noexcept;
        constexpr ~UniquePtr();

        constexpr UniquePtr(const UniquePtr&) = delete;
        UniquePtr& operator=(const UniquePtr&) = delete;

        /** Replaces the managed object with the given pPtr */
        constexpr void Reset(TData* pPtr = nullptr);

        /** Releases the ownership of the managed object, if any
            @return        Pointer to the managed object or nullptr if there was no managed object 
            @note        The caller is responsible for cleaning up the object (e.g. by use of get_deleter()).*/
        constexpr TData* Release() noexcept;

        /** @return        The stored deleter object */
        constexpr TDeleter& GetDeleter() noexcept { return m_deleter; }
        constexpr const TDeleter& GetDeleter() const noexcept { return m_deleter; }

        /** @return        Reference or pointer to the managed object */
        constexpr TData& operator*()  const noexcept { return *m_pPtr; }
        constexpr TData* operator->() const noexcept { return  m_pPtr; }
        constexpr TData* Get()        const noexcept { return  m_pPtr; }

        /** @return        true if *this owns an object, false otherwise */
        constexpr bool IsValid()  const noexcept { return m_pPtr != nullptr; }
        constexpr operator bool() const noexcept { return IsValid(); }
    };

    /** Constructs a non-array type T. The arguments args are passed to the constructor of T. 
        This overload participates in overload resolution only if T is not an array type. */
    template<typename TData, class... TArgs>
    [[nodiscard]] constexpr UniquePtr<TData> MakeUnique(TArgs&&...args)
    {
        TData* pPtr = Allocator<TData>::New(Forward<TArgs>(args)...);
        return UniquePtr<TData>(pPtr);
    }

    template<class TData1, class DleterT1, class TData2, class DleterT2 >
    constexpr bool operator==(const UniquePtr<TData1, DleterT1>& a, const UniquePtr<TData2, DleterT2>& b)
    {
        return a.Get() == b.Get();
    }

    template<typename TData, class TDeleter>
    constexpr UniquePtr<TData, TDeleter>::UniquePtr(TData* pPtr) noexcept
        : m_pPtr(pPtr)
    {
    }

    template<typename TData, class TDeleter>
    constexpr UniquePtr<TData, TDeleter>::UniquePtr(TData* pPtr, const TDeleter& deleter) noexcept
        : m_pPtr(pPtr)
        , m_deleter(deleter)
    {
    }

    template<typename TData, class TDeleter>
    constexpr UniquePtr<TData, TDeleter>::UniquePtr(UniquePtr<TData, TDeleter>&& other) noexcept
        : m_pPtr(other.m_pPtr)
        , m_deleter(Move(other.GetDeleter()))
    {
    }

    template<typename TData, class TDeleter>
    UniquePtr<TData, TDeleter>& UniquePtr<TData, TDeleter>::operator=(UniquePtr<TData, TDeleter>&& other) noexcept
    {
        if (this != &other)
        {
            Reset(other.m_pPtr);
            other.m_pPtr = nullptr;
            m_deleter = Move(other.GetDeleter());
        }

        return *this;
    }

    template<typename TData, class TDeleter>
    constexpr UniquePtr<TData, TDeleter>::~UniquePtr()
    {
        Reset();
    }

    template<typename TData, class TDeleter>
    constexpr void UniquePtr<TData, TDeleter>::Reset(TData* pPtr /*= nullptr*/)
    {
        // If the old pointer was non-empty, deletes the previously managed object
        if (m_pPtr != pPtr)
        {
            m_deleter(m_pPtr);
            m_pPtr = pPtr;
        }
    }

    template<typename TData, class TDeleter>
    constexpr TData* UniquePtr<TData, TDeleter>::Release() noexcept
    {
        TData* dataPtr = m_pPtr;
        m_pPtr = nullptr;
        return dataPtr;
    }

    export template<typename TData, typename TDeleter>
        class UniquePtr<TData[], TDeleter>
    {
    private:
        TData* m_pPtr = nullptr;  /**< The pointer this UniquePtr is going to guard */
        TDeleter m_deleter;          /**< The deleter used to destroy managed object, default to jpt_private::DefaultDelete<TData> */

    public:
        constexpr UniquePtr() noexcept = default;
        constexpr UniquePtr(TData* pPtr) noexcept : m_pPtr(pPtr) {}
        constexpr UniquePtr(TData* pPtr, const TDeleter& deleter) noexcept : m_pPtr(pPtr), m_deleter(deleter) {}
        constexpr UniquePtr(UniquePtr&& uniquePtr) noexcept : m_pPtr(uniquePtr.Get()), m_deleter(Move(uniquePtr.GetDeleter())) {}    
        UniquePtr& operator=(UniquePtr<TData, TDeleter>&& other) noexcept
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

        /** Replaces the managed object with the given pPtr */
        constexpr void Reset(TData* pPtr = nullptr)
        {
            // If the old pointer was non-empty, deletes the previously managed object
            if (m_pPtr != pPtr)
            {
                m_deleter(m_pPtr);
                m_pPtr = pPtr;
            }
        }

        /** Releases the ownership of the managed object, if any
            @return        Pointer to the managed object or nullptr if there was no managed object
            @note        The caller is responsible for cleaning up the object (e.g. by use of get_deleter()).*/
        constexpr TData* Release() noexcept
        {
            TData* dataPtr = m_pPtr;
            m_pPtr = nullptr;
            return dataPtr;
        }

        /** @return        The stored deleter object */
        constexpr TDeleter& GetDeleter() noexcept { return m_deleter; }
        constexpr const TDeleter& GetDeleter() const noexcept { return m_deleter; }

        /** @return        Pointer to the managed object or nullptr if no object is owned */
        constexpr TData* Get() const noexcept { return m_pPtr; }

        /** @return        Reference or pointer to the managed object */
        constexpr TData& operator*() const noexcept { return *Get(); }
        constexpr TData* operator->() const noexcept { return Get(); }

        /** @return        true if *this owns an object, false otherwise */
        constexpr bool IsValid() const noexcept { return m_pPtr != nullptr; }
        constexpr operator bool() const noexcept { return IsValid(); }
    };
}