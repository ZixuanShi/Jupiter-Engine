// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"

export module jpt.SharedPtr;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt_private.Deleter;
import jpt_private.ReferenceCounter;
import jpt.WeakPtr;

namespace jpt
{
    /** Retains shared ownership of an object through a pointer. Several SharedPtr objects may own the same object. The object is destroyed and its memory deallocated when either of the following happens: 
        - the last remaining SharedPtr owning the object is destroyed;
        - the last remaining SharedPtr owning the object is assigned another pointer via operator= or Reset(). */
    export template<typename TData>
    class SharedPtr
    {
        friend class WeakPtr<TData>;

    private:
        TData* m_pPtr = nullptr;
        jpt_private::ReferenceCounter* m_pRefCounter = nullptr;

    public:
        constexpr SharedPtr() noexcept = default;
        constexpr SharedPtr(TData* pPtr) noexcept;
        constexpr SharedPtr(const SharedPtr& other);
        constexpr SharedPtr(const WeakPtr<TData>& weakPtr);
        constexpr SharedPtr(SharedPtr&& other) noexcept;
        SharedPtr& operator=(const SharedPtr& other);
        SharedPtr& operator=(const WeakPtr<TData>& weakPtr);
        SharedPtr& operator=(SharedPtr&& other) noexcept;
        constexpr ~SharedPtr();

        template <typename TOther>
        SharedPtr(const SharedPtr<TOther>& other) = delete;

        /** Replaces the managed object with the given pPtr */
        template<typename TDeleter = jpt_private::DefaultDelete<TData>>
        constexpr void Reset(TData* pPtr = nullptr, const TDeleter& deleter = TDeleter());

        /** @returns    number of SharedPtr objects referring to the same managed object */
        constexpr int32 GetRefCount() const;

        /** @return        Reference or pointer to the managed object */
        constexpr TData& operator*()  const noexcept { return *m_pPtr; }
        constexpr TData* operator->() const noexcept { return m_pPtr;  }

        /** @return        true if *this owns an object, false otherwise */
        constexpr bool IsValid()  const noexcept { return m_pPtr != nullptr; }
        constexpr operator bool() const noexcept { return IsValid(); }

    private:
        /** Resets this->m_pPtr with the passed in pPtr
            Will destroy Ref counter and m_pPtr object if this is the last SharedPtr holding the data */
        template<typename TDeleter = jpt_private::DefaultDelete<TData>>
        constexpr void InternalReset(TData* pPtr, const TDeleter& deleter = TDeleter());

        constexpr void IncrementStrongRef();
    };

    export template<typename TData, class... TArgs>
    [[nodiscard]] constexpr SharedPtr<TData> MakeStrong(TArgs&&... args)
    {
        return SharedPtr<TData>(new TData(jpt::Forward<TArgs>(args)...));
    }

    template<typename TData>
    constexpr SharedPtr<TData>::SharedPtr(TData* pPtr) noexcept
        : m_pPtr(pPtr)
        , m_pRefCounter(new jpt_private::ReferenceCounter(1, 0))
    {
    }

    template<typename TData>
    constexpr SharedPtr<TData>::SharedPtr(const SharedPtr& other)
        : m_pPtr(other.m_pPtr)
        , m_pRefCounter(other.m_pRefCounter)
    {
        IncrementStrongRef();
    }

    template<typename TData>
    constexpr SharedPtr<TData>::SharedPtr(const WeakPtr<TData>& weakPtr)
        : m_pPtr(weakPtr.m_pPtr)
        , m_pRefCounter(weakPtr.m_pRefCounter)
    {
        IncrementStrongRef();
    }

    template<typename TData>
    constexpr SharedPtr<TData>::SharedPtr(SharedPtr&& other) noexcept
        : m_pPtr(other.m_pPtr)
        , m_pRefCounter(other.m_pRefCounter)
    {
        other.m_pPtr = nullptr;
        other.m_pRefCounter = nullptr;
    }

    template<typename TData>
    SharedPtr<TData>& SharedPtr<TData>::operator=(const SharedPtr& other)
    {
        if (this != &other)
        {
            InternalReset(other.m_pPtr);
            m_pRefCounter = other.m_pRefCounter;
            IncrementStrongRef();
        }

        return *this;
    }

    template<typename TData>
    SharedPtr<TData>& SharedPtr<TData>::operator=(const WeakPtr<TData>& weakPtr)
    {
        InternalReset(weakPtr.m_pPtr);
        m_pRefCounter = weakPtr.m_pRefCounter;
        IncrementStrongRef();

        return *this;
    }

    template<typename TData>
    SharedPtr<TData>& SharedPtr<TData>::operator=(SharedPtr&& other) noexcept
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
    constexpr SharedPtr<TData>::~SharedPtr()
    {
        Reset(nullptr);
    }

    template<typename TData>
    constexpr int32 SharedPtr<TData>::GetRefCount() const
    {
        if (m_pRefCounter)
        {
            return m_pRefCounter->GetStrongRefs();
        }

        return 0;
    }

    template<typename TData>
    template<typename TDeleter>
    constexpr void SharedPtr<TData>::Reset(TData* pPtr/* = nullptr*/, const TDeleter& deleter/* = TDeleter()*/)
    {
        // If the old pointer was non-empty, deletes the previously managed object
        if (m_pPtr != pPtr)
        {
            InternalReset(pPtr, deleter);

            if (m_pPtr)
            {
                m_pRefCounter = new jpt_private::ReferenceCounter(1, 0);
            }
        }
    }

    template<typename TData>
    template<typename TDeleter>
    constexpr void SharedPtr<TData>::InternalReset(TData* pPtr, const TDeleter& deleter/* = TDeleter()*/)
    {
        if (m_pRefCounter)
        {
            m_pRefCounter->DecrementStrongRef();
            if (!m_pRefCounter->HasAnyRef())
            {
                JPT_DELETE(m_pRefCounter);
            }
        }

        if (!m_pRefCounter || !m_pRefCounter->HasAnyStrongRef())
        {
            deleter(m_pPtr);
        }

        m_pPtr = pPtr;
    }

    template<typename TData>
    constexpr void SharedPtr<TData>::IncrementStrongRef()
    {
        if (m_pRefCounter)
        {
            m_pRefCounter->IncrementStrongRef();
        }
    }
}